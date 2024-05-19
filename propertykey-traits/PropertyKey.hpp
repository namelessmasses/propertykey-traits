#pragma once

#include "CompileTimePropertyKey.hpp"
#include "RuntimePropertyKey.hpp"
#include "GUID.hpp"
#include "concept.hpp"

#include <string_view>

template<RuntimePropertyKeyConcept RuntimePropertyKeyT,
         GUID                      fmtid_v,
         long                      pid_v>
struct PropertyKey
    : CompileTimePropertyKey<fmtid_v, pid_v>
    , RuntimePropertyKeyT
{
    using compiletime_type = CompileTimePropertyKey<fmtid_v, pid_v>;
    using compiletime_type::fmtid_value;
    using compiletime_type::pid_value;

    using runtime_type = RuntimePropertyKeyT;

    constexpr GUID const &
    fmtid() const
    {
        return fmtid_value;
    }

    constexpr long
    pid() const
    {
        return pid_value;
    }

    constexpr PropertyKey()
        : CompileTimePropertyKey<fmtid_v, pid_v>{}
        , RuntimePropertyKeyT{fmtid_value, pid_value}
    {
    }
};

template<IsCharOrWideChar          CharT,
         RuntimePropertyKeyConcept RuntimePropertyKeyT,
         GUID                      fmtid,
         long                      pid>
struct std::formatter<PropertyKey<RuntimePropertyKeyT, fmtid, pid>, CharT>
{
    template<typename FormatParseContext>
    constexpr auto
    parse(FormatParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    constexpr auto
    format(const PropertyKey<RuntimePropertyKeyT, fmtid, pid> &,
           FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "({},{})", fmtid, pid);
    }
};

template<IsCharOrWideChar          CharT,
         RuntimePropertyKeyConcept RuntimePropertyKeyT,
         GUID                      fmtid,
         long                      pid>
constexpr std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT>                          &os,
           const PropertyKey<RuntimePropertyKeyT, fmtid, pid> &pk)
{
    std::ostreambuf_iterator<CharT> out(os.rdbuf());
    std::format_to(out, "{}", pk);
    return os;
}

#define PROPERTYKEY_DEFINE(runtimekeytype, name, guid, pid)                    \
    struct name##_PropertyKey: PropertyKey<runtimekeytype, guid, pid>          \
    {                                                                          \
        using base_type = PropertyKey<runtimekeytype, guid, pid>;              \
                                                                               \
        constexpr name##_PropertyKey()                                         \
            : PropertyKey<runtimekeytype, guid, pid>()                         \
        {                                                                      \
        }                                                                      \
                                                                               \
        static constexpr std::string_view  nameA_value = #name;                \
        static constexpr std::wstring_view nameW_value = L#name;               \
                                                                               \
        constexpr std::string_view                                             \
        nameA() const                                                          \
        {                                                                      \
            return nameA_value;                                                \
        }                                                                      \
                                                                               \
        constexpr std::wstring_view                                            \
        nameW() const                                                          \
        {                                                                      \
            return nameW_value;                                                \
        }                                                                      \
    } name;                                                                    \
                                                                               \
    template<IsCharOrWideChar CharT>                                           \
    struct std::formatter<name##_PropertyKey, CharT>                           \
    {                                                                          \
        template<typename FormatParseContext>                                  \
        constexpr auto                                                         \
        parse(FormatParseContext &ctx)                                         \
        {                                                                      \
            return ctx.begin();                                                \
        }                                                                      \
                                                                               \
        template<typename FormatContext>                                       \
        constexpr auto                                                         \
        format(const name##_PropertyKey &pk, FormatContext &ctx) const         \
        {                                                                      \
            return std::format_to(                                             \
                ctx.out(),                                                     \
                "{} {}",                                                       \
                #name,                                                         \
                static_cast<typename name##_PropertyKey ::base_type const &>(  \
                    pk));                                                      \
        }                                                                      \
    };                                                                         \
                                                                               \
    template<IsCharOrWideChar CharT>                                           \
    constexpr std::basic_ostream<CharT> &operator<<(                           \
        std::basic_ostream<CharT> &os,                                         \
        const name##_PropertyKey  &pk)                                         \
    {                                                                          \
        std::format_to(std::ostreambuf_iterator<CharT>(os.rdbuf()),            \
                       "{0}",                                                  \
                       pk);                                                    \
        return os;                                                             \
    }
