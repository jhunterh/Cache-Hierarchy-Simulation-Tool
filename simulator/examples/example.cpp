#include "api.h"

int main(int argc, const char* argv)
{
    // Instantiate 4 way set associative cache
    CacheHierarchySimulator::BasicCache cache(
        64, 1024, 32, 4, 2, CacheHierarchySimulator::WRITE_ALLOCATE,
        CacheHierarchySimulator::ReplacementPolicy::FIFO
    );

    // Define system and add core L1 and L2 cache and shared L3
    CacheHierarchySimulator::System system(64, 100, 4);
    system.addCoreCache(cache);
    system.addCoreCache(cache);
    system.addSharedCache(cache);

    // Add system to api
    CacheHierarchySimulator::Api api;
    api.addSystem(system);

    // Create/read in instruction list (parser would be used here)
    std::vector<CacheHierarchySimulator::SimulatorInstruction> instructionList;

    // Run simulation
    auto simulationResults = api.runSimulation(instructionList);

    return 0;
}