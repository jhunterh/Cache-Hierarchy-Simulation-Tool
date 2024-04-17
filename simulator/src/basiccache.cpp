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
    return static_cast<uint64_t>(__builtin_ctzl(num));
}

inline uint64_t setNBits(uint64_t numBits)
{
    return (1 << numBits) - 1;
}

template <class T>
inline void getFieldFromAddress(T& fieldOut, AddressMask fieldMask, FieldOffset fieldOffset, Address addressIn)
{
    fieldOut = static_cast<T>((addressIn >> fieldOffset) & fieldMask);
}

BasicCache::BasicCache(AddressSize addressSize, CacheSize cacheSize, BlockSize blockSize, AssociativitySize associativity, CycleTime latency, WritePolicy writePolicy, const ReplacementPolicy::PolicyInterface& replacementPolicy) : 
        addressSize(addressSize), cacheSize(cacheSize), blockSize(blockSize), associativity(associativity), latency(latency), writePolicy(writePolicy)
{
    // TODO: Either add more exceptions or add exception messages

    // Check address size
    if((sizeof(size_t)*8) < addressSize || addressSize == 0)
    {
        throw InvalidAddressSizeException();
    }

    // Make sure cacheSize is divisible by blockSize
    if(cacheSize % blockSize)
    {
        // cacheSize is not divsible by blockSize, throw error
        throw InvalidCacheSizeException();
    }

    if(!(isPowerOfTwo(blockSize) && blockSize <= cacheSize))
    {
        throw InvalidBlockSizeException();
    }

    if(!isPowerOfTwo(associativity))
    {  
        throw InvalidAssociativitySizeException();
    }

    if(associativity == 0 || associativity > cacheSize)
    {  
        throw InvalidAssociativitySizeException();
    }

    uint64_t blockCount = cacheSize / blockSize;
    uint64_t setCount = blockCount / associativity;

    // Calculate field sizes
    uint64_t offsetSize = fastLog2(blockSize);
    uint64_t indexSize = fastLog2(setCount);

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

    // Resize CacheEntry table
    entryTable.resize(blockCount);

    // Create replacement policy instance
    this->replacementPolicy = replacementPolicy.createInstance();
    this->replacementPolicy->initalize(setCount, associativity); // 2^indexSize
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

void BasicCache::reset()
{
    size_t entryTableSize = entryTable.size();
    entryTable.clear();
    entryTable.resize(entryTableSize);

    replacementPolicy->reset();
}

AccessResult BasicCache::read(Address address)
{
    AccessState accessState = this->access(address);
    if(accessState == CACHE_HIT)
    {
        stats.readHits++;
    }
    else
    {
        stats.readMisses++;

        // If cache miss, replace cache line
        replaceLine(address);
    }

    return AccessResult {
        .accessState = accessState,
        .accessLatency = latency
    };
}

AccessResult BasicCache::write(Address address)
{
    AccessState accessState = this->access(address);
    if(accessState == CACHE_HIT)
    {
        stats.writeHits++;
        // If cache write hit, always write to cache
    }
    else
    {
        stats.writeMisses++;

        // If cache write miss, use write policy
        if(writePolicy == WritePolicy::WRITE_ALLOCATE)
        {
            // If write allocate, replace line
            replaceLine(address);
        }
    }

    return AccessResult {
        .accessState = accessState,
        .accessLatency = latency
    };
}

void BasicCache::replaceLine(Address address)
{
    CacheFields fields = getFieldsFromAddress(address);

    SetLineIdx entryIdx = replacementPolicy->getNextReplacementIndex(fields.index);

    CacheEntry& cacheEntry = entryTable.at(fields.index * associativity + entryIdx);

    cacheEntry.tag = fields.tag;
    cacheEntry.valid = true;
}

ModuleStats BasicCache::getStats()
{
    return stats;
}

AccessState BasicCache::access(Address address)
{
    // Get cache parameters
    CacheFields fields = getFieldsFromAddress(address);

    // Iterate through set
    size_t startIdx = fields.index * associativity;
    for(size_t entryIdx = startIdx; entryIdx < (startIdx + associativity); entryIdx++)
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