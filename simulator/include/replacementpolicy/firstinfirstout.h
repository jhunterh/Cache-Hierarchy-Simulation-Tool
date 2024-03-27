#ifndef CHS_FIRST_IN_FIRST_OUT_H
#define CHS_FIRST_IN_FIRST_OUT_H

#include "replacementpolicy.h"
#include <vector>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class FirstInFirstOut : public IReplacementPolicy
{
public:

    FirstInFirstOut(EntryIdx entryCount, AssociativitySize setLineSize);
    void countAccess(Index setIdx, SetLineIdx entryIdx);
    SetLineIdx getNextReplacementIndex(Index setIdx);

private:
    AssociativitySize setLineSize;
    std::vector<SetLineIdx> nextIndexList;
};

}

}

#endif // CHS_FIRST_IN_FIRST_OUT_H