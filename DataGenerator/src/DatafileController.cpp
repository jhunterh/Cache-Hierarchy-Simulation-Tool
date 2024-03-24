#include <iostream>
#include <sstream>
#include <string>

#include "DatafileController.h"

#define MAX_ENTRY_COUNT 60000000

void DatafileController::flushEntryBufferToFile()
{
    std::string filename("data/data_");
    filename.append(std::to_string(m_currentPid));
    filename.append("_");
    filename.append(std::to_string(m_fileIdx++));
    filename.append(".dat");
    m_outFile.open(filename.c_str(), std::ios::out | std::ios::binary);
    if (!m_outFile.is_open())
    {
        std::cerr << "Failed to open output file!" << std::endl;
    }

    m_outFile.write(reinterpret_cast<char*>(&m_entryIdx), sizeof(uint64_t)); // write number of entries in data file first
    m_outFile.write(reinterpret_cast<char*>(&m_entryBuffer[0]), m_entryIdx*sizeof(DatafileEntry));
    m_outFile.close();
    m_entryBuffer.clear();
    m_entryBuffer.resize(MAX_ENTRY_COUNT);
    m_entryIdx = 0;
}

void DatafileController::addEntry(DatafileEntry entry)
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

std::string DatafileController::DatafileEntry::toString()
{
    std::stringstream returnString("");
    returnString << std::to_string(pid);
    if (load_or_store == 0)
    {
        returnString << " LOAD ";
    }
    else
    {
        returnString << " STORE ";
    }
    returnString << effectiveAddress << " " << timeStamp;
    return returnString.str();
}