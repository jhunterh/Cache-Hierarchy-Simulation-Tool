#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "DatafileController.h"

#define MAX_ENTRY_COUNT 60000000

DatafileController::DatafileController()
{
    json outData;
    outData["core_count"] = std::thread::hardware_concurrency();
    std::ofstream outFile("data/cores.json");
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open output file for cores config!" << std::endl;
    }
    outFile << std::setw(4) << outData << std::endl;
    outFile.close();
}

void DatafileController::flushEntryBufferToFile()
{
    std::string fileName("data/");
    fileName.append(m_exeName);
    fileName.append("_");
    fileName.append(std::to_string(m_currentPid));
    fileName.append("_");
    fileName.append(std::to_string(m_fileIdx++));

    size_t numBytes = m_entryIdx*sizeof(CacheHierarchySimulator::Instruction);
    json fileData;
    fileData["uncompressed_size"] = numBytes;
    fileData["num_entries"] = m_entryIdx;
    fileData["pid"] = m_currentPid;
    std::string fileNameJson(fileName);
    fileNameJson.append(".json");
    std::ofstream sizeFile(fileNameJson.c_str());
    if (!sizeFile.is_open())
    {
        std::cerr << "Failed to open output file for uncompressed file size!" << std::endl;
    }
    sizeFile << std::setw(4) << fileData << std::endl;
    sizeFile.close();

    std::string fileNameDat(fileName);
    fileNameDat.append(".dat");

    std::string pipeCommand("pigz -c > ");
    pipeCommand.append(fileNameDat);
    FILE *outFile = popen(pipeCommand.c_str(), "w");
    if (outFile == NULL)
    {
        std::cerr << "Failed to open output file!" << std::endl;
    }

    fwrite(reinterpret_cast<char*>(m_entryBuffer.data()), 1, numBytes, outFile);
    pclose(outFile);
    m_entryBuffer.clear();
    m_entryBuffer.resize(MAX_ENTRY_COUNT);
    m_entryIdx = 0;
}

void DatafileController::addEntry(CacheHierarchySimulator::Instruction entry)
{
    m_entryBuffer[m_entryIdx++] = entry;
    if (m_entryIdx >= MAX_ENTRY_COUNT)
    {
        flushEntryBufferToFile();
    }
}

uint64_t DatafileController::getEntryIdx()
{
    return m_entryIdx;
}

pid_t DatafileController::getCurrentPid()
{
    return m_currentPid;
}

unsigned int DatafileController::getFileIdx()
{
    return m_fileIdx;
}

// set current pid to new pid
void DatafileController::setCurrentPid(pid_t newPid)
{
    m_currentPid = newPid;

    // reset datafile parameters and entry buffer
    m_entryBuffer.clear();
    m_entryBuffer.resize(MAX_ENTRY_COUNT);
    m_entryIdx = 0; 
    m_fileIdx = 0;
}

std::string DatafileController::getExeName()
{
    return m_exeName;
}

void DatafileController::setExeName(std::string newName)
{
    m_exeName = newName;
}