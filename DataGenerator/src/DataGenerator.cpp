/*
    CPE 631 Project - Cache Hierarchy Simulator
    File: DataGenerator.cpp
*/
#include <iostream>
#include <fstream>
#include <string>

#include "DatafileController.h"
#include "pin.H"

// Global Output Stream
static PIN_MUTEX DatafileMutex;
static DatafileController dataFile;
static uint64_t initialAccessCount = 0; // used for fast forward calculation
static bool canInstrument = false;

KNOB<UINT64> SkipLength(KNOB_MODE_WRITEONCE,"pintool", 
      "s", "0", "Skip specified number of memory accesses before beginning trace collection (default = 0)");

KNOB<UINT64> MaxTraceLength(KNOB_MODE_WRITEONCE, "pintool", 
      "l", "0", "Max number of memory accesses in trace (default is disabled)");

KNOB<UINT64> TracePeriod(KNOB_MODE_WRITEONCE, "pintool",
	  "n", "1000000", "Period for collecting trace information. ie. trace n memory accesses, then skip n memory accesses (default is 1000000)");

// Called when load or store is encountered
VOID MemoryAccessAnalysis(ADDRINT effectiveAddress, BOOL isWrite, UINT64 timeStamp, THREADID tid) 
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
    PIN_MutexUnlock(&DatafileMutex);
}

// Instrumentation routine to handle memory instructions
VOID Instruction(INS ins, VOID *v) 
{
    // Instrument Memory Read
    if (INS_IsMemoryRead(ins)) 
    {
        if (canInstrument)
        {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYREAD_EA,
                       IARG_BOOL, false,
                       IARG_TSC,
                       IARG_THREAD_ID,
                       IARG_END);
        }
        else
        {
            std::cout << "not yet..." << std::endl;
            canInstrument = (++initialAccessCount >= SkipLength.Value());
        }
    }

    // Instrument Memory Write
    if (INS_IsMemoryWrite(ins)) {
        if (canInstrument)
        {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYWRITE_EA,
                       IARG_BOOL, true,
                       IARG_TSC,
                       IARG_THREAD_ID,
                       IARG_END);
        }
        else
        {
            std::cout << "not yet..." << std::endl;
            canInstrument = (++initialAccessCount >= SkipLength.Value());
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

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Initialize DatafileController
    dataFile.setCurrentPid(PIN_GetPid());

    // Set Flag
    canInstrument = (SkipLength.Value() == 0);

    // Start the program
    PIN_StartProgram();

    return 0;
}