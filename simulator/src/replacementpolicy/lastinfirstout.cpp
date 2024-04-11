#include "lastinfirstout.h"

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{
    
PolicyPtr LastInFirstOut::createInstance() const
{
    return PolicyPtr(new LastInFirstOut());
}

void LastInFirstOut::initalize(SetIndex setCount, AssociativitySize setLineSize)
{
    this->setLineSize = setLineSize;
    nextIndexList.resize(setCount);
}

void LastInFirstOut::reset()
{
    size_t setCount = nextIndexList.size();
    nextIndexList.clear();
    nextIndexList.resize(setCount);
}

void LastInFirstOut::countAccess(SetIndex setIdx, SetLineIdx entryIdx)
{

}

SetLineIdx LastInFirstOut::getNextReplacementIndex(SetIndex setIdx)
{
    SetLineIdx& lineIdx = nextIndexList.at(setIdx);
    SetLineIdx retval = lineIdx;

    lineIdx++;
    if(lineIdx >= setLineSize)
        lineIdx = setLineSize - 1;

    return retval;
}


}

}