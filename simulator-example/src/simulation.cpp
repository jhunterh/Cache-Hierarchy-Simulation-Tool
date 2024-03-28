#include "api.h"
#include "basiccache.h"

// Example simulation run (with no instructions)
int main(int argv, char* argc)
{
    // Api instantiation
    CacheHierarchySimulator::Api api;

    // Create an l1 cache
    size_t addressSize = 64;
    CacheHierarchySimulator::BasicCache l1(addressSize, 8192, 64, 4, 5, CacheHierarchySimulator::WRITE_ALLOCATE, CacheHierarchySimulator::ReplacementPolicy::FIFO);

    // Create a new core
    CacheHierarchySimulator::Core core(addressSize);

    // Add cache to core
    core.addCache(l1);
    
    // Create system and add 4 cores of the same type
    CacheHierarchySimulator::System system(addressSize, 10);
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