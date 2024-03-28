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

void System::addSystemCache(const ICache& cache)
{
    systemCacheList.push_back(cache.createInstance());
}

Latency System::read(Address address)
{
    // TODO: Implement read
}

Latency System::write(Address address)
{
    // TODO: Implement write
}

SystemStats System::getStats()
{
    // TODO: Implement stats
}
}