#ifndef CHS_INSTRUCTION_H
#define CHS_INSTRUCTION_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

struct Instruction
{
    /*  
        info encoding (8 bits):
        info[7] = load/store -> 0=load, 1=store
        info[6..0] = thread id
    */
    uint8_t info = 0;
    uint64_t address = 0;
    uint64_t cycleTime = 0;
};

struct SimulatorInstruction
{
    uint32_t pid = 0;
    Instruction instruction;
};

}

#endif // CHS_INSTRUCTION_H