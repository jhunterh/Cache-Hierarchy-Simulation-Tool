#ifndef CHS_I_REPLACEMENT_POLICY_H
#define CHS_I_REPLACEMENT_POLICY_H

#include "types.h"
#include <memory>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class IReplacementPolicy
{
public:

    virtual PolicyPtr createPolicyInstance() const = 0;
    virtual void initalize(SetIndex setCount, AssociativitySize setLineSize) = 0;
    virtual void countAccess(SetIndex setIdx, SetLineIdx entryIdx) = 0;
    virtual SetLineIdx getNextReplacementIndex(SetIndex setIdx) = 0;
};

typedef std::unique_ptr<IReplacementPolicy> PolicyPtr;

}

}

#endif // CHS_I_REPLACEMENT_POLICY_H