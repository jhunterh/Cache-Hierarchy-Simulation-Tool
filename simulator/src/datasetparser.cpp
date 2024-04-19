#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "datasetparser.h"

namespace CacheHierarchySimulator
{

std::vector<SimulatorInstruction> parseInstructionList()
{
    // get datafile names from data folder
    std::vector<std::string> dataFiles;
    for (const auto& file : std::filesystem::directory_iterator("data")) 
    {
        if (std::filesystem::is_regular_file(file) && file.path().extension() == ".dat") 
        {
            dataFiles.push_back(file.path().string());
        }
    }

    // read entries from each datafile and add them to main vector
    std::vector<SimulatorInstruction> sortedInstructionList;
    for (const std::string& filename : dataFiles)
    {
        std::string sizeFileName = filename;
        sizeFileName.erase(sizeFileName.length()-4); // remove .dat file suffix
        sizeFileName.append(".json"); // add json suffix
        std::cout << "Reading config at: " << sizeFileName << std::endl;
        std::ifstream sizeFile(sizeFileName);
        if (!sizeFile.is_open())
        {
            std::cout << "Issue opening config file! Skipping" << std::endl;
            continue;
        }
        json sizeObj = json::parse(sizeFile);
        sizeFile.close();
        size_t uncompressedSize = sizeObj["uncompressed_size"];

        std::cout << "Reading data for " << filename << std::endl;
        std::string tokenString = filename.substr(filename.find_last_of('/')+1);
        std::string delimeter("_");
        tokenString.erase(tokenString.find_last_of(delimeter), tokenString.length());
        std::string pidString = tokenString.substr(tokenString.find_last_of(delimeter)+1, tokenString.length());
        if (pidString == tokenString)
        {
            std::cout << "Datafile: " << filename << " is in wrong naming format! Skipping." << std::endl;
            continue;
        }
        pid_t pid = stoi(pidString);

        std::string commandName("unpigz -c ");
        commandName.append(filename);

        FILE *dataFile = popen(commandName.c_str(), "r");

        std::vector<Instruction> iList;
        iList.resize(uncompressedSize/sizeof(Instruction));
        fread(iList.data(), 1, uncompressedSize, dataFile);
        pclose(dataFile);

        std::vector<SimulatorInstruction> siList(iList.size());
        std::transform(iList.begin(), iList.end(), siList.begin(), 
        [&pid](Instruction i)
        {
            SimulatorInstruction si;
            si.instruction = i;
            si.pid = pid;
            return si;
        });

        sortedInstructionList.insert(sortedInstructionList.end(), siList.begin(), siList.end());
    }

    // sort the final instruction list by timestamp
    std::sort(sortedInstructionList.begin(), sortedInstructionList.end(), 
    [](const SimulatorInstruction& a, const SimulatorInstruction& b) 
    {
        return a.instruction.cycleTime < b.instruction.cycleTime;
    });

    return sortedInstructionList;
}

};