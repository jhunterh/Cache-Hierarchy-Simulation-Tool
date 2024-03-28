#ifndef CHS_DIRECT_MAPPED_CACHE_H
#define CHS_DIRECT_MAPPED_CACHE_H

#include <vector>

#include "cache.h"
#include "ireplacementpolicy.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a direct mapped cache 
*/
class DirectMappedCache : public Cache
{
public:

    DirectMappedCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, Latency latency, WritePolicy writePolicy);
    ~DirectMappedCache();

    CacheResult read(Address address);
    CacheResult write(Address address);

    void replaceLine(Address address);

private:

    CacheResult access(Address address);

    CacheFields getFieldsFromAddress(Address address);

    AddressSize addressSize;
    CacheSize cacheSize;
    BlockSize blockSize;

    AddressMask tagMask;
    AddressMask indexMask;

    FieldOffset tagOffset;
    FieldOffset indexOffset;
    
    std::vector<CacheEntry> entryTable;
};

}

#endif // CHS_DIRECT_MAPPED_CACHE_H