#include "core.h"

namespace CacheHierarchySimulator
{

Core::Core(AddressSize addressSpace) : addressSpace(addressSpace)
{

}

Core::Core(const Core& rhs) : Core(addressSpace)
{
    // Copy existing caches
    for(const std::unique_ptr<CacheInterface>& cache : rhs.cacheList)
    {
        cacheList.push_back(cache->createInstance());
    }
}

Core::~Core()
{
    cacheList.clear();
}

void Core::addCache(const CacheInterface& cache)
{
    cacheList.push_back(cache.createInstance());
}

void Core::reset()
{
    for(std::unique_ptr<CacheInterface>& cache : cacheList)
    {
        cache->reset();
    }
}

AccessResult Core::read(Address address)
{
    // Initialze result
    AccessResult accessResult
    {
        .accessState = CACHE_MISS,
        .accessLatency = 0
    };

    // For each cache in list
    for(std::unique_ptr<CacheInterface>& cache : cacheList)
    {
        // Read from cache
        AccessResult result = cache->read(address);

        // Add cache latency
        accessResult.accessLatency += result.accessLatency;

        // Read from cache and determine if hit
        if(result.accessState == CACHE_HIT)
        {
            accessResult.accessState = CACHE_HIT;
            stats.readHits++;
            break;
        }
    }

    // If miss, log miss
    if(accessResult.accessState == CACHE_MISS)
    {
        stats.readMisses++;
    }

    // Return cache result
    return accessResult;
}

AccessResult Core::write(Address address)
{
    // Initialze result
    AccessResult accessResult
    {
        .accessState = CACHE_MISS,
        .accessLatency = 0
    };

    // For each cache in list
    for(std::unique_ptr<CacheInterface>& cache : cacheList)
    {
        // Write to cache
        AccessResult result = cache->write(address);

        // Add cache latency
        accessResult.accessLatency += result.accessLatency;

        // Check if cache hit
        if(result.accessState == CACHE_HIT)
        {
            accessResult.accessState = CACHE_HIT;
            stats.writeHits++;
            break;
        }
    }

    // If miss, log miss
    if(accessResult.accessState == CACHE_MISS)
    {
        stats.writeMisses++;
    }

    // Return cache result
    return accessResult;
}

CoreStats Core::getStats() const
{
    CoreStats coreStats;
    coreStats.totalCoreStats = stats;

    coreStats.cacheStats.reserve(cacheList.size());
    for(const std::unique_ptr<CacheInterface>& cache : cacheList)
    {
        coreStats.cacheStats.push_back(cache->getStats());
    }

    return coreStats;
}

};