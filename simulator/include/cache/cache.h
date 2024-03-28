#ifndef CHS_CACHE_H
#define CHS_CACHE_H

#include <cinttypes>
#include <exception>

#include "types.h"

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
    SetIndex index;
};

class InvalidCacheSizeException : public std::exception {};
class InvalidAddressSizeException : public std::exception {};
class InvalidBlockSizeException : public std::exception {};
class InvalidAssociativitySizeException : public std::exception {};


/**
 * Abstract class for representing a cache 
*/
class Cache
{
public:

    virtual CacheResult read(Address address) = 0;
    virtual CacheResult write(Address address) = 0;
    virtual void replaceLine(Address address) = 0;

    Latency getLatency();
    WritePolicy getWritePolicy();
    ModuleStats getStats();

protected:

    Latency latency;
    WritePolicy writePolicy;
    ModuleStats stats;
};

}

#endif // CHS_CACHE_H