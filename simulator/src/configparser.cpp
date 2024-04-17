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

std::vector<CacheHierarchySimulator::System> CacheHierarchySimulator::parseConfigFile(std::string filename)
{
    std::ifstream configFile(filename.c_str());
    if (!configFile.is_open())
    {
        std::cout << "Issue opening config file!" << std::endl;
        exit(1); // TODO: throw meaningful exception here
    }

    std::ifstream coresFile("data/cores.json");
    if (!coresFile.is_open())
    {
        std::cout << "Issue opening cores config file!" << std::endl;
        exit(1); // TODO: throw meaningful exception here
    }

    json config = json::parse(configFile);
    json cores = json::parse(coresFile);
    CoreCount coreCount = cores["core_count"];

    std::vector<System> systemList;
    json systemsList = config["systems"];
    for (json::iterator system_it = systemsList.begin(); system_it != systemsList.end(); ++system_it)
    {
        json coreCachesList = system_it.value()["core_caches"];
        json sharedCachesList = system_it.value()["shared_caches"];
        CycleTime memLatency = system_it.value()["latency"];

        System system(64, memLatency, coreCount);

        for (json::iterator cache_it = coreCachesList.begin(); cache_it != coreCachesList.end(); ++cache_it)
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
            system.addCoreCache(cache);
        }

        for (json::iterator cache_it = sharedCachesList.begin(); cache_it != sharedCachesList.end(); ++cache_it)
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

        systemList.push_back(system);
    }

    return systemList;
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