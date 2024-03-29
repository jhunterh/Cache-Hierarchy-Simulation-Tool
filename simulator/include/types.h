#ifndef CHS_TYPES_H
#define CHS_TYPES_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

typedef uint64_t Address;
typedef uint8_t AddressSize;

typedef uint32_t AssociativitySize;
typedef AssociativitySize SetLineIdx;
typedef uint16_t SetIndex;

typedef uint32_t Latency;

enum CacheResult
{
    CACHE_MISS = 0,
    CACHE_HIT = 1
};

struct CoreResult
{
    CacheResult cacheResult;
    Latency latency;
};

struct ModuleStats
{
    uint64_t readHits = 0;
    uint64_t readMisses = 0;
    uint64_t writeHits = 0;
    uint64_t writeMisses = 0;
};

struct CoreStats
{
    ModuleStats totalCoreStats;
    uint64_t averageMemoryAccessTime = 0;
    std::vector<ModuleStats> cacheStats;  
};

struct SystemStats
{
    ModuleStats totalSystemStats;
    uint64_t averageMemoryAccessTime = 0;
    std::vector<CoreStats> coreStats;
};

}

#endif // CHS_TYPES_H