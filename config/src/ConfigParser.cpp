#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "ConfigParser.h"

void ConfigParser::parseConfigFile()
{
    std::ifstream configFile("config.json");
    if (!configFile.is_open())
    {
        std::cout << "Issue opening config file! returning. " << std::endl;
        return;
    }

    json config = json::parse(configFile);
    std::cout << config.dump() << std::endl;
}