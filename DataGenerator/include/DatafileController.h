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

        DatafileEntry() = delete;

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
    std::vector<DatafileEntry> m_entryBuffer;
    std::ofstream m_outFile;
    bool m_isCapturing = false;
};