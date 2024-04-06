#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

#include "datasetparser.h"

namespace CacheHierarchySimulator
{

std::vector<Instruction> parseInstructionList()
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
    std::vector<Instruction> sortedInstructionList;
    for (const std::string& filename : dataFiles)
    {
        std::cout << "Reading data for " << filename << std::endl;
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
        sortedInstructionList.insert(sortedInstructionList.end(), iList.begin(), iList.end());
    }

    // sort the final instruction list by timestamp
    std::sort(sortedInstructionList.begin(), sortedInstructionList.end(), 
    [](const Instruction& a, const Instruction& b) 
    {
        return a.cycleTime < b.cycleTime;
    });

    return sortedInstructionList;
}

};