#ifndef CHS_TYPES_H
#define CHS_TYPES_H

#include <cinttypes>
#include <vector>

namespace CacheHierarchySimulator
{

typedef uint64_t Address;
typedef uint8_t AddressSize;

typedef uint32_t AssociativitySize;
typedef AssociativitySize SetLineIdx;
typedef uint16_t SetIndex;

typedef uint64_t CycleTime;

enum AccessState
{
    CACHE_MISS = 0,
    CACHE_HIT = 1
}; 

struct AccessResult
{
    AccessState accessState;
    CycleTime accessLatency;
};

struct ModuleStats
{
    uint64_t readHits = 0;
    uint64_t readMisses = 0;
    uint64_t writeHits = 0;
    uint64_t writeMisses = 0;
    double averageMemoryAccessTime = 0;
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
    std::vector<ModuleStats> sharedCacheStats;
};

}

#endif // CHS_TYPES_H