#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdint>

#include "api.h"

class DatafileController {
public:
    void addEntry(CacheHierarchySimulator::Instruction entry);

    pid_t getCurrentPid();
    void setCurrentPid(pid_t newPid);

    void flushEntryBufferToFile();

    uint64_t getEntryIdx();

    unsigned int getFileIdx();

    std::string getExeName();
    void setExeName(std::string newName);

private:
    std::vector<CacheHierarchySimulator::Instruction> m_entryBuffer;
    std::ofstream m_outFile;
    uint64_t m_entryIdx = 0;
    pid_t m_currentPid = 0;
    unsigned int m_fileIdx = 0;
    std::string m_exeName = "";
};