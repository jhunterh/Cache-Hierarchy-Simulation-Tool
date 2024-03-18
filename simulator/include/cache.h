#ifndef CHS_CACHE_H
#define CHS_CACHE_H

#include <cinttypes>
#include <vector>
#include <memory>

#include "types.h"

namespace CacheHierarchySimulator
{

struct CacheEntry
{
    bool valid = 0;
    uint32_t tag = 0;
};

enum ReplacementPolicy
{
    FIFO,
    LIFO,
    LRU,
    PLRU,
    MRU,
    LFU
};

// Typedefs
typedef uint32_t CacheSize;
typedef uint32_t BlockSize;
typedef uint32_t AssociativitySize;

typedef uint64_t AddressMask;

typedef uint8_t FieldOffset;

typedef uint32_t Tag;
typedef uint16_t Index;
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

    CacheReturn read(Address address);
    CacheReturn write(Address address);

    Latency getLatency();

    ModuleStats getStats();

private:

    ModuleStats cacheStats;

    CacheFields getFieldsFromAddress(Address address);

    AddressSize addressSize;
    CacheSize cacheSize;
    BlockSize blockSize;
    AssociativitySize associativity;
    Latency latency;

    AddressMask tagMask;
    AddressMask indexMask;

    FieldOffset tagOffset;
    FieldOffset indexOffset;
    
    // TODO: May have to add Set struct, depending on how Replacement is implemented
    std::vector<CacheEntry> entryTable;
};

}

#endif // CHS_CACHE_H