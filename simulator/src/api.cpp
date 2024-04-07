#include "api.h"

namespace CacheHierarchySimulator
{

Api::Api()
{

}

Api::~Api()
{
    systemList.clear();
}

void Api::addSystem(const System& system)
{
    systemList.push_back(system);
}

void Api::clearSystems()
{
    systemList.clear();
}

void Api::resetSystemState()
{
    for(System& system : systemList)
    {
        system.reset();
    }
}

std::vector<SystemStats> Api::runSimulation(std::vector<Instruction> instructionList)
{
    // Simulate system by system
    for(System& system : systemList)
    {
        // Simulate per system
        system.simulate(instructionList);
    }

    // Get stats
    std::vector<SystemStats> statList;
    statList.reserve(systemList.size());

    for(const System& system : systemList)
    {
        statList.push_back(system.getStats());
    }

    return statList;
}

}