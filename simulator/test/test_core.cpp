#include "doctest.h"
#include "core.h"

TEST_CASE("Test Core constructor")
{

}

TEST_CASE("Test Core deconstructor")
{
    
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