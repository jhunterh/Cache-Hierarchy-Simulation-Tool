#ifndef CHS_CORE_H
#define CHS_CORE_H

#include <cinttypes>
#include <vector>
#include <memory>

#include "icache.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a single core.
 * This class includes a list of cache objects, as some cores have their own caches.
*/
class Core
{
public:

    Core(AddressSize addressSpace);
    Core(const Core& rhs);
    ~Core();

    void addCache(const ICache& cache);

    CoreResult read(Address address);
    CoreResult write(Address address);

    CoreStats getStats();

private:

    AddressSize addressSpace;

    ModuleStats stats;

    std::vector<std::unique_ptr<ICache>> cacheList;

};

}

#endif // CHS_CORE_H