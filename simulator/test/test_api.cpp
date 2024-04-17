#include "doctest.h"
#include "api.h"

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