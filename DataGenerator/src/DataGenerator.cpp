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
    DatafileController::DatafileEntry entry(PIN_GetPid(), load_store, effectiveAddress, timeStamp);
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
    // Stop DatafileController
    dataFile.stopCapture();
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

    // Instrument at the Instruction level (fine grained)
    INS_AddInstrumentFunction(Instruction, 0);

    // Init OutFile Mutex
    PIN_MutexInit(&DatafileMutex);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Initialize DatafileController
    dataFile.startCapture();

    // Start the program
    PIN_StartProgram();

    return 0;
}