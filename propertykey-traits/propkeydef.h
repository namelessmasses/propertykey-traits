#if !defined(_HAS_CXX20) || _HAS_CXX20 == 0

// Force CPP to search for the 'next' guiddef.h file in the include path
// This is a workaround for the fact that the C++20 standard does not have a
// 'strong_ordering' type.
#include_next <propkeydef.h>

#else

#pragma once

#include "PropertyKey.hpp"

#include <wtypes.h>

static_assert(RuntimePropertyKeyConcept<PROPERTYKEY>);

#define DEFINE_PROPERTYKEY(name,                                               \
                           Data1,                                              \
                           Data2,                                              \
                           Data3,                                              \
                           Data41,                                             \
                           Data42,                                             \
                           Data43,                                             \
                           Data44,                                             \
                           Data45,                                             \
                           Data46,                                             \
                           Data47,                                             \
                           Data48,                                             \
                           pid)                                                \
    DEFINE_PROPERTYKEY_(PROPERTYKEY,                                           \
                        name,                                                  \
                        (constexpr_guid_v<Data1,                               \
                                          Data2,                               \
                                          Data3,                               \
                                          Data41,                              \
                                          Data42,                              \
                                          Data43,                              \
                                          Data44,                              \
                                          Data45,                              \
                                          Data46,                              \
                                          Data47,                              \
                                          Data48>),                            \
                        pid)

#endif // _HAS_CXX20