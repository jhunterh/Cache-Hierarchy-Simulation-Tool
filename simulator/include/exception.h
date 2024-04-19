#ifndef CHS_EXCEPTION_H
#define CHS_EXCEPTION_H

#include <exception>

namespace CacheHierarchySimulator
{

// TODO: Add exception message
class ChsException : public std::exception {};


class InvalidCacheSizeException : public ChsException {};
class InvalidAddressSizeException : public ChsException {};
class InvalidBlockSizeException : public ChsException {};
class InvalidAssociativitySizeException : public ChsException {};
class MismatchedAddressSpaceException : public ChsException {};
class InvalidCoreCountException : public ChsException {};

}

#endif // CHS_EXCEPTION_H