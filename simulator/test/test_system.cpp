#include "doctest.h"
#include "system.h"

#include "replacementpolicy/policylist.h"
#include "basiccache.h"

TEST_CASE("Test System constructor")
{

    // Tests if, when given an invalid address size,
    // Constructor throws an error
    SUBCASE("Test address size is zero")
    {
        // Test address size of 0
        CHECK_THROWS_AS(
            CacheHierarchySimulator::System(
                0, 1, 4
            ),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Test address size too large")
    {
        // Test address size too large
        CHECK_THROWS_AS(
            CacheHierarchySimulator::System(
                72, 1, 4
            ),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Core count is too small")
    {
        // Test core count too small
        CHECK_THROWS_AS(
            CacheHierarchySimulator::System(
                64, 1, 0
            ),
            CacheHierarchySimulator::InvalidCoreCountException
        );
    }

    SUBCASE("Core count is too large")
    {
        // Test core count too large
        CHECK_THROWS_AS(
            CacheHierarchySimulator::System(
                64, 1, 129
            ),
            CacheHierarchySimulator::InvalidCoreCountException
        );
    }

    SUBCASE("Test valid constructor")
    {
        // Test that constructor does not throw an error
        REQUIRE_NOTHROW(
            CacheHierarchySimulator::System(
                64, 1, 4
            )
        );
    }
}

TEST_CASE("Test System deconstructor")
{
    CacheHierarchySimulator::System* cache = new CacheHierarchySimulator::System(
        32, 1, 4
    );

    // Test that deconstructor does not throw
    REQUIRE_NOTHROW(
        delete cache;
    );
}

class TestSystemFixture {

public:
    TestSystemFixture()
    {
        system = new CacheHierarchySimulator::System(64, 10, 2);
    };

    ~TestSystemFixture()
    {
        delete system;
    };

protected:
    CacheHierarchySimulator::System* system;

};

TEST_CASE_FIXTURE(TestSystemFixture, "Test System copy constructor")
{
    REQUIRE_NOTHROW(CacheHierarchySimulator::System system2(*system));
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test addCoreCache method")
{
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    // Make sure add cache does not throw
    REQUIRE_NOTHROW(
        system->addCoreCache(cache);
    );

    // Make sure system has correct number of core stats
    CacheHierarchySimulator::SystemStats stats = system->getStats();
    REQUIRE_EQ(stats.coreStats.at(1).cacheStats.size(), 1);
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test addSharedCache method")
{
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    // Make sure add cache does not throw
    REQUIRE_NOTHROW(
        system->addSharedCache(cache);
    );

    // make sure system stats have correct number of stats
    CacheHierarchySimulator::SystemStats stats = system->getStats();
    REQUIRE_EQ(stats.sharedCacheStats.size(), 1);
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test reset method")
{
    // Add caches to system
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    system->addCoreCache(cache);
    system->addSharedCache(cache);

    // Check stats at zero
    auto systemStats = system->getStats();
    CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 1);
    CHECK_EQ(systemStats.sharedCacheStats.size(), 1);
    
    auto stats = systemStats.totalSystemStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 12);

    // Do cache read
    CacheHierarchySimulator::SimulatorInstruction instRead;
    instRead.instruction.address = 0x1234;
    instRead.instruction.info = 0;
    std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList { instRead };
    system->simulate(instructionList);

    // Reset system
    system->reset();

    // Check still at 0
    systemStats = system->getStats();
    CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 1);
    CHECK_EQ(systemStats.sharedCacheStats.size(), 1);
    
    stats = systemStats.totalSystemStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 12);
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test simulate method")
{
    // Add caches to system
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    system->addCoreCache(cache);
    system->addSharedCache(cache);

    SUBCASE("Test stats start at zero")
    {
        // Check stats at zero
        auto systemStats = system->getStats();
        CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 1);
        CHECK_EQ(systemStats.sharedCacheStats.size(), 1);
        
        auto stats = systemStats.totalSystemStats;
        CHECK_EQ(stats.readHits, 0);
        CHECK_EQ(stats.readMisses, 0);
        CHECK_EQ(stats.writeHits, 0);
        CHECK_EQ(stats.writeMisses, 0);
        CHECK_EQ(stats.averageMemoryAccessTime, 12);
    }

    SUBCASE("Test simulate read")
    {
        // Test read
        CacheHierarchySimulator::SimulatorInstruction instruction;
        instruction.instruction.address = 0x1234;
        instruction.instruction.info = 0;
        std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList { instruction, instruction };
        system->simulate(instructionList);

        // Check correct values
        auto systemStats = system->getStats();
        auto stats = systemStats.totalSystemStats;
        CHECK_EQ(stats.readHits, 1);
        CHECK_EQ(stats.readMisses, 1);
        CHECK_EQ(stats.writeHits, 0);
        CHECK_EQ(stats.writeMisses, 0);
        CHECK_EQ(stats.averageMemoryAccessTime, 9.25); // AMAT should be 9.25
    }

    SUBCASE("Test simulate write")
    {
        // Test write
        CacheHierarchySimulator::SimulatorInstruction instruction;
        instruction.instruction.address = 0x5678;
        instruction.instruction.info = 0x80;
        std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList { instruction, instruction };
        system->simulate(instructionList);

        // Check correct values
        auto systemStats = system->getStats();
        auto stats = systemStats.totalSystemStats;
        CHECK_EQ(stats.readHits, 0);
        CHECK_EQ(stats.readMisses, 0);
        CHECK_EQ(stats.writeHits, 1);
        CHECK_EQ(stats.writeMisses, 1);
        CHECK_EQ(stats.averageMemoryAccessTime, 9.25); // AMAT should be 9.25
    }
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test getStats method")
{
    // Test that stats start at 0
    auto systemStats = system->getStats();
    CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 0);
    CHECK_EQ(systemStats.sharedCacheStats.size(), 0);

    auto stats = systemStats.totalSystemStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 10);

    // Add core and system cache
    CacheHierarchySimulator::BasicCache cache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    system->addCoreCache(cache);
    system->addSharedCache(cache);

    // Check still at 0, but vectors have elements
    systemStats = system->getStats();
    CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 1);
    CHECK_EQ(systemStats.sharedCacheStats.size(), 1);
    
    stats = systemStats.totalSystemStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 12);

    // Do cache read
    CacheHierarchySimulator::SimulatorInstruction instRead;
    instRead.instruction.address = 0x1234;
    instRead.instruction.info = 0;
    std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList { instRead };
    system->simulate(instructionList);

    // Test return values
    systemStats = system->getStats();
    CHECK_EQ(systemStats.coreStats.at(1).cacheStats.size(), 1);
    CHECK_EQ(systemStats.sharedCacheStats.size(), 1);
    
    stats = systemStats.totalSystemStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 1);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 12); // AMAT should be 12
}