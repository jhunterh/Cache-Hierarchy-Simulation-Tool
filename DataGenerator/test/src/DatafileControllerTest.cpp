#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "DatafileController.h"

TEST_CASE("Add entries to the entry buffer")
{
    DatafileController controller;
    controller.setCurrentPid(0);
    controller.setExeName("data");

    CacheHierarchySimulator::Instruction entry1 = 
    {
        .info = 0x81, // write = true, threadid = 1
        .address = 1,
        .cycleTime = 1
    };

    CacheHierarchySimulator::Instruction entry2 = 
    {
        .info = 0x02, // write = false, threadid = 2
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
        CHECK(iList[0].info == 0x81); // write = true, threadid = 1
        CHECK(iList[0].cycleTime == 1);

        CHECK(iList[1].address == 2);
        CHECK(iList[1].info == 0x02); // write = false, threadid = 2
        CHECK(iList[1].cycleTime == 2);
    }
}