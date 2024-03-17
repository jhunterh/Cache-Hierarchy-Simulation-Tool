#ifndef CHS_TYPES_H
#define CHS_TYPES_H

#include <cinttypes>

namespace CacheHierarchySimulator
{

enum CacheReturn
{
    CACHE_MISS = 0,
    CACHE_HIT = 1
};

typedef uint64_t Address;
typedef uint8_t AddressSize;

typedef uint32_t Latency;

}

#endif // CHS_TYPES_H