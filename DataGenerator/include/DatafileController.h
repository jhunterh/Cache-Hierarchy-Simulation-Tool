#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

#include "pin.H"

class DatafileController {
public:
    enum MemoryAccessType
    {
        LOAD = 0,
        STORE = 1
    };

    struct DatafileEntry
    {
        UINT32 pid = 0;
        MemoryAccessType accessType = LOAD;
        ADDRINT effectiveAddress = 0;
        UINT64 timeStamp = 0;

        DatafileEntry() = default;

        DatafileEntry(UINT32 processid, MemoryAccessType type, ADDRINT address, UINT64 time) :
            pid(processid),
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