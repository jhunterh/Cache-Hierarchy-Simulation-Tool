#ifndef CHS_DATAFILEPARSER_H
#define CHS_DATAFILEPARSER_H

#include <vector>
#include <unistd.h>
#include <string>

#include "instruction.h"

namespace CacheHierarchySimulator
{

class DatafileParser
{
public:
    DatafileParser() {}
    DatafileParser(std::string fileHeading, pid_t pid);

    ~DatafileParser();

    bool initialize();
    SimulatorInstruction getNextInstruction();
    bool hasFinished();

private:
    bool getNextInstructionWave();
    std::string getDataFileName();
    std::string getJsonFileName();
    bool openNextFile();

    std::string m_fileHeading = "";
    pid_t m_pid = 0;
    unsigned int m_fileIdx = 0;
    FILE *m_dataFile = nullptr;
    uint64_t m_numEntries = 0;
    uint64_t m_entriesLeft = 0;
    std::vector<SimulatorInstruction> m_instructionBuffer;
    bool m_hasFinished = false;
};

}

#endif // CHS_DATAFILEPARSER_H