#ifndef CHS_SYSTEM_H
#define CHS_SYSTEM_H

#include <cinttypes>
#include <vector>
#include <memory>

#include "types.h"
#include "core.h"
#include "cache.h"

namespace CacheHierarchySimulator
{

// From my understanding, cache write policies are more a system level thing
enum HitWritePolicy
{
    WRITE_THROUGH,
    WRITE_BACK
};

enum MissWritePolicy
{
    WRITE_ALLOCATE,
    NO_WRITE_ALLOCATE
};

/**
 * Class for representing a system, including cores, caches, and main memory.
 * Caches in a Core belong to that core, while Caches in the systemCaches
 * belong to the entire system and their memory is shared between cores.
*/
class System
{
public:

    struct Stats : Cache::Stats
    {
        std::vector<Cache::Stats> CoreStats;  
    };

    System(Latency memoryLatency);
    ~System();

    void addCore(const Core& core);
    void addSystemCache(const Cache& cache);

    Latency read(Address address);
    Latency write(Address address);

    System::Stats getStats();

private:

    System::Stats stats;

    std::vector<Core> coreList;
    std::vector<Cache> systemCaches;

    Latency memoryLatency;
    AddressSize addressSpace;

};

}

#endif // CHS_SYSTEM_H