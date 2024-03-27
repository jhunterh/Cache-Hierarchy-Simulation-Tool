#ifndef CHS_REPLACEMENT_POLICY_H
#define CHS_REPLACEMENT_POLICY_H

#include "types.h"

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class IReplacementPolicy
{
public:

    virtual void countAccess(Index setIdx, SetLineIdx entryIdx) = 0;
    virtual SetLineIdx getNextReplacementIndex(Index setIdx) = 0;
};

class LastInFirstOut : public IReplacementPolicy {};
class LeastRecentlyUsed : public IReplacementPolicy {};
class PseudoLeastRecentlyUsed : public IReplacementPolicy {};
class MostRecentlyUsed : public IReplacementPolicy {};
class LeastFrequentlyUsed : public IReplacementPolicy {};

}

}

#endif // CHS_REPLACEMENT_POLICY_H