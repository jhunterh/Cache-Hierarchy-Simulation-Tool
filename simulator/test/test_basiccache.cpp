#include "doctest.h"
#include "basiccache.h"
#include "policylist.h"
#include "exception.h"

TEST_CASE("Test BasicCache constructor")
{
    // Tests if, when given a non-power of two number for block size,
    // Constructor throws an error
    SUBCASE("Test invalid block size")
    {
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 992, 31, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidBlockSizeException
        );
    }

    // Tests if, when given an invalid cache size for the given block size,
    // Constructor throws an error
    SUBCASE("Test cache size not divisible by block size")
    {
        // Test when cache size is not divisible by block size
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 900, 32, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidCacheSizeException
        );
    }

    SUBCASE("Test block size larger than cache size")
    {
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 1024, 2048, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidCacheSizeException
        );
    }

    // Tests if, when given an invalid address size,
    // Constructor throws an error
    SUBCASE("Test address size is zero")
    {
        // Test address size of 0
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                0, 1024, 32, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Test address size too small")
    {
        // Test address size too small to index
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                8, 8192, 8, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    SUBCASE("Test address size too large")
    {
        // Test address size too large
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                65, 1024, 32, 1, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAddressSizeException
        );
    }

    // Tests if, when given an invalid number for associativity size,
    // Constructor throws an error
    SUBCASE("Test associativity size of zero")
    {
        // Test associativity of 0
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 1024, 32, 0, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAssociativitySizeException
        );
    }

    SUBCASE("Test associativity size not power of two")
    {
        // Test a non-power of two number for associativity size
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 1024, 32, 7, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAssociativitySizeException
        );
    }

    SUBCASE("Test associativity size too large")
    {
        // Test a too large associativity
        CHECK_THROWS_AS(
            CacheHierarchySimulator::BasicCache(
                32, 1024, 32, 2048, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            ),
            CacheHierarchySimulator::InvalidAssociativitySizeException
        );
    }

    CacheHierarchySimulator::BasicCache* cache = nullptr;
    SUBCASE("Test valid constructor")
    {
        // Test that good constructor does not throw
        REQUIRE_NOTHROW(
            cache = new CacheHierarchySimulator::BasicCache(
                32, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
                CacheHierarchySimulator::ReplacementPolicy::FIFO
            )
        );

        // Ensure cache ptr is not null
        REQUIRE_NE(cache, nullptr);

        // Check that stats start at 0
        auto stats = cache->getStats();
        CHECK_EQ(stats.readHits, 0);
        CHECK_EQ(stats.readMisses, 0);
        CHECK_EQ(stats.writeHits, 0);
        CHECK_EQ(stats.writeMisses, 0);
        CHECK_EQ(stats.averageMemoryAccessTime, 0);

        delete cache;
    }
}

TEST_CASE("Test BasicCache copy constructor")
{
    CacheHierarchySimulator::BasicCache cache = CacheHierarchySimulator::BasicCache(
        32, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    REQUIRE_NOTHROW(CacheHierarchySimulator::BasicCache cache2(cache));
}

TEST_CASE("Test BasicCache deconstructor")
{
    CacheHierarchySimulator::BasicCache* cache = new CacheHierarchySimulator::BasicCache(
        32, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    // Test that deconstructor does not throw
    REQUIRE_NOTHROW(
        delete cache;
    );
}

class TestBasicCacheFixture {

public:
    TestBasicCacheFixture()
    {
        cache = new CacheHierarchySimulator::BasicCache(
            32, 1024, 32, 4, 1, CacheHierarchySimulator::WRITE_ALLOCATE, 
            CacheHierarchySimulator::ReplacementPolicy::FIFO
        );
    };

    ~TestBasicCacheFixture()
    {
        delete cache;
    };

protected:
    CacheHierarchySimulator::BasicCache* cache;

};

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test createInstance method")
{
    // Test that instance creation does not throw
    std::unique_ptr<CacheHierarchySimulator::CacheInterface> cache2 = 
        std::unique_ptr<CacheHierarchySimulator::CacheInterface>(nullptr);
    REQUIRE_NOTHROW(
        cache2 = std::move(cache->createInstance());
    );
    
    // Test that cache2 is not null
    REQUIRE_NE(cache2.get(), nullptr);
}

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test initialize method")
{
    // Test that, when given the correct address size, no error is thrown
    CHECK_NOTHROW(cache->initialize(32));

    // Test that, when given a mismatched address size, an error is thrown
    CHECK_THROWS_AS(cache->initialize(40),
        CacheHierarchySimulator::MismatchedAddressSpaceException
    );
}

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test reset method")
{
    // Check that first cache access is miss
    CacheHierarchySimulator::AccessResult result = cache->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);    

    // Check that second cache access is hit
    result = cache->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);    

    // Reset cache
    cache->reset();

    // Check that, after reset, cache access is miss
    result = cache->read(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);    
}

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test read method")
{
    // Test cache read miss
    CacheHierarchySimulator::AccessResult result = cache->read(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);
    CHECK_EQ(result.accessLatency, 1);

    // Test cache read hit
    result = cache->read(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);
    CHECK_EQ(result.accessLatency, 1);
}

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test write method")
{
    // Test cache write miss
    CacheHierarchySimulator::AccessResult result = cache->write(0x1234);
    
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_MISS);
    CHECK_EQ(result.accessLatency, 1);    

    // Test cache write hit
    result = cache->write(0x1234);
    CHECK_EQ(result.accessState, CacheHierarchySimulator::CACHE_HIT);
    CHECK_EQ(result.accessLatency, 1); 
}

TEST_CASE_FIXTURE(TestBasicCacheFixture, "Test getStats method")
{
    // Test that stats start empty
    auto stats = cache->getStats();
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 0);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 0);

    // Do cache read
    cache->read(0x1234);

    // Test that stats return 1 cache miss
    stats = cache->getStats();
    CHECK_EQ(stats.readHits, 0);
    CHECK_EQ(stats.readMisses, 1);
    CHECK_EQ(stats.writeHits, 0);
    CHECK_EQ(stats.writeMisses, 0);
    CHECK_EQ(stats.averageMemoryAccessTime, 0);
}