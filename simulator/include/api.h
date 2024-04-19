#ifndef CHS_API_H
#define CHS_API_H

#include "instruction.h"
#include "system.h"
#include "replacementpolicy/policylist.h"

namespace CacheHierarchySimulator
{

// TODO: Add error codes for api

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
    size_t getSystemCount();
    void clearSystems();

    void resetSystemState();

    std::vector<SystemStats> runSimulation(std::vector<SimulatorInstruction> instructionList);

private:

    std::vector<System> systemList;
};

}

#endif // CHS_API_H