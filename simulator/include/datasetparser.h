#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <vector>

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
};

}

#endif // CHS_DATASETPARSER_H