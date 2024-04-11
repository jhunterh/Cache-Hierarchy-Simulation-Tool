#ifndef CHS_POLICY_INTERFACE_H
#define CHS_POLICY_INTERFACE_H

#include "types.h"
#include <memory>

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

class PolicyInterface; // forward declaration

typedef std::shared_ptr<PolicyInterface> PolicyPtr;

class PolicyInterface
{
public:
    virtual ~PolicyInterface() {};
    virtual PolicyPtr createInstance() const = 0;
    virtual void initalize(SetIndex setCount, AssociativitySize setLineSize) = 0;
    virtual void reset() = 0;
    virtual void countAccess(SetIndex setIdx, SetLineIdx entryIdx) = 0;
    virtual SetLineIdx getNextReplacementIndex(SetIndex setIdx) = 0;
};

}

}

#endif // CHS_POLICY_INTERFACE_H