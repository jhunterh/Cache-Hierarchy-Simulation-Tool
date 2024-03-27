#include "core.h"

namespace CacheHierarchySimulator
{

Core::Core()
{

}

Core::~Core()
{

}

void Core::addCache(const Cache& cache)
{
    cacheList.push_back(cache);
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
    for(Cache& cache : cacheList)
    {
        // Add cache latency
        cacheResult.latency += cache.getLatency();

        // Read from cache and determine if hit
        if(cache.read(address))
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
    
}

CoreStats Core::getStats()
{
    
}

};