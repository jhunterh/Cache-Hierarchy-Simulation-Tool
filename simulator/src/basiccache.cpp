#include <basiccache.h>

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

BasicCache::BasicCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, Latency latency, WritePolicy writePolicy, const ReplacementPolicy::PolicyInterface& replacementPolicy) : 
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

    if((indexSize + offsetSize) >= addressSize)
    {
        throw InvalidAddressSizeException();
    }

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

BasicCache::BasicCache(const BasicCache& rhs)
    : BasicCache(rhs.addressSize, rhs.cacheSize, rhs.blockSize, rhs.associativity, rhs.latency, rhs.writePolicy, *rhs.replacementPolicy)
{
    
}

BasicCache::~BasicCache()
{
    entryTable.clear();
}

std::unique_ptr<CacheInterface> BasicCache::createInstance() const
{
    return std::unique_ptr<CacheInterface>(new BasicCache(*this));
}

void BasicCache::initialize(AddressSize addressSize)
{
    if(addressSize != this->addressSize)
    {
        throw MismatchedAddressSpaceException();
    }
}

CacheResult BasicCache::read(Address address)
{
    CacheResult accessResult = this->access(address);
    if(accessResult == CACHE_HIT)
    {
        stats.readHits++;
    }
    else
    {
        stats.readMisses++;

        // If cache miss, replace cache line
        replaceLine(address);
    }
    return accessResult;
}

CacheResult BasicCache::write(Address address)
{
    CacheResult accessResult = this->access(address);
    if(accessResult == CACHE_HIT)
    {
        stats.writeHits++;
        // If cache write hit, always write to cache
    }
    else
    {
        stats.writeMisses++;

        // If cache write miss, use write policy
        if(WritePolicy::WRITE_ALLOCATE)
        {
            // If write allocate, replace line
            replaceLine(address);
        }
    }

    return accessResult;
}

void BasicCache::replaceLine(Address address)
{
    CacheFields fields = getFieldsFromAddress(address);

    SetLineIdx entryIdx = replacementPolicy->getNextReplacementIndex(fields.index);

    CacheEntry& cacheEntry = entryTable.at(fields.index + entryIdx);

    cacheEntry.tag = fields.tag;
    cacheEntry.valid = true;
}

Latency BasicCache::getLatency()
{
    return latency;
}

ModuleStats BasicCache::getStats()
{
    return stats;
}

CacheResult BasicCache::access(Address address)
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

CacheFields BasicCache::getFieldsFromAddress(Address address)
{
    CacheFields fields;

    getFieldFromAddress(fields.tag, tagMask, tagOffset, address);
    getFieldFromAddress(fields.index, indexMask, indexOffset, address);

    return fields;
}

}