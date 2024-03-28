#ifndef CHS_FULLY_ASSOCIATIVE_CACHE_H
#define CHS_FULLY_ASSOCIATIVE_CACHE_H

#include <vector>

#include "cache.h"
#include "ireplacementpolicy.h"

namespace CacheHierarchySimulator
{
/**
 * Class for representing a standard fully associative cache 
*/
class FullyAssociativeCache
{
public:

    FullyAssociativeCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, Latency latency, const ReplacementPolicy::IReplacementPolicy& replacementPolicy, WritePolicy writePolicy);
    ~FullyAssociativeCache();

    CacheResult read(Address address);
    CacheResult write(Address address);

    void replaceLine(Address address);

private:

    CacheResult access(Address address);

    CacheFields getFieldsFromAddress(Address address);

    ModuleStats stats;

    AddressSize addressSize;
    CacheSize cacheSize;
    BlockSize blockSize;
    AssociativitySize associativity;

    ReplacementPolicy::PolicyPtr replacementPolicy;

    AddressMask tagMask;

    FieldOffset tagOffset;
    
    std::vector<CacheEntry> entryTable;
};

}

#endif // CHS_FULLY_ASSOCIATIVE_CACHE_H