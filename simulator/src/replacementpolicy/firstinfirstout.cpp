#include "firstinfirstout.h"

namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

FirstInFirstOut::FirstInFirstOut(EntryIdx entryCount, AssociativitySize setLineSize) 
    : setLineSize(setLineSize)
{
    nextIndexList.resize(entryCount);
}

void FirstInFirstOut::countAccess(Index setIdx, SetLineIdx entryIdx)
{

}

SetLineIdx FirstInFirstOut::getNextReplacementIndex(Index setIdx)
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