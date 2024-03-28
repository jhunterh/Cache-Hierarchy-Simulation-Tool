#ifndef CHS_I_CACHE_H
#define CHS_I_CACHE_H

#include <cinttypes>
#include <vector>
#include <memory>
#include <deque>

#include "types.h"
#include "ireplacementpolicy.h"

namespace CacheHierarchySimulator
{

struct CacheEntry
{
    bool valid = 0;
    uint32_t tag = 0;
};

enum WritePolicy
{
    WRITE_ALLOCATE,
    WRITE_AROUND
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

class CacheException : public std::exception {};
class InvalidCacheSizeException : public CacheException {};
class InvalidAddressSizeException : public CacheException {};
class InvalidBlockSizeException : public CacheException {};
class InvalidAssociativitySizeException : public CacheException {};
class MismatchedAddressSpaceException : public CacheException {};

/**
 * Interface for representing a cache 
*/
class ICache
{
public:

    virtual std::unique_ptr<ICache> createInstance() const = 0;
    virtual void initialize(AddressSize addressSize) = 0;

    CacheResult read(Address address);
    CacheResult write(Address address);

    void replaceLine(Address address);

    Latency getLatency();

    ModuleStats getStats();

};

}

#endif // CHS_I_CACHE_H