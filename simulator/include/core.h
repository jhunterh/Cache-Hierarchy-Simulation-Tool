#ifndef CHS_CORE_H
#define CHS_CORE_H

#include <cinttypes>
#include <vector>
#include <memory>

#include "cacheinterface.h"

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

    void addCache(const CacheInterface& cache);

    void reset();

    AccessResult read(Address address);
    AccessResult write(Address address);

    CoreStats getStats() const;

private:

    AddressSize addressSpace;

    CoreStats stats;

    std::vector<std::unique_ptr<CacheInterface>> cacheList;

};

}

#endif // CHS_CORE_H