#ifndef CHS_FIRST_IN_FIRST_OUT_H
#define CHS_FIRST_IN_FIRST_OUT_H

#include "policyinterface.h"
#include <vector>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class FirstInFirstOut : public PolicyInterface
{
public:
    FirstInFirstOut() {}
    ~FirstInFirstOut();
    PolicyPtr createInstance() const;
    void initalize(SetIndex setCount, AssociativitySize setLineSize);
    void reset();
    void countAccess(SetIndex setIdx, SetLineIdx entryIdx);
    SetLineIdx getNextReplacementIndex(SetIndex setIdx);

private:

    AssociativitySize setLineSize;
    std::vector<SetLineIdx> nextIndexList;
};

}

}

#endif // CHS_FIRST_IN_FIRST_OUT_H