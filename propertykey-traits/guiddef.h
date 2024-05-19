#if !defined(_HAS_CXX20) || !_HAS_CXX20

// Force CPP to search for the 'next' guiddef.h file in the include path
// This is a workaround for the fact that the C++20 standard does not have a
// 'strong_ordering' type.
#include_next <guiddef.h>

#else

#pragma once
#include "GUID.hpp"

#endif