#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <vector>

#include "DatafileController.h"

namespace CacheHierarchySimulator
{

/**
 * Class for parsing dataset files into instructions that the simulator can process
 */
class DatasetParser
{
public:
    DatasetParser() = delete;
    static std::vector<DatafileController::DatafileEntry> parseInstructionList();
};

}

#endif // CHS_DATASETPARSER_H