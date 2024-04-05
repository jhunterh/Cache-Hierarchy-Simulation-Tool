#ifndef CHS_INSTRUCTION_H
#define CHS_INSTRUCTION_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

enum AccessType : uint8_t
{
    READ = 0,
    WRITE = 1
};

struct Instruction
{
    uint32_t pid = 0;
    uint8_t read_write = 0;
    uint64_t address = 0;
    uint64_t cycleTime = 0;

    Instruction() = default;

    Instruction(uint32_t processid, uint8_t load_store, uint64_t address_, uint64_t time) :
        pid(processid),
        read_write(load_store), 
        address(address_), 
        cycleTime(time) {}
};

}

#endif // CHS_INSTRUCTION_H