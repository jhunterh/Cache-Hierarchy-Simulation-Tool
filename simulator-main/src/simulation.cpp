#include <iostream>
#include <fstream>
#include <vector>

#include "api.h"
#include "basiccache.h"
#include "datasetparser.h"
#include "configparser.h"

// Example simulation run
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./SimulatorExample <config file> <output file>" << std::endl;
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
    std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList = CacheHierarchySimulator::parseInstructionList();
    std::cout << "Dataset Loaded!" << std::endl;

    // Run the simulation
    std::cout << "Starting Simulation." << std::endl;
    std::vector<CacheHierarchySimulator::SystemStats> stats = api.runSimulation(instructionList);

    // Open output file
    std::ofstream outputFile(argv[2], std::ios::out);
    if(!outputFile.good())
    {
        std::cerr << "Unable to open file: argv[2]" << std::endl;
        return 1;
    }

    for (size_t systemIdx = 0; systemIdx < stats.size(); systemIdx++)
    {
        outputFile << "- System #" << +systemIdx << std::endl;
        const auto& systemStat = stats.at(systemIdx);
        outputFile << "    - System averageMemoryAccessTime " << systemStat.totalSystemStats.averageMemoryAccessTime << std::endl;
        outputFile << "    - System readHits " << systemStat.totalSystemStats.readHits << std::endl;
        outputFile << "    - System readMisses " << systemStat.totalSystemStats.readMisses << std::endl;
        outputFile << "    - System writeHits " << systemStat.totalSystemStats.writeHits << std::endl;
        outputFile << "    - System writeMisses " << systemStat.totalSystemStats.writeMisses << std::endl << std::endl;

        const auto& coreStats = systemStat.coreStats;
        size_t cacheIdx = 0;
        for (size_t coreIdx = 0; coreIdx < coreStats.size(); coreIdx++)
        {

            outputFile << "    - Core #" << +coreIdx << std::endl;
            const auto& coreStat = coreStats.at(coreIdx);
            outputFile << "        - Core averageMemoryAccessTime " << coreStat.totalCoreStats.averageMemoryAccessTime << std::endl;
            outputFile << "        - Core readHits " << coreStat.totalCoreStats.readHits << std::endl;
            outputFile << "        - Core readMisses " << coreStat.totalCoreStats.readMisses << std::endl;
            outputFile << "        - Core writeHits " << coreStat.totalCoreStats.writeHits << std::endl;
            outputFile << "        - Core writeMisses " << coreStat.totalCoreStats.writeMisses << std::endl << std::endl;

            const auto& cacheStats = coreStat.cacheStats;
            for (cacheIdx = 0; cacheIdx < cacheStats.size(); cacheIdx++)
            {
                const auto& cacheStat = cacheStats.at(cacheIdx);
                outputFile << "        - L" << +(cacheIdx + 1) <<  " Cache Stats" << std::endl;
                outputFile << "            - Cache averageMemoryAccessTime " << cacheStat.averageMemoryAccessTime << std::endl;
                outputFile << "            - Cache readHits " << cacheStat.readHits << std::endl;
                outputFile << "            - Cache readMisses " << cacheStat.readMisses << std::endl;
                outputFile << "            - Cache writeHits " << cacheStat.writeHits << std::endl;
                outputFile << "            - Cache writeMisses " << cacheStat.writeMisses << std::endl << std::endl;
            }
        }

        const auto& sharedCacheStats = systemStat.sharedCacheStats;
        for (size_t sharedCacheIdx = 0; sharedCacheIdx < sharedCacheStats.size(); sharedCacheIdx++)
        {
            const auto& sharedCacheStat = sharedCacheStats.at(sharedCacheIdx);
            outputFile << "    - Shared L" << +(sharedCacheIdx + cacheIdx + 1) << " Cache Stats" << std::endl;
            outputFile << "        - Cache averageMemoryAccessTime " << sharedCacheStat.averageMemoryAccessTime << std::endl;
            outputFile << "        - Cache readHits " << sharedCacheStat.readHits << std::endl;
            outputFile << "        - Cache readMisses " << sharedCacheStat.readMisses << std::endl;
            outputFile << "        - Cache writeHits " << sharedCacheStat.writeHits << std::endl;
            outputFile << "        - Cache writeMisses " << sharedCacheStat.writeMisses << std::endl << std::endl;
        }
    }

    return 0;
}