/*
    CPE 631 Project - Cache Hierarchy Simulator
    File: DataGenerator.cpp
*/
#include <iostream>
#include <fstream>
#include <string>

#include "DatafileController.h"
#include "pin.H"

enum TraceState
{
    SKIPPING,
    COLLECTING,
    GAP
};

// Global Output Stream
static PIN_MUTEX DatafileMutex;
static PIN_MUTEX GapMutex;
static DatafileController dataFile;
static uint64_t initialAccessCount = 0; // used for fast forward calculation
static uint64_t traceCount = 0;
static uint64_t traceCountPeriod = 0;
static TraceState state = COLLECTING;
static bool doingGaps = false;
static uint64_t gapSize = 0;
static uint64_t skipSize = 0;
static uint64_t collectionSize = 0;
static uint64_t maxTraceValue = 0;

KNOB<UINT64> SkipLength(KNOB_MODE_WRITEONCE,"pintool", 
      "s", "0", "Skip specified number of memory accesses before beginning trace collection (default = 0)");

KNOB<UINT64> MaxTraceLength(KNOB_MODE_WRITEONCE, "pintool", 
      "l", "0", "Max number of memory accesses in trace for each process (default is disabled)");

KNOB<UINT64> CollectionSize(KNOB_MODE_WRITEONCE, "pintool",
	  "n", "0", "Number of elements in each trace collection group. (default is disabled)");

KNOB<UINT64> GapSize(KNOB_MODE_WRITEONCE, "pintool",
	  "g", "0", "Number of elements between each collection group. (default is disabled)");

// Called when load or store is encountered
VOID MemoryAccessAnalysis(ADDRINT effectiveAddress, BOOL isWrite, UINT64 timeStamp, THREADID tid) 
{
    switch (state)
    {
        case GAP:
        {
            PIN_MutexLock(&GapMutex);
            if (++traceCountPeriod >= gapSize)
            {
                traceCountPeriod = 0;
                state = COLLECTING;
            }
            PIN_MutexUnlock(&GapMutex);
            break;
        }
        case COLLECTING:
        default:
        {
            // Send LOAD/STORE to DatafileController
            PIN_MutexLock(&DatafileMutex);

            // detect new child process start up
            pid_t pid = PIN_GetPid();
            if (pid != dataFile.getCurrentPid())
            {
                dataFile.setCurrentPid(pid);
            }

            uint8_t info = tid;
            info |= (0x80 & (((uint8_t) isWrite) << 7));
            
            CacheHierarchySimulator::Instruction entry = 
            {
                .info = info,
                .address = effectiveAddress,
                .cycleTime = timeStamp
            };

            dataFile.addEntry(entry);

            ++traceCount;

            if (doingGaps && (++traceCountPeriod >= collectionSize))
            {
                traceCountPeriod = 0;
                state = GAP;
            }
            PIN_MutexUnlock(&DatafileMutex);

            // Detect Max Trace Count
            if ((maxTraceValue > 0) && (traceCount >= maxTraceValue))
            {
                std::cout << "Process " << dataFile.getCurrentPid() << " exiting at max trace count of " << traceCount << std::endl;
                PIN_ExitApplication(0);
            }
        }
    }
}

// Instrumentation routine to handle memory instructions
VOID Instruction(INS ins, VOID *v) 
{
    // Instrument Memory Read
    if (INS_IsMemoryRead(ins)) 
    {
        switch (state)
        {
            case SKIPPING:
            {
                if (++initialAccessCount >= skipSize)
                {
                    state = COLLECTING;
                    std::cout << "leaving skip phase" << std::endl;
                }
                break;
            }
            default:
            {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYREAD_EA,
                       IARG_BOOL, false,
                       IARG_TSC,
                       IARG_THREAD_ID,
                       IARG_END);
            }
        }
    }

    // Instrument Memory Write
    if (INS_IsMemoryWrite(ins)) {
        switch (state)
        {
            case SKIPPING:
            {
                if (++initialAccessCount >= skipSize)
                {
                    state = COLLECTING;
                    std::cout << "leaving skip phase" << std::endl;
                }
                break;
            }
            default:
            {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYWRITE_EA,
                       IARG_BOOL, true,
                       IARG_TSC,
                       IARG_THREAD_ID,
                       IARG_END);
            }
        }
    }
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Flush datafile
    dataFile.flushEntryBufferToFile();
}

int main(int argc, char *argv[]) 
{
    // Initialize PIN
    PIN_InitSymbols();
    if (PIN_Init(argc, argv)) 
    {
        std::cerr << "Error initializing PIN" << std::endl;
        return 1;
    }

    int idx = 0;
    for (int i = 0; i < argc; ++i)
    {
        if ((std::string(argv[i]).compare("--") == 0) && (i+1 < argc))
        {
            idx = i+1;
            break;
        }
    }

    std::string fullName(argv[idx]);
    std::string exeName = fullName.substr(fullName.find_last_of('/')+1);
    size_t pos = 0;
    if ((pos = exeName.find('.')) != std::string::npos)
    {
        exeName = exeName.substr(0, pos);
    }

    dataFile.setExeName(exeName);

    // Instrument at the Instruction level (fine grained)
    INS_AddInstrumentFunction(Instruction, 0);

    // Init OutFile Mutex
    PIN_MutexInit(&DatafileMutex);
    PIN_MutexInit(&GapMutex);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Initialize DatafileController
    dataFile.setCurrentPid(PIN_GetPid());

    // Set Flag
    if (SkipLength.Value() > 0)
    {
        state = SKIPPING;
        skipSize = SkipLength.Value();
        std::cout << "Starting skip phase of " << SkipLength.Value() << " memory accesses." << std::endl;
    }

    doingGaps = (CollectionSize.Value() > 0) && (GapSize.Value() > 0);
    if (doingGaps)
    {
        gapSize = GapSize.Value();
        collectionSize = CollectionSize.Value();
        std::cout << "Doing gaps of " << GapSize.Value() << " for every " << CollectionSize.Value() << " accesses traced." << std::endl;
    }

    if (MaxTraceLength.Value() > 0)
    {
        maxTraceValue = MaxTraceLength.Value();
        std::cout << "Max trace length is: " << MaxTraceLength.Value() << std::endl;
    }

    // Start the program
    PIN_StartProgram();

    return 0;
}