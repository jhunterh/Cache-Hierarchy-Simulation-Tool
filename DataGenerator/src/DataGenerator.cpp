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

// Called when load or store is encountered
VOID MemoryAccessAnalysis(ADDRINT effectiveAddress, UINT32 load_store, UINT64 timeStamp) 
{
    // Send LOAD/STORE to DatafileController
    PIN_MutexLock(&DatafileMutex);

    // detect new child process start up
    pid_t pid = PIN_GetPid();
    if (pid != dataFile.getCurrentPid())
    {
        dataFile.setCurrentPid(pid);
    }

    DatafileController::DatafileEntry entry(pid, load_store, effectiveAddress, timeStamp);
    dataFile.addEntry(entry);
    PIN_MutexUnlock(&DatafileMutex);
}

// Instrumentation routine to handle memory instructions
VOID Instruction(INS ins, VOID *v) 
{
    // Instrument Memory Read
    if (INS_IsMemoryRead(ins)) 
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYREAD_EA,
                       IARG_UINT32, 0,
                       IARG_TSC,
                       IARG_END);
    }

    // Instrument Memory Write
    if (INS_IsMemoryWrite(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYWRITE_EA,
                       IARG_UINT32, 1,
                       IARG_TSC,
                       IARG_END);
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

    // Start the program
    PIN_StartProgram();

    return 0;
}