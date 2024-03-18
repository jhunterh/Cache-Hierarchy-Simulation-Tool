#ifndef CHS_TYPES_H
#define CHS_TYPES_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

enum CacheReturn
{
    CACHE_MISS = 0,
    CACHE_HIT = 1
};

struct ModuleStats
{
    uint64_t readHits = 0;
    uint64_t readMisses = 0;
    uint64_t writeHits = 0;
    uint64_t writeMisses = 0;
    uint64_t averageMemoryAccessTime = 0;
};

struct CoreStats
{
    ModuleStats totalCoreStats;
    std::vector<ModuleStats> cacheStats;  
};

struct SystemStats
{
    ModuleStats totalSystemStats;
    std::vector<CoreStats> coreStats;
};


typedef uint64_t Address;
typedef uint8_t AddressSize;

typedef uint32_t Latency;

}

#endif // CHS_TYPES_H