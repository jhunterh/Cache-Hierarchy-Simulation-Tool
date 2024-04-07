#pragma once

#include <string>

#include "system.h"
#include "replacementpolicy/policyinterface.h"

namespace CacheHierarchySimulator
{
    System parseConfigFile(std::string filename);
    const ReplacementPolicy::PolicyInterface& getReplacementPolicy(std::string policyString);
    WritePolicy getWritePolicy(std::string policyString);
}