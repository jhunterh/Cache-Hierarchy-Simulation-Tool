#ifndef CHS_SET_ASSOCIATIVE_CACHE_H
#define CHS_SET_ASSOCIATIVE_CACHE_H

#include <vector>

#include "cache.h"
#include "ireplacementpolicy.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a standard Set Associative cache 
*/
class SetAssociativeCache : public Cache
{
public:

    SetAssociativeCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency, const ReplacementPolicy::IReplacementPolicy& replacementPolicy, WritePolicy writePolicy);
    ~SetAssociativeCache();

    CacheResult read(Address address);
    CacheResult write(Address address);

    void replaceLine(Address address);

private:

    CacheResult access(Address address);

    CacheFields getFieldsFromAddress(Address address);

    AddressSize addressSize;
    CacheSize cacheSize;
    BlockSize blockSize;
    AssociativitySize associativity;

    ReplacementPolicy::PolicyPtr replacementPolicy;

    AddressMask tagMask;
    AddressMask indexMask;

    FieldOffset tagOffset;
    FieldOffset indexOffset;
    
    std::vector<CacheEntry> entryTable;
};

}

#endif // CHS_SET_ASSOCIATIVE_CACHE_H