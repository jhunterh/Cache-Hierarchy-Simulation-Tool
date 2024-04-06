#include "system.h"

namespace CacheHierarchySimulator
{

System::System(AddressSize addressSpace, CycleTime memoryLatency) : addressSpace(addressSpace), memoryLatency(memoryLatency)
{

}

System::~System()
{
    coreList.clear();
    sharedCacheList.clear();
}

void System::addCore(const Core& core)
{
    coreList.push_back(core);
}

void System::addSharedCache(const CacheInterface& cache)
{
    sharedCacheList.push_back(cache.createInstance());
}

AccessResult System::read(Address address)
{
    // TODO: Implement read
    return AccessResult();
}

AccessResult System::write(Address address)
{
    // TODO: Implement write
    return AccessResult();
}

SystemStats System::getStats()
{
    // TODO: Implement stats
    SystemStats retVal;
    return retVal;
}
}