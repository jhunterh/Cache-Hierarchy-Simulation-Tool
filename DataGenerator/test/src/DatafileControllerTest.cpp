#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "DatafileController.h"
#include "api.h"

TEST_CASE("Add entries to the entry buffer")
{
    DatafileController controller;
    controller.setCurrentPid(0);
    controller.setExeName("data");

    CacheHierarchySimulator::Instruction entry1 = 
    {
        .pid = 1,
        .threadid = 1,
        .isWrite = true,
        .address = 1,
        .cycleTime = 1
    };

    CacheHierarchySimulator::Instruction entry2 = 
    {
        .pid = 2,
        .threadid = 2,
        .isWrite = false,
        .address = 2,
        .cycleTime = 2
    };

    SUBCASE("Add entry without flushing buffer")
    {
        controller.addEntry(entry1);
        CHECK(controller.getEntryIdx() == 1);
        controller.addEntry(entry2);
        CHECK(controller.getEntryIdx() == 2);
    }

    SUBCASE("Add entry with flushing buffer")
    {
        controller.addEntry(entry1);
        controller.addEntry(entry2);
        controller.flushEntryBufferToFile();
        CHECK(controller.getEntryIdx() == 0);
        CHECK(controller.getFileIdx() == 1);
        std::string filename("data/data_");
        filename.append(std::to_string(controller.getCurrentPid()));
        filename.append("_");
        filename.append(std::to_string(controller.getFileIdx()-1));
        filename.append(".dat");
        std::ifstream dataFile;
        dataFile.open(filename.c_str(), std::ios::in | std::ios::binary);
        CHECK(dataFile.is_open() == true);
        std::vector<CacheHierarchySimulator::Instruction> iList;
        iList.resize(2);

        dataFile.read((char*)iList.data(), sizeof(CacheHierarchySimulator::Instruction)*2);
        CHECK(iList[0].address == 1);
        CHECK(iList[0].threadid == 1);
        CHECK(iList[0].isWrite == true);
        CHECK(iList[0].pid == 1);
        CHECK(iList[0].cycleTime == 1);

        CHECK(iList[1].address == 2);
        CHECK(iList[1].threadid == 2);
        CHECK(iList[1].isWrite == false);
        CHECK(iList[1].pid == 2);
        CHECK(iList[1].cycleTime == 2);
    }
}