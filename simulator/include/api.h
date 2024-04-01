#ifndef CHS_API_H
#define CHS_API_H

#include "system.h"
#include "replacementpolicy/policylist.h"

namespace CacheHierarchySimulator
{

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

/**
 * Api for adding systems and running simulations on a list of instructions.
 * Has mutiple systems to allow for system comparisons
 * Will return a list of structs containing simulation results.
*/
class Api
{
public:

    Api();
    ~Api();

    void addSystem(const System& system);
    void clearSystems();

    std::vector<SystemStats> runSimulation(std::vector<Instruction> instructionList);

private:

    std::vector<System> systems;
};

}

#endif // CHS_API_H