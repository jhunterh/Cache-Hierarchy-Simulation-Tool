#include "doctest.h"
#include "api.h"
#include "basiccache.h"
#include "policylist.h"

TEST_CASE("Test Api constructor")
{
    // Test that constructor does not throw an error
    REQUIRE_NOTHROW(
        CacheHierarchySimulator::Api();
    );
}

TEST_CASE("Test Api deconstructor")
{
    CacheHierarchySimulator::Api* api = new CacheHierarchySimulator::Api();

    // Test that deconstructor does not throw
    REQUIRE_NOTHROW(
        delete api;
    ); 
}

class TestApiFixture {

public:
    TestApiFixture()
    {
        api = new CacheHierarchySimulator::Api();
    };

    ~TestApiFixture()
    {
        delete api;
    };

protected:
    CacheHierarchySimulator::Api* api;

};

TEST_CASE_FIXTURE(TestApiFixture, "Test addSystem method")
{
    // Make sure no systems are present
    REQUIRE_EQ(api->getSystemCount(), 0);

    // Add system
    CacheHierarchySimulator::System system(64, 1, 2);
    REQUIRE_NOTHROW(api->addSystem(system));

    // Make sure system is present
    REQUIRE_EQ(api->getSystemCount(), 1);
}

TEST_CASE_FIXTURE(TestApiFixture, "Test getSystemCount method")
{
    // Make sure no systems are present
    REQUIRE_EQ(api->getSystemCount(), 0);

    // Add system
    CacheHierarchySimulator::System system(64, 1, 2);
    api->addSystem(system);

    // Make sure system is present
    REQUIRE_EQ(api->getSystemCount(), 1);

    // Clear system
    api->clearSystems();

    // Make sure no systems are present
    REQUIRE_EQ(api->getSystemCount(), 0);
}

TEST_CASE_FIXTURE(TestApiFixture, "Test clearSystems method")
{
    // Add system
    CacheHierarchySimulator::System system(64, 1, 2);
    api->addSystem(system);

    // Make sure system is present
    REQUIRE_EQ(api->getSystemCount(), 1);

    // Clear system
    REQUIRE_NOTHROW(api->clearSystems());

    // Make sure no systems are present
    REQUIRE_EQ(api->getSystemCount(), 0);
}

TEST_CASE_FIXTURE(TestApiFixture, "Test resetSystemState method")
{
    // Add system
    CacheHierarchySimulator::System system(64, 1, 2);
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    system.addCoreCache(cache);
    api->addSystem(system);

    // Execute read instruction
    CacheHierarchySimulator::Instruction instruction;
    instruction.address = 0x1234;
    instruction.isWrite = false;
    std::vector<CacheHierarchySimulator::Instruction> instructionList { instruction };
    auto statList = api->runSimulation(instructionList);

    // Check that stats show 1 read
    REQUIRE_EQ(statList.size(), 1);
    auto systemStats = statList.at(0).totalSystemStats;
    CHECK_EQ(systemStats.readMisses, 1);

    // Reset system state
    REQUIRE_NOTHROW(api->resetSystemState());

    // Re-run simulation
    statList = api->runSimulation(instructionList);

    // Check that stats show 1 read
    REQUIRE_EQ(statList.size(), 1);
    systemStats = statList.at(0).totalSystemStats;
    CHECK_EQ(systemStats.readMisses, 1);
}

TEST_CASE_FIXTURE(TestApiFixture, "Test runSimulation method")
{
    // Initialize system
    CacheHierarchySimulator::System system(64, 10, 2);
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    system.addCoreCache(cache);
    system.addSharedCache(cache);
    api->addSystem(system);

    SUBCASE("Test simulate read")
    {
        // Test read
        CacheHierarchySimulator::Instruction instruction;
        instruction.address = 0x1234;
        instruction.isWrite = false;
        std::vector<CacheHierarchySimulator::Instruction> instructionList { instruction, instruction };
        auto statList = api->runSimulation(instructionList);

        // Check correct values
        REQUIRE_EQ(statList.size(), 1);
        auto stats = statList.at(0).totalSystemStats;
        CHECK_EQ(stats.readHits, 1);
        CHECK_EQ(stats.readMisses, 1);
        CHECK_EQ(stats.writeHits, 0);
        CHECK_EQ(stats.writeMisses, 0);
        CHECK_EQ(stats.averageMemoryAccessTime, 9.25); // AMAT should be 9.25
    }

    SUBCASE("Test simulate write")
    {
        // Test write
        CacheHierarchySimulator::Instruction instruction;
        instruction.address = 0x5678;
        instruction.isWrite = true;
        std::vector<CacheHierarchySimulator::Instruction> instructionList { instruction, instruction };
        auto statList = api->runSimulation(instructionList);

        // Check correct values
        REQUIRE_EQ(statList.size(), 1);
        auto stats = statList.at(0).totalSystemStats;
        CHECK_EQ(stats.readHits, 0);
        CHECK_EQ(stats.readMisses, 0);
        CHECK_EQ(stats.writeHits, 1);
        CHECK_EQ(stats.writeMisses, 1);
        CHECK_EQ(stats.averageMemoryAccessTime, 9.25); // AMAT should be 9.25
    }
}