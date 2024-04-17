#pragma once

#include <string>
#include <vector>

#include "system.h"
#include "replacementpolicy/policyinterface.h"

namespace CacheHierarchySimulator
{
    std::vector<System> parseConfigFile(std::string filename);
    const ReplacementPolicy::PolicyInterface& getReplacementPolicy(std::string policyString);
    WritePolicy getWritePolicy(std::string policyString);
}