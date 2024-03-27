#include "lastinfirstout.h"

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

LastInFirstOut::LastInFirstOut(EntryIdx entryCount, AssociativitySize setLineSize) 
    : setLineSize(setLineSize)
{
    nextIndexList.resize(entryCount);
}

void LastInFirstOut::countAccess(Index setIdx, SetLineIdx entryIdx)
{

}

SetLineIdx LastInFirstOut::getNextReplacementIndex(Index setIdx)
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