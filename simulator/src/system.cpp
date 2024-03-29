#include "system.h"

namespace CacheHierarchySimulator
{

System::System(AddressSize addressSpace, Latency memoryLatency) : addressSpace(addressSpace), memoryLatency(memoryLatency)
{

}

System::~System()
{
    coreList.clear();
    systemCacheList.clear();
}

void System::addCore(const Core& core)
{
    coreList.push_back(core);
}

void System::addSystemCache(const CacheInterface& cache)
{
    systemCacheList.push_back(cache.createInstance());
}

Latency System::read(Address address)
{
    // TODO: Implement read
    return 0;
}

Latency System::write(Address address)
{
    // TODO: Implement write
    return 0;
}

SystemStats System::getStats()
{
    // TODO: Implement stats
    SystemStats retVal;
    return retVal;
}
}