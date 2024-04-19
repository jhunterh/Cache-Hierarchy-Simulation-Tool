#include "datafileparser.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define ENTRIES_IN_BUFFER 100000

namespace CacheHierarchySimulator
{

DatafileParser::DatafileParser(std::string fileHeading, pid_t pid) 
    : m_fileHeading(fileHeading), m_pid(pid) 
{

}

DatafileParser::~DatafileParser()
{
    pclose(m_dataFile);
}

bool DatafileParser::initialize()
{
    bool success = true;

    if (!openNextFile())
    {
        std::cout << "Something went wrong while initializing files!" << std::endl;
        success = false;
    }

    if (!getNextInstructionWave())
    {
        std::cout << "Something went wrong while reading datafile: " << getDataFileName() << std::endl;
        success = false;
    }

    return success;
}

std::string DatafileParser::getDataFileName()
{
    std::ostringstream filename("");
    filename << m_fileHeading << "_" << std::to_string(m_fileIdx) << ".dat";
    return filename.str();
}

std::string DatafileParser::getJsonFileName()
{
    std::ostringstream filename("");
    filename << m_fileHeading << "_" << std::to_string(m_fileIdx) << ".json";
    return filename.str();
}

bool DatafileParser::getNextInstructionWave()
{
    bool success = true;

    // check if at the end of the file
    if (feof(m_dataFile))
    {
        ++m_fileIdx;
        if (!openNextFile()) // open new file if needed
        {
            success = false;
            return success;
        }
    }

    // read next wave of instructions from file
    uint64_t numEntriesToRead = (m_entriesLeft < ENTRIES_IN_BUFFER) ? m_entriesLeft : ENTRIES_IN_BUFFER;
    std::vector<Instruction> rawInstructionBuffer(numEntriesToRead);
    uint64_t uncompressedSize = sizeof(Instruction)*numEntriesToRead;
    if (fread(rawInstructionBuffer.data(), 1, uncompressedSize, m_dataFile) == uncompressedSize)
    {
        m_entriesLeft -= numEntriesToRead;

        m_instructionBuffer.clear();
        m_instructionBuffer.resize(rawInstructionBuffer.size());
        std::transform(rawInstructionBuffer.begin(), rawInstructionBuffer.end(), m_instructionBuffer.begin(), 
        [this](Instruction i)
        {
            SimulatorInstruction si;
            si.instruction = i;
            si.pid = m_pid;
            return si;
        });
    }
    else
    {
        success = false;
    }

    return success;
}

bool DatafileParser::openNextFile()
{
    bool success = true;
    if (m_dataFile != nullptr) pclose(m_dataFile);

    // read config file
    std::string configFileName = getJsonFileName();
    std::ifstream configFile(configFileName);
    json configObj = json::parse(configFile);
    configFile.close();
    m_numEntries = configObj["num_entries"];
    m_entriesLeft = m_numEntries;

    // open pipe to read data
    std::string commandName("unpigz -c ");
    std::string dataFileName = getDataFileName();
    std::cout << "Reading data at: " << dataFileName << std::endl;
    commandName.append(dataFileName);

    m_dataFile = popen(commandName.c_str(), "r");
    if (m_dataFile == nullptr)
    {
        m_hasFinished = true;
        success = false;
    }
    return success;
}

bool DatafileParser::hasFinished()
{
    return m_hasFinished;
}

SimulatorInstruction DatafileParser::getNextInstruction()
{
    SimulatorInstruction instruction = m_instructionBuffer.front();
    m_instructionBuffer.erase(m_instructionBuffer.begin());
    if (m_instructionBuffer.empty())
    {
        getNextInstructionWave();
    }
    return instruction;
}

}





