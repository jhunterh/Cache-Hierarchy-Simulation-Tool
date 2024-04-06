#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "configparser.h"

#include "system.h"
#include "core.h"
#include "basiccache.h"
#include "types.h"

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
            std::cout << "Basic Cache: " << std::endl;
            std::cout << "associativity: " << cache_obj["associativity"] << std::endl;
            std::cout << "block size: " << cache_obj["block_size"] << std::endl;
            std::cout << "capacity: " << cache_obj["capacity"] << std::endl;
            std::cout << "latency: " << cache_obj["latency"] << std::endl;
            std::cout << "replacement policy: " << cache_obj["replacement_policy"] << std::endl;
            std::cout << "write policy: " << cache_obj["write_policy"] << std::endl;
            std::string write_policy_string(cache_obj["write_policy"]);
            std::string replacement_policy_string(cache_obj["replacement_policy"]);
            //BasicCache cache(64, cache_obj["capacity"], cache_obj["block_size"], 
                                //cache_obj["associativity"], cache_obj["latency"],
                                //WritePolicy::WRITE_ALLOCATE, );
        }
        system.addCore(core);
    }

    for (json::iterator cache_it = config["system"]["shared_caches"].begin(); cache_it != config["system"]["shared_caches"].end(); ++cache_it) 
    {
        json cache_obj = cache_it.value();
        std::cout << "Shared Cache: " << std::endl;
        std::cout << "associativity: " << cache_obj["associativity"] << std::endl;
        std::cout << "block size: " << cache_obj["block_size"] << std::endl;
        std::cout << "capacity: " << cache_obj["capacity"] << std::endl;
        std::cout << "latency: " << cache_obj["latency"] << std::endl;
        std::cout << "replacement policy: " << cache_obj["replacement_policy"] << std::endl;
        std::cout << "write policy: " << cache_obj["write_policy"] << std::endl;
    }

    return system;
}