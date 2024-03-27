#ifndef CHS_LAST_IN_FIRST_OUT_H
#define CHS_LAST_IN_FIRST_OUT_H

#include "replacementpolicy.h"
#include <vector>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class LastInFirstOut : public IReplacementPolicy
{
public:

    LastInFirstOut(EntryIdx entryCount, AssociativitySize setLineSize);
    void countAccess(Index setIdx, SetLineIdx entryIdx);
    SetLineIdx getNextReplacementIndex(Index setIdx);

private:
    AssociativitySize setLineSize;
    std::vector<SetLineIdx> nextIndexList;
};

}

}

#endif // CHS_LAST_IN_FIRST_OUT_H