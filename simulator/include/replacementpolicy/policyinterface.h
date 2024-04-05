#ifndef CHS_POLICY_INTERFACE_H
#define CHS_POLICY_INTERFACE_H

#include "types.h"
#include <memory>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class PolicyInterface; // forward declaration

typedef std::unique_ptr<PolicyInterface> PolicyPtr;

class PolicyInterface
{
public:

    virtual PolicyPtr createInstance() const = 0;
    virtual void initalize(SetIndex setCount, AssociativitySize setLineSize) = 0;
    virtual void reset();
    virtual void countAccess(SetIndex setIdx, SetLineIdx entryIdx) = 0;
    virtual SetLineIdx getNextReplacementIndex(SetIndex setIdx) = 0;
};

}

}

#endif // CHS_POLICY_INTERFACE_H