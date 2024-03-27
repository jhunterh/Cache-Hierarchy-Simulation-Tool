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

    Core();
    ~Core();

    void addCache(const Cache& cache);

    CoreResult read(Address address);
    CoreResult write(Address address);

    CoreStats getStats();

private:

    ModuleStats stats;

    std::vector<Cache> cacheList;

};

}

#endif // CHS_CORE_H