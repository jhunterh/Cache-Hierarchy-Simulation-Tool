#include "system.h"

namespace CacheHierarchySimulator
{


System::System(AddressSize addressSpace, CycleTime memoryLatency, CoreCount coreCount) : addressSpace(addressSpace), memoryLatency(memoryLatency), nextCoreId(0)
{
    // Make sure address space is valid
    if((sizeof(size_t)*8) < addressSpace || addressSpace == 0)
    {
        throw InvalidAddressSizeException();
    }

    // Make sure core count is valid
    if(coreCount > 128 || coreCount < 1)
    {
        throw InvalidCoreCountException();
    }

    coreList = std::vector<Core>(coreCount, Core(addressSpace));
}

System::~System()
{
    coreList.clear();
    sharedCacheList.clear();
    pidToCoreMap.clear();
}

void System::addCoreCache(const CacheInterface& cache)
{
    // Add cache to each core
    for(Core& core : coreList)
    {
        core.addCache(cache);
    }
}

void System::addSharedCache(const CacheInterface& cache)
{
    sharedCacheList.push_back(cache.createInstance());
}

size_t System::getCoreIdFromPid(uint32_t pid)
{
    // TODO: Add scheduler
    // For now, use a hash map to assign a process to a core,
    // as well as a single counter variable to assign new processes
    size_t coreId = 0;
    if(pidToCoreMap.count(pid) > 0)
    {
        coreId = pidToCoreMap.at(pid);
    }
    else
    {
        coreId = nextCoreId;

        pidToCoreMap.insert(std::pair<uint32_t, size_t>(pid, coreId));

        nextCoreId++;
        if(nextCoreId >= coreList.size())
        {
            nextCoreId = 0;
        }
    }

    return coreId;
}

void System::reset()
{
    for(Core& core : coreList)
    {
        core.reset();
    }

    for(std::shared_ptr<CacheInterface>& cache : sharedCacheList)
    {
        cache->reset();
    }

    // Reset stats
    stats = {};
}


void System::simulate(const std::vector<SimulatorInstruction>& instructionList)
{
    // All instructions are already sorted by cycle time
    for(const SimulatorInstruction& simInstruction : instructionList)
    {   
        // Use pid to get core
        // TODO: Add scheduler
        size_t coreId = getCoreIdFromPid(simInstruction.pid);
        Core& core = coreList.at(coreId);

        // Execute read or write
        // TODO: Possibly do something with read & write function outputs
        bool isWrite = ((simInstruction.instruction.info & 0x80) >> 7);
        if(isWrite)
        {
            write(core, simInstruction.instruction.address);
        }
        else
        {
            read(core, simInstruction.instruction.address);
        }
    }
}

AccessResult System::read(Core& core, Address address)
{
    // Start with core hit/miss
    AccessResult systemResult = core.read(address);

    // Check if cache miss
    if(systemResult.accessState == CACHE_MISS)
    {
        // If cache miss, then search in shared caches
        
        // For each shared cache in list
        for(std::shared_ptr<CacheInterface>& cache : sharedCacheList)
        {
            // Read from cache
            AccessResult result = cache->read(address);

            // Add cache latency
            systemResult.accessLatency += result.accessLatency;

            // Read from cache and determine if hit
            if(result.accessState == CACHE_HIT)
            {
                systemResult.accessState = CACHE_HIT;
                break;
            }
        }
    }


    // Log hit or miss
    if(systemResult.accessState == CACHE_HIT)
    {
        stats.readHits++;
    }
    else
    {
        stats.readMisses++;

        // Add memory latency
        systemResult.accessLatency += memoryLatency;
    }

    return systemResult;
}

AccessResult System::write(Core& core, Address address)
{
    // Start with core hit/miss
    AccessResult systemResult = core.write(address);

    // Check if cache miss
    if(systemResult.accessState == CACHE_MISS)
    {
        // If cache miss, then search in shared caches

        // For each shared cache in list
        for(std::shared_ptr<CacheInterface>& cache : sharedCacheList)
        {
            // Write to cache
            AccessResult result = cache->write(address);

            // Add cache latency
            systemResult.accessLatency += result.accessLatency;

            // Write to cache and determine if hit
            if(result.accessState == CACHE_HIT)
            {
                systemResult.accessState = CACHE_HIT;
                break;
            }
        }
    }


    // Log hit or miss
    if(systemResult.accessState == CACHE_HIT)
    {
        stats.writeHits++;
    }
    else
    {
        stats.writeMisses++;

        // Add memory latency
        systemResult.accessLatency += memoryLatency;
    }

    return systemResult;
}

SystemStats System::getStats() const
{
    // Save system module stats
    SystemStats systemStats = stats;

    // TODO: Possibly calculate AMAT from logging time for each access and averaging
    // Should be faster but don't have time to confirm correctness

    // Get shared cache stats
    systemStats.sharedCacheStats.reserve(sharedCacheList.size());
    double systemAMAT = static_cast<double>(memoryLatency);
    for (size_t i = sharedCacheList.size(); i --> 0 ;)
    {
        CacheStats cacheStats = sharedCacheList.at(i)->getStats();
        
        uint64_t misses = cacheStats.readMisses + cacheStats.writeMisses;
        uint64_t totalAccesses = misses + cacheStats.readHits + cacheStats.writeHits;
        double missRatio = (totalAccesses > 0) ? static_cast<double>(misses)/totalAccesses : 1.0;

        systemAMAT = (systemAMAT * missRatio) + cacheStats.latency;

        systemStats.sharedCacheStats.push_back(cacheStats);
    }

    // Get core stats
    systemStats.coreStats.reserve(coreList.size());
    double totalCoreAMAT = 0.0;
    for(const Core& core : coreList)
    {
        CoreStats coreStats = core.getStats();
        double coreAMAT = systemAMAT;
        for (size_t i = coreStats.cacheStats.size(); i --> 0 ;)
        {
            CacheStats cacheStats = coreStats.cacheStats.at(i);
            
            uint64_t misses = cacheStats.readMisses + cacheStats.writeMisses;
            uint64_t totalAccesses = misses + cacheStats.readHits + cacheStats.writeHits;
            double missRatio = (totalAccesses > 0) ? static_cast<double>(misses)/totalAccesses : 1.0;

            coreAMAT = (coreAMAT * missRatio) + cacheStats.latency;
        }

        totalCoreAMAT += coreAMAT;
        systemStats.coreStats.push_back(coreStats);
    }

    // Finish calculating AMAT
    systemStats.averageMemoryAccessTime = (coreList.size() > 0) ? (totalCoreAMAT / coreList.size()) : totalCoreAMAT;
    
    return systemStats;
}
}