#ifndef CHS_API_H
#define CHS_API_H

#include "system.h"

namespace CacheHierarchySimulator
{

struct Instruction
{
    uint8_t read_write = 0;
    uint64_t address = 0;
    uint64_t cycleTime = 0;
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

    std::vector<System::Stats> runSimulation(std::vector<Instruction> instructionList);

private:

    std::vector<System> systems;
};

}

#endif // CHS_API_H