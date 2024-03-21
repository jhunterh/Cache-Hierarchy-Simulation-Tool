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

    void startCapture();
    void stopCapture();

    void addEntry(DatafileEntry entry);

private:
    void flushEntryBufferToFile();

    std::vector<DatafileEntry> m_entryBuffer;
    std::ofstream m_outFile;
    bool m_isCapturing = false;
    uint64_t m_entryIdx = 0;
};