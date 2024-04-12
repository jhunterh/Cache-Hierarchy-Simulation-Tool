#include "doctest.h"
#include "firstinfirstout.h"

TEST_CASE("Test FirstInFirstOut constructor")
{
    // Test that constructor does not throw an error
    REQUIRE_NOTHROW(
        CacheHierarchySimulator::ReplacementPolicy::FirstInFirstOut();
    );
}

TEST_CASE("Test FirstInFirstOut deconstructor")
{
    auto fifo = new CacheHierarchySimulator::ReplacementPolicy::FirstInFirstOut();

    // Test that deconstructor does not throw
    REQUIRE_NOTHROW(
        delete fifo;
    );
}

class TestFirstInFirstOutFixture {

public:
    TestFirstInFirstOutFixture()
    {
        fifo = new CacheHierarchySimulator::ReplacementPolicy::FirstInFirstOut();
    };

    ~TestFirstInFirstOutFixture()
    {
        delete fifo;
    };

protected:
    CacheHierarchySimulator::ReplacementPolicy::FirstInFirstOut* fifo;

};

TEST_CASE_FIXTURE(TestFirstInFirstOutFixture, "Test createInstance method")
{
    // Test that instance creation does not throw
    auto fifo2 = 
        CacheHierarchySimulator::ReplacementPolicy::PolicyPtr(nullptr);
    REQUIRE_NOTHROW(
        fifo2 = std::move(fifo->createInstance());
    );
    
    // Test that cache2 is not null
    REQUIRE_NE(fifo2.get(), nullptr);
}

TEST_CASE_FIXTURE(TestFirstInFirstOutFixture, "Test initalize method")
{
    // Test that no error is thrown
    REQUIRE_NOTHROW(fifo->initalize(10, 10));
}

TEST_CASE_FIXTURE(TestFirstInFirstOutFixture, "Test reset method")
{
    // Initialize buffer
    REQUIRE_NOTHROW(fifo->initalize(10, 3));

    // Load test data, ensure data is correct
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 0);
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 1);

    // Test that the reset method actually resets the internal data buffer
    fifo->reset();
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 0);
}

TEST_CASE_FIXTURE(TestFirstInFirstOutFixture, "Test countAccess method")
{
    // Test that no error is thrown
    REQUIRE_NOTHROW(fifo->countAccess(10, 10));
}

TEST_CASE_FIXTURE(TestFirstInFirstOutFixture, "Test getNextReplacementIndex method")
{
    // Initialize buffer
    REQUIRE_NOTHROW(fifo->initalize(10, 3));

    // Test that value increments
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 0);
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 1);
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 2);

    // Test that the data works round-robin
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 0);

    // Test that when reset, the buffer returns the correct value
    fifo->reset();
    REQUIRE_EQ(fifo->getNextReplacementIndex(0), 0);

}
