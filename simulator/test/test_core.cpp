#include "doctest.h"
#include "core.h"
#include "policylist.h"
#include "basiccache.h"
#include "exception.h"

TEST_CASE("Test Core constructor")
{
    // Tests if, when given an invalid address size,
    // Constructor throws an error
    SUBCASE("Test address size is zero")
    {
        // Test address size of 0
        CHECK_THROWS_AS(
            CacheHierarchySimulator::Core(0),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Test address size too large")
    {
        // Test address size too large
        CHECK_THROWS_AS(
            CacheHierarchySimulator::Core(72),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Test valid constructor")
    {
        // Test that constructor does not throw an error
        REQUIRE_NOTHROW(
            CacheHierarchySimulator::Core(64);
        );
    }
}

TEST_CASE("Test Core deconstructor")
{
    CacheHierarchySimulator::Core* core = new CacheHierarchySimulator::Core(64);

    // Test that deconstructor does not throw
    REQUIRE_NOTHROW(
        delete core;
    );
}

class TestCoreFixture {

public:
    TestCoreFixture()
    {
        core = new CacheHierarchySimulator::Core(64);
    };

    ~TestCoreFixture()
    {
        delete core;
    };

protected:
    CacheHierarchySimulator::Core* core;

};

TEST_CASE_FIXTURE(TestCoreFixture, "Test Core copy constructor")
{
    REQUIRE_NOTHROW(CacheHierarchySimulator::Core core2(*core));
}

TEST_CASE_FIXTURE(TestCoreFixture, "Test addCache method")
{
    // Test no cache is present
    auto coreStats = core->getStats();
    REQUIRE_EQ(coreStats.cacheStats.size(), 0);

    // Add cache
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    REQUIRE_NOTHROW(
        core->addCache(cache);
    );
    
    // Test cache is present
    coreStats = core->getStats();
    REQUIRE_EQ(coreStats.cacheStats.size(), 1);
}

TEST_CASE_FIXTURE(TestCoreFixture, "Test reset method")
{
    // Add cache
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    core->addCache(cache);

    // Check that first cache access is miss
    CacheHierarchySimulator::AccessResult result = core->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);    

    // Check that second cache access is hit
    result = core->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);  

    // Test stats
    auto coreStats = core->getStats();
    auto stats = coreStats.totalCoreStats;
    CHECK_EQ(stats.readHits, 1);
    CHECK_EQ(stats.readMisses, 1);

    // Reset cache
    core->reset();

    // Test stats now empty
    coreStats = core->getStats();
    stats = coreStats.totalCoreStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);

    // Check that, after reset, cache access is miss
    result = core->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS); 
}

TEST_CASE_FIXTURE(TestCoreFixture, "Test read method")
{
    // Add cache
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    core->addCache(cache);

    // Test cache read miss
    CacheHierarchySimulator::AccessResult result = core->read(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);
    CHECK_EQ(result.accessLatency, 1);

    // Test cache read hit
    result = core->read(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);
    CHECK_EQ(result.accessLatency, 1);
}

TEST_CASE_FIXTURE(TestCoreFixture, "Test write method")
{
    // Add cache
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    core->addCache(cache);

    // Test cache write miss
    CacheHierarchySimulator::AccessResult result = core->write(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);
    CHECK_EQ(result.accessLatency, 1);    

    // Test cache write hit
    result = core->write(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);
    CHECK_EQ(result.accessLatency, 1); 
}

TEST_CASE_FIXTURE(TestCoreFixture, "Test getStats method")
{
    // Test that stats start at 0
    auto coreStats = core->getStats();
    CHECK_EQ(coreStats.cacheStats.size(), 0);

    auto stats = coreStats.totalCoreStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);

    // Add cache
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        64, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );
    core->addCache(cache);

    // Check still at 0, but vectors have elements
    coreStats = core->getStats();
    CHECK_EQ(coreStats.cacheStats.size(), 1);
    
    stats = coreStats.totalCoreStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);

    // Do core read
    core->read(0x1234);

    // Test return values
    coreStats = core->getStats();
    CHECK_EQ(coreStats.cacheStats.size(), 1);
    
    stats = coreStats.totalCoreStats;
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 1);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
}