#ifndef CHS_POLICY_LIST_H
#define CHS_POLICY_LIST_H

#include "firstinfirstout.h"
#include "lastinfirstout.h" 
namespace CacheHierarchySimulator
{

namespace ReplacementPolicy
{

static const FirstInFirstOut FIFO;
static const LastInFirstOut LIFO;

}

}


#endif // CHS_POLICY_LIST_H