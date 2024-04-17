#include "doctest.h"
#include "core.h"
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

}

TEST_CASE_FIXTURE(TestCoreFixture, "Test reset method")
{

}

TEST_CASE_FIXTURE(TestCoreFixture, "Test read method")
{

}

TEST_CASE_FIXTURE(TestCoreFixture, "Test write method")
{

}

TEST_CASE_FIXTURE(TestCoreFixture, "Test getStats method")
{

}