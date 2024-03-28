#include <cache.h>

namespace CacheHierarchySimulator
{

Latency Cache::getLatency()
{
    return latency;
}

WritePolicy Cache::getWritePolicy()
{
    return writePolicy;
}

ModuleStats Cache::getStats()
{
    return stats;
}

}