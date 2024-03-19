#include <iostream>
#include <sstream>

#include "DatafileController.h"

#define MAX_ENTRY_COUNT 60000000

void DatafileController::startCapture()
{
    m_outFile.open("data.dat", std::ios::out | std::ios::binary);
    if (!m_outFile.is_open())
    {
        std::cerr << "Failed to open output file!" << std::endl;
    }
    else
    {
        m_entryBuffer.clear();
        m_entryBuffer.resize(MAX_ENTRY_COUNT);
        m_entryIdx = 0;
        m_isCapturing = true;
    }
}

void DatafileController::stopCapture()
{
    if (m_isCapturing)
    {
        flushEntryBufferToFile();
    }
    m_isCapturing = false;
    m_outFile.close();
}

void DatafileController::flushEntryBufferToFile()
{
    m_outFile.write(reinterpret_cast<char*>(&m_entryIdx), sizeof(unsigned long)); // write number of entries in data file first
    m_outFile.write(reinterpret_cast<char*>(&m_entryBuffer[0]), m_entryIdx*sizeof(DatafileEntry));
    m_entryBuffer.clear();
    m_entryBuffer.resize(MAX_ENTRY_COUNT);
    m_entryIdx = 0;
}

void DatafileController::addEntry(DatafileEntry entry)
{
    if (m_isCapturing)
    {
        m_entryBuffer[m_entryIdx++] = entry;
        if (m_entryIdx >= MAX_ENTRY_COUNT)
        {
            flushEntryBufferToFile();
        }
    }
}

std::string DatafileController::DatafileEntry::toString()
{
    std::stringstream returnString("");
    returnString << std::to_string(pid);
    if (accessType == LOAD)
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