#include <iostream>

#include "api.h"
#include "basiccache.h"
#include "datasetparser.h"
#include "configparser.h"

// Example simulation run
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./SimulatorExample <config file>" << std::endl;
        return 1;
    }
    // Api instantiation
    /*CacheHierarchySimulator::Api api;

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

    // get instruction trace
    std::vector<CacheHierarchySimulator::Instruction> instructionList = CacheHierarchySimulator::parseInstructionList();

    // Run the simulation
    std::vector<CacheHierarchySimulator::SystemStats> stats = api.runSimulation(instructionList); */
    CacheHierarchySimulator::System system = CacheHierarchySimulator::parseConfigFile(argv[1]);

    return 0;
}