#include <iostream>
#include <sstream>

#include "DatafileController.h"

void DatafileController::startCapture()
{
    m_outFile.open("data.txt", std::ios::out);
    if (!m_outFile.is_open())
    {
        std::cerr << "Failed to open output file!" << std::endl;
    }
    else
    {
        m_entryBuffer.clear();
        m_isCapturing = true;
    }
}

void DatafileController::stopCapture()
{
    if (m_isCapturing)
    {
        std::stringstream outString("");
        for (auto entry : m_entryBuffer)
        {
            outString << entry.toString() << std::endl;
        }
        m_outFile << outString.str();
    }
    m_isCapturing = false;
    m_outFile.close();
}

void DatafileController::addEntry(DatafileEntry entry)
{
    if (m_isCapturing)
    {
        m_entryBuffer.push_back(entry);
    }
}

std::string DatafileController::DatafileEntry::toString()
{
    std::stringstream returnString("");
    if (accessType == LOAD)
    {
        returnString << "LOAD ";
    }
    else
    {
        returnString << "STORE ";
    }
    returnString << effectiveAddress << " " << timeStamp;
    return returnString.str();
}