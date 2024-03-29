#ifndef CHS_BASIC_CACHE_H
#define CHS_BASIC_CACHE_H

#include "cacheinterface.h"

namespace CacheHierarchySimulator
{

/**
 * Class for representing a cache 
*/
class BasicCache : public CacheInterface
{
public:

    BasicCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency, WritePolicy writePolicy, const ReplacementPolicy::PolicyInterface& replacementPolicy);
    BasicCache(const BasicCache& rhs);
    ~BasicCache();

    std::unique_ptr<CacheInterface> createInstance() const;
    void initialize(AddressSize addressSize);

    CacheResult read(Address address);
    CacheResult write(Address address);
    void replaceLine(Address address);

    Latency getLatency();
    ModuleStats getStats();

private:

    CacheResult access(Address address);

    CacheFields getFieldsFromAddress(Address address);

    Latency latency;
    WritePolicy writePolicy;
    ModuleStats stats;

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

#endif // CHS_BASIC_CACHE_H