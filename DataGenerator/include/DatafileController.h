#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <cstdint>

#include "instruction.h"

class DatafileController {
public:
    DatafileController();

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
    uint64_t m_entryIdx = 0;
    pid_t m_currentPid = 0;
    unsigned int m_fileIdx = 0;
    std::string m_exeName = "";
};