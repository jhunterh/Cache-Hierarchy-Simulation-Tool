#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdint>

class DatafileController {
public:
    struct DatafileEntry
    {
        uint32_t pid = 0;
        uint8_t load_or_store = 0; // 0 = load, 1 = store
        uint64_t effectiveAddress = 0;
        uint64_t timeStamp = 0;

        DatafileEntry() = default;

        DatafileEntry(uint32_t processid, uint8_t load_store, uint64_t address, uint64_t time) :
            pid(processid),
            load_or_store(load_store), 
            effectiveAddress(address), 
            timeStamp(time) {}

        std::string toString();
    };

    void addEntry(DatafileEntry entry);

    pid_t getCurrentPid();
    void setCurrentPid(pid_t newPid);

    void flushEntryBufferToFile();

    uint64_t getEntryIdx();

    unsigned int getFileIdx();

private:
    std::vector<DatafileEntry> m_entryBuffer;
    std::ofstream m_outFile;
    uint64_t m_entryIdx = 0;
    pid_t m_currentPid = 0;
    unsigned int m_fileIdx = 0;
};