#ifndef CHS_DATASETPARSER_H
#define CHS_DATASETPARSER_H

#include <vector>

#include "DatafileController.h"

namespace CacheHierarchySimulator
{

std::vector<DatafileController::DatafileEntry> parseInstructionList();

}

#endif // CHS_DATASETPARSER_H