#ifndef CHS_LAST_IN_FIRST_OUT_H
#define CHS_LAST_IN_FIRST_OUT_H

#include "ireplacementpolicy.h"
#include <vector>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class LastInFirstOut : public IReplacementPolicy
{
public:

    PolicyPtr createPolicyInstance() const;
    void initalize(SetIndex setCount, AssociativitySize setLineSize);
    void countAccess(SetIndex setIdx, SetLineIdx entryIdx);
    SetLineIdx getNextReplacementIndex(SetIndex setIdx);

private:
    AssociativitySize setLineSize;
    std::vector<SetLineIdx> nextIndexList;
};

}

}

#endif // CHS_LAST_IN_FIRST_OUT_H