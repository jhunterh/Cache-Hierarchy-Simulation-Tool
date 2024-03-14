/*
    CPE 631 Project - Cache Hierarchy Simulator
    File: DataGenerator.cpp
*/
#include <iostream>
#include <fstream>
#include <string>

#include "pin.H"

enum MemoryAccessType
{
    LOAD,
    STORE
};

// Global Output Stream
std::ofstream outFile;

// Called when load or store is encountered
VOID MemoryAccessAnalysis(ADDRINT effectiveAddress, MemoryAccessType type, UINT64 timeStamp) 
{
    std::string typeString("");
    switch (type) 
    {
        case LOAD:
        {
            typeString = "LOAD";
            break;
        }
        case STORE:
        {
            typeString = "STORE";
            break;
        }
        default:
        {
            std::cerr << "Unrecognized MemoryAccessType! Returning." << std::endl;
            return;
        }
    };

    // Write LOAD/STORE to output file
    outFile << typeString << " " << effectiveAddress << " " << timeStamp << std::endl;
}

// Instrumentation routine to handle memory instructions
VOID Instruction(INS ins, VOID *v) 
{
    // Instrument Memory Read
    if (INS_IsMemoryRead(ins)) 
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYREAD_EA,
                       IARG_UINT32, LOAD,
                       IARG_TSC,
                       IARG_END);
    }

    // Instrument Memory Write
    if (INS_IsMemoryWrite(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)MemoryAccessAnalysis,
                       IARG_MEMORYWRITE_EA,
                       IARG_UINT32, STORE,
                       IARG_TSC,
                       IARG_END);
    }
}

int main(int argc, char *argv[]) 
{
    // Early Abort Case
    outFile.open("data.txt", std::ios::out);
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open output file! Aborting." << std::endl;
        return 1;
    }

    // Initialize PIN
    PIN_InitSymbols();
    if (PIN_Init(argc, argv)) 
    {
        std::cerr << "Error initializing PIN" << std::endl;
        return 1;
    }

    // Instrument at the Instruction level (fine grained)
    INS_AddInstrumentFunction(Instruction, 0);

    // Start the program
    PIN_StartProgram();

    outFile.close();

    return 0;
}