#pragma once
#include <format>
#include <cstdint>
#include <iostream>
#include <compare>

//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  File:       guiddef.h
//
//  Contents:   GUID definition
//
//----------------------------------------------------------------------------

#ifndef GUID_DEFINED
#if defined(__midl)
typedef struct
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    byte           Data4[8];
} GUID;
#else
typedef struct _GUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;

constexpr std::strong_ordering
operator<=>(const GUID &lhs, const GUID &rhs)
{
    if (auto cmp = lhs.Data1 <=> rhs.Data1; cmp != 0)
    {
        return cmp;
    }
    if (auto cmp = lhs.Data2 <=> rhs.Data2; cmp != 0)
    {
        return cmp;
    }
    if (auto cmp = lhs.Data3 <=> rhs.Data3; cmp != 0)
    {
        return cmp;
    }
    for (int i = 0; i < 8; ++i)
    {
        if (auto cmp = lhs.Data4[i] <=> rhs.Data4[i]; cmp != 0)
        {
            return cmp;
        }
    }
    return std::strong_ordering::equal;
}

constexpr auto
operator==(const GUID &lhs, const GUID &rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::equal;
}

constexpr auto
operator!=(const GUID &lhs, const GUID &rhs)
{
    return !(lhs == rhs);
}

constexpr auto
operator<(const GUID &lhs, const GUID &rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::less;
}

template<uint32_t Data, uint16_t Data2, uint16_t Data3, uint8_t... Data4>
struct constexpr_guid
{
    static constexpr GUID value{Data, Data2, Data3, {Data4...}};
};

template<uint32_t Data1, uint16_t Data2, uint16_t Data3, uint8_t... Data4>
inline constexpr GUID constexpr_guid_v
    = constexpr_guid<Data1, Data2, Data3, Data4...>::value;

#if !defined(CONSTEXPR_GUID_NULL)
constexpr inline GUID CONSTEXPR_GUID_NULL = constexpr_guid_v<0x00000000,
                                                             0x0000,
                                                             0x0000,
                                                             0x00,
                                                             0x00,
                                                             0x00,
                                                             0x00,
                                                             0x00,
                                                             0x00,
                                                             0x00,
                                                             0x00>;
#endif

static_assert(CONSTEXPR_GUID_NULL == CONSTEXPR_GUID_NULL);
static_assert(CONSTEXPR_GUID_NULL <=> CONSTEXPR_GUID_NULL
              == std::strong_ordering::equal);
static_assert(constexpr_guid_v<0x00000000,
                               0x0000,
                               0x0000,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x01>
                  <=> CONSTEXPR_GUID_NULL
              == std::strong_ordering::greater);
static_assert(CONSTEXPR_GUID_NULL <=> constexpr_guid_v<0x00000000,
                                                       0x0000,
                                                       0x0000,
                                                       0x00,
                                                       0x00,
                                                       0x00,
                                                       0x00,
                                                       0x00,
                                                       0x00,
                                                       0x01>
              == std::strong_ordering::less);

template<typename CharT> struct std::formatter<GUID, CharT>
{
    template<typename FormatParseContext>
    auto
    parse(FormatParseContext &ctx) const
    {
        return ctx.begin();
    }

    template<size_t N, size_t... Is>
    auto
    format_impl(const uint8_t (&ary)[N], std::index_sequence<Is...>) const
    {
        return (std::format("{:02x}", ary[Is + 2]) + ...);
    }

    template<typename FormatContext>
    auto
    format(const GUID &guid, FormatContext &ctx) const
    {
        return std::format_to(
            ctx.out(),
            "{0}{2:08x}-{3:04x}-{4:04x}-{5:02x}{6:02x}-{7}{1}",
            '{',
            '}',
            guid.Data1,
            guid.Data2,
            guid.Data3,
            guid.Data4[0],
            guid.Data4[1],
            format_impl(guid.Data4, std::make_index_sequence<6>()));
    }
};

template<typename CharT>
std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT> &os, const GUID &guid)
{
    std::format_to(std::ostreambuf_iterator<CharT>(os.rdbuf()), "{0}", guid);
    return os;
}

