#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <vector>
#include <map>
#include <unistd.h>

#include "instruction.h"
#include "datafileparser.h"

namespace CacheHierarchySimulator
{

class DatasetParser
{
public:
    DatasetParser() {}
    bool initialize();
    bool getNextInstructionWave(std::vector<SimulatorInstruction>& instructionList);

private:
    struct HeapEntry
    {
        SimulatorInstruction instruction;
        DatafileParser *parser;
    };

    struct HeapEntryComparator
    {
        // custom comparator
        bool operator()(const HeapEntry& a, const HeapEntry& b) const
        {
            return a.instruction.instruction.cycleTime > b.instruction.instruction.cycleTime;
        }
    };

    std::vector<HeapEntry> m_instructionHeap;
    std::map<pid_t, DatafileParser> m_dataMap;
};

}

#endif // CHS_DATASETPARSER_H