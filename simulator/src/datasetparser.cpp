#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "datasetparser.h"

#define INSTRUCTION_WAVE_SIZE 100000

namespace CacheHierarchySimulator
{

bool DatasetParser::getNextInstructionWave(std::vector<SimulatorInstruction>& instructionList)
{
    bool simulating = true;
    instructionList.clear();
    for (int i = 0; i < INSTRUCTION_WAVE_SIZE; ++i)
    {
        if (m_instructionHeap.empty())
        {   
            simulating =  instructionList.size() > 0;
            break;
        }
        else
        {
            HeapEntry item = m_instructionHeap.front();
            std::pop_heap(m_instructionHeap.begin(), m_instructionHeap.end(), HeapEntryComparator());
            instructionList.push_back(item.instruction);
            item.instruction = item.parser->getNextInstruction();
            if (!item.parser->hasFinished())
            {
                m_instructionHeap.push_back(item);
                std::push_heap(m_instructionHeap.begin(), m_instructionHeap.end(), HeapEntryComparator());
            }
        }
    }
    
    return simulating;
}

bool DatasetParser::initialize()
{
    bool success = true;
    // start reading files
    // get datafile names from data folder
    std::vector<std::string> dataFiles;
    for (const auto& file : std::filesystem::directory_iterator("data")) 
    {
        if (std::filesystem::is_regular_file(file) && file.path().extension() == ".dat") 
        {
            dataFiles.push_back(file.path().string());
        }
    }

    // read each datafile config and create parsers
    unsigned int numKeys = 0;
    for (const std::string& filename : dataFiles)
    {
        std::string configFileName = filename;
        configFileName.erase(configFileName.length()-4); // remove .dat file suffix
        configFileName.append(".json"); // add json suffix

        std::cout << "Reading config at: " << configFileName << std::endl;
        std::ifstream configFile(configFileName);
        if (!configFile.is_open())
        {
            std::cout << "Issue opening config file! Skipping" << std::endl;
            continue;
        }
        json configObj = json::parse(configFile);
        configFile.close();
        pid_t pid = configObj["pid"];

        std::string fileHeader = filename;
        fileHeader.erase(fileHeader.length()-4); // remove .dat file suffix
        fileHeader.erase(fileHeader.find_last_of("_"));

        // check if pid exists already
        if (m_dataMap.find(pid) == m_dataMap.end())
        {
            // need to create new parser and initialize
            DatafileParser parser(fileHeader, pid);
            m_dataMap.emplace(pid, parser);
            ++numKeys;

            if (!parser.initialize())
            {
                std::cout << "Issue initializing data file parser: " << filename << std::endl;
                success = false;
                break;
            }
        }
    }

    m_instructionHeap.clear();
    for (auto& pair : m_dataMap)
    {
        HeapEntry item;
        item.instruction = pair.second.getNextInstruction();
        item.parser = &pair.second;
        m_instructionHeap.push_back(item);
        std::push_heap(m_instructionHeap.begin(), m_instructionHeap.end(), HeapEntryComparator());
    }

    return success;
}

}