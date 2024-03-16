#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "pin.H"

class DatafileController {
public:
    enum MemoryAccessType
    {
        LOAD,
        STORE
    };

    struct DatafileEntry
    {
        MemoryAccessType accessType;
        ADDRINT effectiveAddress;
        UINT64 timeStamp;

        DatafileEntry() = default;

        DatafileEntry(MemoryAccessType type, ADDRINT address, UINT64 time) :
            accessType(type), 
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
    unsigned long m_entryIdx = 0;
};