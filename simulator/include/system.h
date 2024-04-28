#ifndef CHS_SYSTEM_H
#define CHS_SYSTEM_H

#include <cinttypes>
#include <vector>
#include <memory>

#include <unordered_map>

#include "instruction.h"
#include "types.h"
#include "core.h"
#include "cacheinterface.h"
#include "exception.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a system, including cores, caches, and main memory.
 * Caches in a Core belong to that core, while Caches in the systemCaches
 * belong to the entire system and their memory is shared between cores.
*/
class System
{
public:

    System(AddressSize addressSpace, CycleTime memoryLatency, CoreCount coreCount);
    ~System();

    void addCoreCache(const CacheInterface& cache);
    void addSharedCache(const CacheInterface& cache);

    void reset();

    void simulate(const std::vector<SimulatorInstruction>& instructionList);

    SystemStats getStats() const;

private:

    AccessResult read(Core& core, Address address);
    AccessResult write(Core& core, Address address);
    
    AddressSize addressSpace;
    CycleTime memoryLatency;
    size_t nextCoreId;

    SystemStats stats;

    std::vector<Core> coreList;
    std::vector<std::shared_ptr<CacheInterface>> sharedCacheList;

    // TODO: Add custom scheduler policies
    std::unordered_map<uint32_t, size_t> pidToCoreMap;
    size_t getCoreIdFromPid(uint32_t pid);

};

}

#endif // CHS_SYSTEM_H