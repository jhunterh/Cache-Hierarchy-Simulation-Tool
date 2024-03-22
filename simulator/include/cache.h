#ifndef CHS_CACHE_H
#define CHS_CACHE_H

#include <cinttypes>
#include <vector>
#include <memory>
#include <deque>

#include "types.h"
#include "replacementpolicy.h"

namespace CacheHierarchySimulator
{

struct CacheEntry
{
    bool valid = 0;
    uint32_t tag = 0;
};

enum WritePolicy
{
    READ_ONLY,
    WRITE_THROUGH,
    WRITE_THROUGH_ALLOCATE,
    WRITE_BACK,
    WRITE_BACK_ALLOCATE
};

// Typedefs
typedef uint32_t CacheSize;
typedef uint32_t BlockSize;

typedef uint64_t AddressMask;

typedef uint8_t FieldOffset;

typedef uint32_t Tag;
struct CacheFields
{
    Tag tag;
    Index index;
};

class InvalidCacheSizeException : public std::exception {};
class InvalidAddressSizeException : public std::exception {};
class InvalidBlockSizeException : public std::exception {};
class InvalidAssociativitySizeException : public std::exception {};

// TODO: Switch to Abstract class to allow for new cache types and
// potentially split into direct mapped, set associative, and fully associative
/**
 * Class for representing a cache 
*/
class Cache
{
public:

    // TODO: Add replacement method
    Cache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency);
    ~Cache();

    CacheResult read(Address address);
    CacheResult write(Address address);

    void replaceLine(Address address);

    Latency getLatency();
    WritePolicy getWritePolicy();

    ModuleStats getStats();

private:

    CacheResult access(Address address);

    CacheFields getFieldsFromAddress(Address address);

    ModuleStats stats;

    AddressSize addressSize;
    CacheSize cacheSize;
    BlockSize blockSize;
    AssociativitySize associativity;
    Latency latency;
    WritePolicy writePolicy;

    std::unique_ptr<ReplacementPolicy::IReplacementPolicy> replacementPolicy;

    AddressMask tagMask;
    AddressMask indexMask;

    FieldOffset tagOffset;
    FieldOffset indexOffset;
    
    std::vector<CacheEntry> entryTable;
};

}

#endif // CHS_CACHE_H