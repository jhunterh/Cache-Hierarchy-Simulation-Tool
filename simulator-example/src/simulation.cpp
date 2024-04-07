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
    CacheHierarchySimulator::Api api;

    // Add the system to the api
    std::cout << "Reading Configuration File..." << std::endl;
    api.addSystem(CacheHierarchySimulator::parseConfigFile(argv[1]));
    std::cout << "System Configured!" << std::endl;

    // get instruction trace
    std::cout << "Loading Dataset..." << std::endl;
    std::vector<CacheHierarchySimulator::Instruction> instructionList = CacheHierarchySimulator::parseInstructionList();
    std::cout << "Dataset Loaded!" << std::endl;

    // Run the simulation
    std::cout << "Starting Simulation." << std::endl;
    std::vector<CacheHierarchySimulator::SystemStats> stats = api.runSimulation(instructionList);

    return 0;
}