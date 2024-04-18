#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <vector>
#include <map>
#include <unistd.h>

#include "instruction.h"

namespace CacheHierarchySimulator
{

class DatasetParser
{
public:
    DatasetParser() {}
    bool getNextInstructionWave(std::vector<SimulatorInstruction>& instructionList);

private:
    std::vector<SimulatorInstruction> parseInstructionList();
    std::map<pid_t, std::vector<SimulatorInstruction>> m_dataMap;
};

}

#endif // CHS_DATASETPARSER_H