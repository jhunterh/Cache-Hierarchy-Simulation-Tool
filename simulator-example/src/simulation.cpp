#include "api.h"

// Example simulation run (with no instructions)
int main(int argv, char* argc)
{
    // Api instantiation
    CacheHierarchySimulator::Api api;

    // Create an l1 cache
    CacheHierarchySimulator::Cache l1(64, 8192, 64, 4, 5);

    // Create a new core
    CacheHierarchySimulator::Core core;

    // Add cache to core
    core.addCache(l1);
    
    // Create system and add 4 cores of the same type
    CacheHierarchySimulator::System system(10);
    system.addCore(core);
    system.addCore(core);
    system.addCore(core);
    system.addCore(core);

    // Add the system to the api
    api.addSystem(system);

    std::vector<CacheHierarchySimulator::Instruction> instructionList;

    // Run the simulation
    std::vector<CacheHierarchySimulator::SystemStats> stats = api.runSimulation(instructionList);

    return 0;
}