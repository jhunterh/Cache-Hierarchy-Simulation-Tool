#include <iostream>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "configparser.h"

#include "system.h"
#include "core.h"
#include "basiccache.h"
#include "types.h"
#include "replacementpolicy/policylist.h"

CacheHierarchySimulator::System CacheHierarchySimulator::parseConfigFile(std::string filename)
{
    std::ifstream configFile(filename.c_str());
    if (!configFile.is_open())
    {
        std::cout << "Issue opening config file!" << std::endl;
        exit(1); // TODO: throw meaningful exception here
    }

    json config = json::parse(configFile);
    System system(64, config["system"]["latency"]);
    // iterator through each core in the system
    for (json::iterator core_it = config["system"]["cores"].begin(); core_it != config["system"]["cores"].end(); ++core_it) 
    {
        Core core(64);
        // for each core, iterate through all caches
        for (json::iterator cache_it = core_it.value()["caches"].begin(); cache_it != core_it.value()["caches"].end(); ++cache_it) 
        {
            json cache_obj = cache_it.value();

            AddressSize addressSize = 64;
            CacheSize capacity = cache_obj["capacity"];
            BlockSize blockSize = cache_obj["block_size"];
            AssociativitySize associativity = cache_obj["associativity"];
            CycleTime latency = cache_obj["latency"];
            WritePolicy writePolicy = getWritePolicy(cache_obj["write_policy"]);
            const ReplacementPolicy::PolicyInterface& replacementPolicy = getReplacementPolicy(cache_obj["replacement_policy"]);
            BasicCache cache(addressSize, capacity, blockSize, associativity, latency, writePolicy, replacementPolicy);
            core.addCache(cache);
        }
        system.addCore(core);
    }

    for (json::iterator cache_it = config["system"]["shared_caches"].begin(); cache_it != config["system"]["shared_caches"].end(); ++cache_it) 
    {
        json cache_obj = cache_it.value();
        
        AddressSize addressSize = 64;
        CacheSize capacity = cache_obj["capacity"];
        BlockSize blockSize = cache_obj["block_size"];
        AssociativitySize associativity = cache_obj["associativity"];
        CycleTime latency = cache_obj["latency"];
        WritePolicy writePolicy = getWritePolicy(cache_obj["write_policy"]);
        const ReplacementPolicy::PolicyInterface& replacementPolicy = getReplacementPolicy(cache_obj["replacement_policy"]);
        BasicCache cache(addressSize, capacity, blockSize, associativity, latency, writePolicy, replacementPolicy);
        system.addSharedCache(cache);
    }

    return system;
}

const CacheHierarchySimulator::ReplacementPolicy::PolicyInterface& CacheHierarchySimulator::getReplacementPolicy(std::string policyString)
{
    static const std::map<std::string, const ReplacementPolicy::PolicyInterface&> converterMap = 
    {
        {"FIFO", ReplacementPolicy::FIFO},
        {"LIFO", ReplacementPolicy::LIFO}
    };

    if (converterMap.find(policyString) == converterMap.end())
    {
        std::cout << "Unrecognized replacement policy: " << policyString << std::endl;
        std::cout << "Using deafult replacement policy of FIFO" << std::endl;
        return ReplacementPolicy::FIFO;
    }
    else
    {
        return converterMap.at(policyString);
    }
}

CacheHierarchySimulator::WritePolicy CacheHierarchySimulator::getWritePolicy(std::string policyString)
{
    static const std::map<std::string, WritePolicy> converterMap = 
    {
        {"WRITE_ALLOCATE", WritePolicy::WRITE_ALLOCATE},
        {"WRITE_AROUND", WritePolicy::WRITE_AROUND}
    };

    WritePolicy policy(WritePolicy::WRITE_ALLOCATE);
    if (converterMap.find(policyString) == converterMap.end())
    {
        std::cout << "Unrecognized write policy: " << policyString << std::endl;
        std::cout << "Using deafult write policy of WRITE_ALLOCATE" << std::endl;
    }
    else
    {
        policy = converterMap.at(policyString);
    }
    
    return policy;
}