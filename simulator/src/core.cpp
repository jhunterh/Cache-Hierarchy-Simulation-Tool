#include "core.h"

namespace CacheHierarchySimulator
{

Core::Core(AddressSize addressSpace) : addressSpace(addressSpace)
{

}

Core::Core(const Core& rhs) : Core(addressSpace)
{
    // Copy existing caches
    for(const std::unique_ptr<ICache>& cache : rhs.cacheList)
    {
        cacheList.push_back(cache->createInstance());
    }
}

Core::~Core()
{
    cacheList.clear();
}

void Core::addCache(const ICache& cache)
{
    cacheList.push_back(cache.createInstance());
}

CoreResult Core::read(Address address)
{
    // Initialze result
    CoreResult cacheResult
    {
        .cacheResult = CACHE_MISS,
        .latency = 0
    };

    // For each cache in list
    for(std::unique_ptr<ICache>& cache : cacheList)
    {
        // Add cache latency
        cacheResult.latency += cache->getLatency();

        // Read from cache and determine if hit
        if(cache->read(address))
        {
            cacheResult.cacheResult = CACHE_HIT;
            stats.readHits++;
            break;
        }
    }

    if(cacheResult.cacheResult == CACHE_MISS)
    {
        stats.readMisses++;
    }

    // Return cache result
    return cacheResult;
}

CoreResult Core::write(Address address)
{
    // Initialze result
    CoreResult cacheResult
    {
        .cacheResult = CACHE_MISS,
        .latency = 0
    };

    // For each cache in list
    for(std::unique_ptr<ICache>& cache : cacheList)
    {
        // Add cache latency
        cacheResult.latency += cache->getLatency();

        // Write to cache and determine if hit
        if(cache->write(address))
        {
            cacheResult.cacheResult = CACHE_HIT;
            stats.writeHits++;
            break;
        }
    }

    if(cacheResult.cacheResult == CACHE_MISS)
    {
        stats.writeMisses++;
    }

    // Return cache result
    return cacheResult;
}

CoreStats Core::getStats()
{
    CoreStats coreStats;
    coreStats.totalCoreStats = stats;
    // TODO: Calculate AMAT & get cache stats

    return coreStats;
}

};