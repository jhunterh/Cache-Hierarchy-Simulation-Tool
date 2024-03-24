#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "DatafileController.h"

TEST_CASE("Add entries to the entry buffer")
{
    DatafileController controller;
    controller.setCurrentPid(0);

    DatafileController::DatafileEntry entry(0,0,0,0);

    SUBCASE("Add entry without flushing buffer")
    {
        controller.addEntry(entry);
        CHECK(controller.getEntryIdx() == 1);
    }
}