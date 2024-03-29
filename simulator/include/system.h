#ifndef CHS_SYSTEM_H
#define CHS_SYSTEM_H

#include <cinttypes>
#include <vector>
#include <memory>

#include "types.h"
#include "core.h"
#include "icache.h"

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

    System(AddressSize addressSpace, Latency memoryLatency);
    ~System();

    void addCore(const Core& core);
    void addSystemCache(const ICache& cache);

    Latency read(Address address);
    Latency write(Address address);

    SystemStats getStats();

private:

    std::vector<Core> coreList;
    std::vector<std::unique_ptr<ICache>> systemCacheList;

    Latency memoryLatency;
    AddressSize addressSpace;

};

}

#endif // CHS_SYSTEM_H