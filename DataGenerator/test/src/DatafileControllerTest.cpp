#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "DatafileController.h"
#include "api.h"

TEST_CASE("Add entries to the entry buffer")
{
    DatafileController controller;
    controller.setCurrentPid(0);
    controller.setExeName("data");

    CacheHierarchySimulator::Instruction entry1(1,1,1,1);
    CacheHierarchySimulator::Instruction entry2(2,0,2,2);

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
        CacheHierarchySimulator::Instruction readEntry(0,0,0,0);

        dataFile.read((char*)&readEntry, sizeof(CacheHierarchySimulator::Instruction));
        CHECK(readEntry.address == 1);
        CHECK(readEntry.read_write == 1);
        CHECK(readEntry.pid == 1);
        CHECK(readEntry.cycleTime == 1);

        dataFile.read((char*)&readEntry, sizeof(CacheHierarchySimulator::Instruction));
        CHECK(readEntry.address == 2);
        CHECK(readEntry.read_write == 0);
        CHECK(readEntry.pid == 2);
        CHECK(readEntry.cycleTime == 2);
    }
}