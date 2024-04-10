#ifndef CHS_INSTRUCTION_H
#define CHS_INSTRUCTION_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

struct Instruction
{
    uint32_t pid = 0;
    uint32_t threadid = 0;
    bool isWrite = false;
    uint64_t address = 0;
    uint64_t cycleTime = 0;
};

}

#endif // CHS_INSTRUCTION_H