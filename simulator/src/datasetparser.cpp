#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

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
        std::cout << "Reading data for " << filename << std::endl;
        std::string tokenString = filename.substr(filename.find_last_of('/')+1);
        std::string delimeter("_");
        tokenString.erase(0, tokenString.find(delimeter)+delimeter.length());
        std::string pidString = tokenString.substr(0, tokenString.find(delimeter));
        if (pidString == tokenString)
        {
            std::cout << "Datafile: " << filename << " is in wrong naming format! Skipping." << std::endl;
            continue;
        }
        pid_t pid = stoi(pidString);
        std::cout << "PID: " << pid << std::endl;

        std::ifstream dataFile;
        dataFile.open(filename.c_str(), std::ios::in | std::ios::binary);
        if (!dataFile.is_open())
        {
            std::cout << "Issue opening datafile (skipping): " << filename << std::endl;
            continue;
        }
        dataFile.unsetf(std::ios::skipws);
        std::streampos fileSize;
        dataFile.seekg(0, std::ios::end);
        fileSize = dataFile.tellg();
        dataFile.seekg(0, std::ios::beg);
        std::vector<Instruction> iList;
        iList.resize(fileSize/sizeof(Instruction));
        dataFile.read((char*)iList.data(), fileSize);
        dataFile.close();

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