#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "DatafileController.h"

TEST_CASE("Add entries to the entry buffer")
{
    DatafileController controller;
    controller.setCurrentPid(0);

    DatafileController::DatafileEntry entry(1,1,1,1);

    SUBCASE("Add entry without flushing buffer")
    {
        controller.addEntry(entry);
        CHECK(controller.getEntryIdx() == 1);
    }

    SUBCASE("Add entry with flushing buffer")
    {
        controller.addEntry(entry);
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
        uint64_t entryCount = 0;
        dataFile.read((char*)&entryCount, sizeof(uint64_t));
        CHECK(entryCount == 1);
        DatafileController::DatafileEntry readEntry(0,0,0,0);
        dataFile.read((char*)&readEntry, sizeof(DatafileController::DatafileEntry));
        CHECK(readEntry.effectiveAddress == 1);
        CHECK(readEntry.load_or_store == 1);
        CHECK(readEntry.pid == 1);
        CHECK(readEntry.timeStamp == 1);
    }
}