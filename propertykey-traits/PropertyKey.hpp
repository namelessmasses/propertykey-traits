#pragma once

#include "CompileTimePropertyKey.hpp"
#include "RuntimePropertyKey.hpp"
#include "GUID.hpp"
#include "concept.hpp"

#include <string_view>
#include <source_location>
#include <stdexcept>
#include <compare>
namespace details
{

struct IRuntimePropertyKey
{
    constexpr IRuntimePropertyKey() {}

    virtual ~IRuntimePropertyKey() = default;

    virtual GUID const &
    fmtid_rt() const
        = 0;

    virtual long
    pid_rt() const
        = 0;

    bool
    operator<(IRuntimePropertyKey const &rhs) const
    {
        if (auto cmp = pid_rt() <=> rhs.pid_rt();
            cmp == std::strong_ordering::less)
            return true;

        return fmtid_rt() < rhs.fmtid_rt();
    }
};

struct IRuntimePropertyKeyTraits: IRuntimePropertyKey
{
    virtual std::string_view const &
    nameA_rt() const
        = 0;

    virtual std::wstring_view const &
    nameW_rt() const
        = 0;

    virtual uint64_t
    metatype_rt(std::source_location const sc
                = std::source_location::current()) const
        = 0;

    virtual std::string_view const &
    metatype_nameA_rt(std::source_location const sc
                      = std::source_location::current()) const
        = 0;

    virtual std::wstring_view const &
    metatype_nameW_rt(std::source_location const sc
                      = std::source_location::current()) const
        = 0;
};

template<RuntimePropertyKeyConcept RuntimePropertyT>
struct CRuntimePropertyKeyShim: IRuntimePropertyKey
{
    RuntimePropertyT const &key;

    constexpr CRuntimePropertyKeyShim(RuntimePropertyT const &key)
        : key{key}
    {
    }

    virtual GUID const &
    fmtid_rt() const override
    {
        return key.fmtid;
    }

    virtual long
    pid_rt() const override
    {
        return key.pid;
    }
};

void
AddPropertyKey(IRuntimePropertyKeyTraits const &key);

IRuntimePropertyKeyTraits const *
GetPropertyKey(IRuntimePropertyKey const &);

template<RuntimePropertyKeyConcept RuntimePropertyT>
struct RuntimePropertyKeyShim: IRuntimePropertyKey
{
    RuntimePropertyKeyShim(RuntimePropertyT const &key)
        : key{key}
    {
    }

    virtual GUID const &
    fmtid_rt() const override
    {
        return key.fmtid;
    }

    virtual long
    pid_rt() const override
    {
        return key.pid;
    }

    RuntimePropertyT const &key;
};

} // namespace details

template<RuntimePropertyKeyConcept RuntimePropertyKeyT>
details::IRuntimePropertyKeyTraits const *
GetPropertyKey(RuntimePropertyKeyT const &key)
{
    details::RuntimePropertyKeyShim<RuntimePropertyKeyT> shim{key};
    return details::GetPropertyKey(shim);
}

template<RuntimePropertyKeyConcept RuntimePropertyKeyT,
         GUID                      fmtid_v,
         long                      pid_v>
struct PropertyKey
    : CompileTimePropertyKey<fmtid_v, pid_v>
    , RuntimePropertyKeyT
    , details::IRuntimePropertyKeyTraits
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

    virtual GUID const &
    fmtid_rt() const override
    {
        return fmtid_value;
    }

    virtual long
    pid_rt() const override
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
        constexpr std::string_view const &                                     \
        nameA() const                                                          \
        {                                                                      \
            return nameA_value;                                                \
        }                                                                      \
                                                                               \
        constexpr std::wstring_view const &                                    \
        nameW() const                                                          \
        {                                                                      \
            return nameW_value;                                                \
        }                                                                      \
                                                                               \
        virtual std::string_view const &                                       \
        nameA_rt() const override                                              \
        {                                                                      \
            return nameA_value;                                                \
        }                                                                      \
                                                                               \
        virtual std::wstring_view const &                                      \
        nameW_rt() const override                                              \
        {                                                                      \
            return nameW_value;                                                \
        }                                                                      \
                                                                               \
        virtual uint64_t                                                       \
        metatype_rt(std::source_location const sc) const override              \
        {                                                                      \
            throw std::runtime_error(                                          \
                std::format("{}:{}:{} - {} metatype not available for {}.",    \
                            sc.file_name(),                                    \
                            sc.line(),                                         \
                            sc.column(),                                       \
                            sc.function_name(),                                \
                            nameA_value));                                     \
        }                                                                      \
                                                                               \
        virtual std::string_view const &                                       \
        metatype_nameA_rt(std::source_location const sc) const override        \
        {                                                                      \
            throw std::runtime_error(                                          \
                std::format("{}:{}:{} - {} metatype not available for {}.",    \
                            sc.file_name(),                                    \
                            sc.line(),                                         \
                            sc.column(),                                       \
                            sc.function_name(),                                \
                            nameA_value));                                     \
        }                                                                      \
                                                                               \
        virtual std::wstring_view const &                                      \
        metatype_nameW_rt(std::source_location const sc) const override        \
        {                                                                      \
            throw std::runtime_error(                                          \
                std::format("{}:{}:{} - {} metatype not available for {}.",    \
                            sc.file_name(),                                    \
                            sc.line(),                                         \
                            sc.column(),                                       \
                            sc.function_name(),                                \
                            nameA_value));                                     \
        }                                                                      \
    };                                                                         \
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
    }                                                                          \
    inline constexpr name##_PropertyKey name
