#include <iostream>
#include <vector>

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
    std::vector<CacheHierarchySimulator::System> systemList = CacheHierarchySimulator::parseConfigFile(argv[1]);
    for (auto& system : systemList)
    {
        api.addSystem(system);
    }
    std::cout << "Systems Configured!" << std::endl;

    // get instruction trace
    std::cout << "Loading Dataset..." << std::endl;
    std::vector<CacheHierarchySimulator::Instruction> instructionList = CacheHierarchySimulator::parseInstructionList();
    std::cout << "Dataset Loaded!" << std::endl;

    // Run the simulation
    std::cout << "Starting Simulation." << std::endl;
    std::vector<CacheHierarchySimulator::SystemStats> stats = api.runSimulation(instructionList);

    for (auto& stat : stats)
    {
        for (auto& coreStat : stat.coreStats)
        {
            for (auto& cacheStat : coreStat.cacheStats)
            {
                std::cout << "L1 Cache Stats" << std::endl;
                std::cout << "L1 Cache averageMemoryAccessTime " << cacheStat.averageMemoryAccessTime << std::endl;
                std::cout << "L1 Cache readHits " << cacheStat.readHits << std::endl;
                std::cout << "L1 Cache readMisses " << cacheStat.readMisses << std::endl;
                std::cout << "L1 Cache writeHits " << cacheStat.writeHits << std::endl;
                std::cout << "L1 Cache writeMisses " << cacheStat.writeMisses << std::endl << std::endl;
            }
            std::cout << "Core Stats" << std::endl;
            std::cout << "Core averageMemoryAccessTime " << coreStat.totalCoreStats.averageMemoryAccessTime << std::endl;
            std::cout << "Core readHits " << coreStat.totalCoreStats.readHits << std::endl;
            std::cout << "Core readMisses " << coreStat.totalCoreStats.readMisses << std::endl;
            std::cout << "Core writeHits " << coreStat.totalCoreStats.writeHits << std::endl;
            std::cout << "Core writeMisses " << coreStat.totalCoreStats.writeMisses << std::endl << std::endl;
        }

        for (auto& cacheStat : stat.sharedCacheStats)
        {
            std::cout << "Shared Cache Stats" << std::endl;
            std::cout << "Shared Cache averageMemoryAccessTime " << cacheStat.averageMemoryAccessTime << std::endl;
            std::cout << "Shared Cache readHits " << cacheStat.readHits << std::endl;
            std::cout << "Shared Cache readMisses " << cacheStat.readMisses << std::endl;
            std::cout << "Shared Cache writeHits " << cacheStat.writeHits << std::endl;
            std::cout << "Shared Cache writeMisses " << cacheStat.writeMisses << std::endl << std::endl;
        }

        std::cout << "System Stats" << std::endl;
        std::cout << "System averageMemoryAccessTime " << stat.totalSystemStats.averageMemoryAccessTime << std::endl;
        std::cout << "System readHits " << stat.totalSystemStats.readHits << std::endl;
        std::cout << "System readMisses " << stat.totalSystemStats.readMisses << std::endl;
        std::cout << "System writeHits " << stat.totalSystemStats.writeHits << std::endl;
        std::cout << "System writeMisses " << stat.totalSystemStats.writeMisses << std::endl << std::endl;
    }

    return 0;
}