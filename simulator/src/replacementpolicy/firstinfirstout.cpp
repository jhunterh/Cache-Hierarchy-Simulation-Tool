#include "firstinfirstout.h"

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

PolicyPtr FirstInFirstOut::createInstance() const
{
    return PolicyPtr(new FirstInFirstOut());
}

void FirstInFirstOut::initalize(SetIndex setCount, AssociativitySize setLineSize)
{
    this->setLineSize = setLineSize;
    nextIndexList.resize(setCount);
}

void FirstInFirstOut::reset()
{
    size_t setCount = nextIndexList.size();
    nextIndexList.clear();
    nextIndexList.resize(setCount);
}

void FirstInFirstOut::countAccess(SetIndex setIdx, SetLineIdx entryIdx)
{

}

SetLineIdx FirstInFirstOut::getNextReplacementIndex(SetIndex setIdx)
{
    SetLineIdx& lineIdx = nextIndexList.at(setIdx);
    SetLineIdx retval = lineIdx;

    lineIdx++;
    if(lineIdx >= setLineSize)
        lineIdx = 0;

    return retval;
}


}

}