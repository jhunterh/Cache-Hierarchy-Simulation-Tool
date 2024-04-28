#include <iostream>
#include <fstream>
#include <vector>

#include "api.h"
#include "basiccache.h"
#include "datasetparser.h"
#include "configparser.h"

double calculateMissRate(const CacheHierarchySimulator::ModuleStats& stats)
{
    return static_cast<double>(stats.readMisses + stats.writeMisses) / 
        (stats.readHits + stats.readMisses + stats.writeHits + stats.writeMisses);
}

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
        outputFile << "    - System averageMemoryAccessTime " << systemStat.averageMemoryAccessTime << std::endl;
        outputFile << "    - System missRate " << calculateMissRate(systemStat) << std::endl;
        outputFile << "    - System readHits " << systemStat.readHits << std::endl;
        outputFile << "    - System readMisses " << systemStat.readMisses << std::endl;
        outputFile << "    - System writeHits " << systemStat.writeHits << std::endl;
        outputFile << "    - System writeMisses " << systemStat.writeMisses << std::endl << std::endl;

        const auto& coreStats = systemStat.coreStats;
        size_t cacheIdx = 0;
        for (size_t coreIdx = 0; coreIdx < coreStats.size(); coreIdx++)
        {

            outputFile << "    - Core #" << +coreIdx << std::endl;
            const auto& coreStat = coreStats.at(coreIdx);
            outputFile << "        - Core missRate " << calculateMissRate(coreStat) << std::endl;
            outputFile << "        - Core readHits " << coreStat.readHits << std::endl;
            outputFile << "        - Core readMisses " << coreStat.readMisses << std::endl;
            outputFile << "        - Core writeHits " << coreStat.writeHits << std::endl;
            outputFile << "        - Core writeMisses " << coreStat.writeMisses << std::endl << std::endl;

            const auto& cacheStats = coreStat.cacheStats;
            for (cacheIdx = 0; cacheIdx < cacheStats.size(); cacheIdx++)
            {
                const auto& cacheStat = cacheStats.at(cacheIdx);
                outputFile << "        - L" << +(cacheIdx + 1) <<  " Cache Stats" << std::endl;
                outputFile << "            - Cache missRate " << calculateMissRate(cacheStat) << std::endl;
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
            outputFile << "        - Cache missRate " << calculateMissRate(sharedCacheStat) << std::endl;
            outputFile << "        - Cache readHits " << sharedCacheStat.readHits << std::endl;
            outputFile << "        - Cache readMisses " << sharedCacheStat.readMisses << std::endl;
            outputFile << "        - Cache writeHits " << sharedCacheStat.writeHits << std::endl;
            outputFile << "        - Cache writeMisses " << sharedCacheStat.writeMisses << std::endl << std::endl;
        }
    }

    return 0;
}