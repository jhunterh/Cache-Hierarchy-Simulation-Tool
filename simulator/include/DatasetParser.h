#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <string>
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
    DatasetParser(std::string dataset = "data");

    std::vector<DatafileController::DatafileEntry> parseInstructionList();

private:
    std::vector<std::string> m_datafiles;
};

}

#endif // CHS_DATASETPARSER_H