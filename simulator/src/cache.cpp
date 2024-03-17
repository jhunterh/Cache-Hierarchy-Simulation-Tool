#include <cache.h>

namespace CacheHierarchySimulator
{

// TODO: Possibly move these to utility class/header?
inline bool isPowerOfTwo(uint64_t num)
{
    return __builtin_popcountl(num) == 1UL;
}

inline uint64_t fastLog2(uint64_t num)
{
    return __builtin_ctzl(num);
}

inline uint64_t setNBits(uint64_t numBits)
{
    return (1 << numBits) - 1;
}

template <class T>
inline void getFieldFromAddress(T fieldOut, AddressMask fieldMask, FieldOffset fieldOffset, Address addressIn)
{
    fieldOut = (addressIn >> fieldOffset) & fieldMask;
}

Cache::Cache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency) : 
        addressSize(addressSize), cacheSize(cacheSize), blockSize(blockSize), associativity(associativity), latency(latency)
{
    // Make sure cacheSize is divisible by blockSize
    if(!(cacheSize % blockSize))
    {
        // cacheSize is not divsible by blockSize, throw error
        throw InvalidCacheSizeException();
    }

    if(addressSize == 0)
    {
        throw InvalidAddressSizeException();
    }

    // Check
    if(sizeof(size_t) > addressSize)
    {
        throw InvalidAddressSizeException();
    }

    if(!isPowerOfTwo(blockSize))
    {
        throw InvalidBlockSizeException();
    }

    if(!isPowerOfTwo(associativity))
    {  
        throw InvalidAssociativitySizeException();
    }

    uint64_t blockCount = cacheSize / blockSize;

    // Calculate field sizes
    uint64_t offsetSize = fastLog2(blockSize);
    uint64_t indexSize = fastLog2(blockCount / associativity);
    uint64_t tagSize = addressSize - indexSize - offsetSize;

    // Calculate masks
    indexMask = setNBits(indexSize);
    tagMask = setNBits(tagSize);

    // Calculate offsets
    indexOffset = offsetSize;
    tagOffset = indexSize + offsetSize;

    // Add index offset for set associativity
    indexMask << associativity;
    indexOffset -= associativity;

    // Resize CacheEntry table
    entryTable.resize(blockCount);
}

Cache::~Cache()
{
    entryTable.clear();
}

CacheReturn Cache::read(size_t address)
{
    // Get cache parameters
    CacheFields fields = getFieldsFromAddress(address);

    // Find beginning of set
    std::vector<CacheEntry>::iterator setBegin = entryTable.begin() + fields.index;

    // Iterate through set
    for(std::vector<CacheEntry>::iterator it = setBegin; it < (setBegin + associativity); it++)
    {
        // If valid bit is set and tag is set, return hit
        if(it->valid && it->tag == fields.tag)
            return CACHE_HIT;
    }

    // If not found, return cache miss
    return CACHE_MISS;
}

CacheReturn Cache::write(size_t address)
{
    // Get cache parameters
    CacheFields fields = getFieldsFromAddress(address);

    // TODO: Add write
    // Dont forget to use replacement policy
    
}

CacheFields Cache::getFieldsFromAddress(Address address)
{
    CacheFields fields;

    getFieldFromAddress(fields.tag, tagMask, tagOffset, address);
    getFieldFromAddress(fields.index, indexMask, indexOffset, address);

    return fields;
}

}