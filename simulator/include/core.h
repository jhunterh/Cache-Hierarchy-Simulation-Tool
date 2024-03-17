#ifndef CHS_CORE_H
#define CHS_CORE_H

#include <cinttypes>
#include <vector>

#include "cache.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a single core.
 * This class includes a list of cache objects, as some cores have their own caches.
*/
class Core
{
public:

    struct Stats : Cache::Stats
    {
        std::vector<Cache::Stats> cacheStats;  
    };

    void addCache(const Cache& cache);

    CacheReturn read(Address address);
    CacheReturn write(Address address);

    Core::Stats getStats();

private:

    Core::Stats stats;

    std::vector<Cache> caches;

};

}

#endif // CHS_CORE_H