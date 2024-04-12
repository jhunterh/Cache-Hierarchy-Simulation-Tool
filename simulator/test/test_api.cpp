#include "doctest.h"
#include "api.h"

TEST_CASE("Test Api constructor")
{

}

TEST_CASE("Test Api deconstructor")
{
    
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

}

TEST_CASE_FIXTURE(TestApiFixture, "Test clearSystems method")
{

}

TEST_CASE_FIXTURE(TestApiFixture, "Test resetSystemState method")
{

}

TEST_CASE_FIXTURE(TestApiFixture, "Test runSimulation method")
{

}