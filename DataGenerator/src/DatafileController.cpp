#include <iostream>
#include <sstream>
#include <string>

#include "DatafileController.h"

#define MAX_ENTRY_COUNT 60000000

void DatafileController::flushEntryBufferToFile()
{
    std::string filename("data/");
    filename.append(m_exeName);
    filename.append("_");
    filename.append(std::to_string(m_currentPid));
    filename.append("_");
    filename.append(std::to_string(m_fileIdx++));
    filename.append(".dat");
    m_outFile.open(filename.c_str(), std::ios::out | std::ios::binary);
    if (!m_outFile.is_open())
    {
        std::cerr << "Failed to open output file!" << std::endl;
    }

    m_outFile.write(reinterpret_cast<char*>(m_entryBuffer.data()), m_entryIdx*sizeof(CacheHierarchySimulator::Instruction));
    m_outFile.close();
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
    if (m_outFile.is_open()) m_outFile.close();
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