#endif // !defined(__midl)

#define GUID_DEFINED 1

#endif // !defined(GUID_DEFINED)

#ifndef FAR
#define FAR
#endif

#ifndef DECLSPEC_SELECTANY
#define DECLSPEC_SELECTANY
#endif

#if defined(__cplusplus)

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)           \
    constexpr inline GUID name                                                 \
        = constexpr_guid_v<l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8>

#else
#if defined(INITGUID)
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)           \
    const GUID DECLSPEC_SELECTANY name                                         \
        = {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)           \
    extern const GUID FAR name
#endif
#endif // defined(__cplusplus)

#ifndef _GUIDDEF_H_
#define _GUIDDEF_H_

#ifndef __LPGUID_DEFINED__
#define __LPGUID_DEFINED__
typedef GUID *LPGUID;
#endif

#ifndef __LPCGUID_DEFINED__
#define __LPCGUID_DEFINED__
typedef const GUID *LPCGUID;
#endif

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef GUID IID;
typedef IID *LPIID;
#define IID_NULL                 CONSTEXPR_GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
typedef GUID   CLSID;
typedef CLSID *LPCLSID;
#define CLSID_NULL                     CONSTEXPR_GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
typedef GUID   FMTID;
typedef FMTID *LPFMTID;
#define FMTID_NULL                     CONSTEXPR_GUID_NULL
#define IsEqualFMTID(rfmtid1, rfmtid2) IsEqualGUID(rfmtid1, rfmtid2)

#ifdef __midl_proxy
#define __MIDL_CONST
#else
#define __MIDL_CONST const
#endif

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID *__MIDL_CONST
#endif
#endif

#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID *__MIDL_CONST
#endif
#endif

#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const IID &
#else
#define REFCLSID const IID *__MIDL_CONST
#endif
#endif

#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#ifdef __cplusplus
#define REFFMTID const IID &
#else
#define REFFMTID const IID *__MIDL_CONST
#endif
#endif

#endif // !__IID_DEFINED__

#if !defined(__midl)
#if !defined(_SYS_GUID_OPERATORS_)
#define _SYS_GUID_OPERATORS_
// Faster (but makes code fatter) inline version...use sparingly
#ifdef __cplusplus
__inline int
InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    // Use the already defined constexpr operator== for GUID
    return rguid1 == rguid2;
}

__inline int
IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    return rguid1 == rguid2;
}

#else // ! __cplusplus

#define InlineIsEqualGUID(rguid1, rguid2)                                      \
    (((unsigned long *)rguid1)[0] == ((unsigned long *)rguid2)[0]              \
     && ((unsigned long *)rguid1)[1] == ((unsigned long *)rguid2)[1]           \
     && ((unsigned long *)rguid1)[2] == ((unsigned long *)rguid2)[2]           \
     && ((unsigned long *)rguid1)[3] == ((unsigned long *)rguid2)[3])

#include <string.h>

#ifdef __INLINE_ISEQUAL_GUID
#define IsEqualGUID(rguid1, rguid2) InlineIsEqualGUID(rguid1, rguid2)
#else
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#endif

#endif // __cplusplus

// Same type, different name

#define IsEqualIID(riid1, riid2)       IsEqualGUID(riid1, riid2)
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)

#endif // _SYS_GUID_OPERATORS_
#endif // __midl
#endif // _GUIDDEF_H_

#if 0
/// Macro that converts a decimal value to a string of the hexidecimal value.
/// For example, STR_HEX_VALUE(42) is "2a".
/// @param x The decimal value to convert to a hexidecimal string.
/// @return The hexidecimal string.
#define STR_HEX_VALUE(x) std::format("{:x}", x)

#define GUID_STRING(l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    "{" #l "-" #w1 "-" #w2 "-" #b1 #b2 "-" #b3 #b4 #b5 #b6 #b7 #b8 "}"

STR_HEX_VALUE(0x42)

GUID_STRING(0x12345678,
            0x9012,
            0x3456,
            0x78,
            0x90,
            0x12,
            0x34,
            0x56,
            0x78,
            0x90,
            0x12);

#endif