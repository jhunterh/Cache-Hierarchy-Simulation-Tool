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

Cache::Cache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency, const ReplacementPolicy::IReplacementPolicy& replacementPolicy, WritePolicy writePolicy) : 
        addressSize(addressSize), cacheSize(cacheSize), blockSize(blockSize), associativity(associativity), latency(latency), writePolicy(writePolicy)
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

    // Create replacement policy instance
    this->replacementPolicy = replacementPolicy.createPolicyInstance();
    this->replacementPolicy->initalize(indexSize, associativity);
}

Cache::~Cache()
{
    entryTable.clear();
}

CacheResult Cache::read(Address address)
{
    CacheResult accessResult = this->access(address);
    (accessResult == CACHE_HIT) ? stats.readHits++ : stats.readMisses++;
    return accessResult;
}

CacheResult Cache::write(Address address)
{
    CacheResult accessResult = this->access(address);
    (accessResult == CACHE_HIT) ? stats.writeHits++ : stats.writeMisses++;
    return accessResult;
}

void Cache::replaceLine(Address address)
{
    CacheFields fields = getFieldsFromAddress(address);

    SetLineIdx entryIdx = replacementPolicy->getNextReplacementIndex(fields.index);

    CacheEntry& cacheEntry = entryTable.at(fields.index + entryIdx);

    cacheEntry.tag = fields.tag;
    cacheEntry.valid = true;
}

Latency Cache::getLatency()
{
    return latency;
}

WritePolicy Cache::getWritePolicy()
{
    return writePolicy;
}

CacheResult Cache::access(Address address)
{
    // Get cache parameters
    CacheFields fields = getFieldsFromAddress(address);

    // Iterate through set
    for(size_t entryIdx = fields.index; entryIdx < (fields.index + associativity); entryIdx++)
    {
        CacheEntry& cacheEntry = entryTable.at(entryIdx);

        // If valid bit is set and tag is set, return hit
        if(cacheEntry.valid && cacheEntry.tag == fields.tag)
        {
            replacementPolicy->countAccess(fields.index, entryIdx);
            return CACHE_HIT;
        }
    }

    // If not found, return cache miss
    return CACHE_MISS;
}

CacheFields Cache::getFieldsFromAddress(Address address)
{
    CacheFields fields;

    getFieldFromAddress(fields.tag, tagMask, tagOffset, address);
    getFieldFromAddress(fields.index, indexMask, indexOffset, address);

    return fields;
}

}