#include "doctest.h"
#include "system.h"

TEST_CASE("Test System constructor")
{

}

TEST_CASE("Test System deconstructor")
{
    
}

class TestSystemFixture {

public:
    TestSystemFixture()
    {
        system = new CacheHierarchySimulator::System(64, 1, 2);
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
    
}

TEST_CASE_FIXTURE(TestSystemFixture, "Test addCoreCache method")
{

}

TEST_CASE_FIXTURE(TestSystemFixture, "Test addSharedCache method")
{

}


TEST_CASE_FIXTURE(TestSystemFixture, "Test reset method")
{

}

TEST_CASE_FIXTURE(TestSystemFixture, "Test simulate method")
{

}

TEST_CASE_FIXTURE(TestSystemFixture, "Test getStats method")
{

}