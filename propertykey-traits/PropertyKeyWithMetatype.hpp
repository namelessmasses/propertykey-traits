#pragma once

#include "PropertyKey.hpp"
#include "RuntimePropertyKey.hpp"
#include "concept.hpp"

#include <concepts>
#include <type_traits>
#include <string_view>
#include <format>

template<typename PropertyKeyTraitsT>
concept PropertyKeyTraitsConcept = requires {
    typename PropertyKeyTraitsT::runtime_type;
    RuntimePropertyKeyConcept<typename PropertyKeyTraitsT::runtime_type>;

    typename PropertyKeyTraitsT::compiletime_type;

    std::same_as<decltype(PropertyKeyTraitsT::fmtid_value), GUID>;
    std::same_as<decltype(PropertyKeyTraitsT::pid_value), long>;
#if _HAS_CXX23
    std::formattable<PropertyKeyTraitsT, char>;
    std::formattable<PropertyKeyTraitsT, wchar_t>;
#else
    std::_Formattable_with<PropertyKeyTraitsT,
                           std::format_context,
                           std::formatter<PropertyKeyTraitsT, char>>;
    std::_Formattable_with<PropertyKeyTraitsT,
                           std::wformat_context,
                           std::formatter<PropertyKeyTraitsT, wchar_t>>;
#endif
};

template<typename MetaTypeT>
concept IsFormattable = requires {
#if _HAS_CXX23
    std::formattable<MetaTypeT, char>;
    std::formattable<MetaTypeT, wchar_t>;
#else
    std::_Formattable_with<MetaTypeT,
                           std::format_context,
                           std::formatter<MetaTypeT, char>>;
    std::_Formattable_with<MetaTypeT,
                           std::wformat_context,
                           std::formatter<MetaTypeT, wchar_t>>;
#endif
};

template<typename EnumT>
concept IsNotFormattable = requires {
#if _HAS_CXX23
    requires std::negation_v<std::formattable<EnumT, char>>;
    requires std::negation_v<std::formattable<EnumT, wchar_t>>;
#else
    !std::_Formattable_with<EnumT,
                            std::format_context,
                            std::formatter<EnumT, char>>;

    !std::_Formattable_with<EnumT,
                            std::wformat_context,
                            std::formatter<EnumT, wchar_t>>;
#endif
};

/// \brief std::formatter for enum VARENUM
///
/// \details enum VARENUM is heavily utilized in Windows properties. Provide a
/// specialized std::formatter to allow formatting of common Windows property
/// keys when they've been wrapped with a value metatype.
template<typename CharT>
// requires IsNotFormattable<enum VARENUM>
struct std::formatter<enum VARENUM, CharT>
    : std::formatter<std::underlying_type_t<enum VARENUM>, CharT>
{
    template<typename FormatContext>
    auto
    format(enum VARENUM ve, FormatContext &ctx) const
    {
        return std::formatter<std::underlying_type_t<enum VARENUM>, CharT>::
            format(static_cast<std::underlying_type_t<enum VARENUM>>(ve), ctx);
    }
};

template<PropertyKeyTraitsConcept PropertyKeyTraitsT,
         IsFormattable            MetaTypeT,
         MetaTypeT                MetaTypeValue>
struct PropertyKeyWithMetatype: PropertyKeyTraitsT
{
    using base_type                               = PropertyKeyTraitsT;

    using metatype_type                           = MetaTypeT;

    static constexpr metatype_type metatype_value = MetaTypeValue;

    constexpr PropertyKeyWithMetatype()
        : base_type{}
    {
    }
};

#define PROPERTYKEY_ADD_METATYPE(KeyName, MetaTypeValue, ...)                  \
    struct KeyName##_PropertyKey_With_Metatype                                 \
        : PropertyKeyWithMetatype<decltype(KeyName),                           \
                                  std::decay_t<decltype(MetaTypeValue)>,       \
                                  MetaTypeValue>                               \
    {                                                                          \
        using base_type = typename PropertyKeyWithMetatype<                    \
            decltype(KeyName),                                                 \
            std::decay_t<decltype(MetaTypeValue)>,                             \
            MetaTypeValue>::base_type;                                         \
                                                                               \
        using metatype_type = std::decay_t<decltype(MetaTypeValue)>;           \
                                                                               \
        constexpr KeyName##_PropertyKey_With_Metatype()                        \
            : PropertyKeyWithMetatype<decltype(KeyName),                       \
                                      std::decay_t<decltype(MetaTypeValue)>,   \
                                      MetaTypeValue>{}                         \
        {                                                                      \
        }                                                                      \
                                                                               \
        static constexpr metatype_type    metatype_value = (MetaTypeValue);    \
                                                                               \
        static constexpr std::string_view metatype_nameA_value                 \
            = #MetaTypeValue;                                                  \
                                                                               \
        static constexpr std::wstring_view metatype_nameW_value                \
            = L#MetaTypeValue;                                                 \
                                                                               \
        constexpr metatype_type const &                                        \
        metatype() const                                                       \
        {                                                                      \
            return metatype_value;                                             \
        }                                                                      \
                                                                               \
        constexpr std::string_view                                             \
        metatype_nameA() const                                                 \
        {                                                                      \
            return metatype_nameA_value;                                       \
        }                                                                      \
                                                                               \
        constexpr std::wstring_view                                            \
        metatype_nameW() const                                                 \
        {                                                                      \
            return metatype_nameW_value;                                       \
        }                                                                      \
    } KeyName##_With_Metatype;                                                 \
                                                                               \
    template<IsCharOrWideChar CharT>                                           \
    struct std::formatter<KeyName##_PropertyKey_With_Metatype, CharT>          \
    {                                                                          \
        using format_type = KeyName##_PropertyKey_With_Metatype;               \
                                                                               \
        using base_type   = typename format_type::base_type;                   \
                                                                               \
        template<typename FormatParseContext>                                  \
        auto                                                                   \
        parse(FormatParseContext &ctx) const                                   \
        {                                                                      \
            return ctx.begin();                                                \
        }                                                                      \
                                                                               \
        template<typename FormatContext>                                       \
        auto                                                                   \
        format(const KeyName##_PropertyKey_With_Metatype &key,                 \
               FormatContext                             &context) const                                   \
        {                                                                      \
            return std::format_to(context.out(),                               \
                                  "{} ({}/{})",                                \
                                  static_cast<base_type const &>(key),         \
                                  format_type::metatype_nameA_value,           \
                                  format_type::metatype_value);                \
        }                                                                      \
    };                                                                         \
    template<IsCharOrWideChar CharT>                                           \
    std::basic_ostream<CharT> &operator<<(                                     \
        std::basic_ostream<CharT>                 &os,                         \
        const KeyName##_PropertyKey_With_Metatype &key)                        \
    {                                                                          \
        std::ostreambuf_iterator<CharT> osi(os.rdbuf());                       \
        std::format_to(osi, "{}", key);                                        \
        return os;                                                             \
    }