#pragma once

#include "PropertyKey.hpp"
#include "RuntimePropertyKey.hpp"

#include <concepts>
#include <type_traits>
#include <string_view>

template<typename PropertyKeyTraitsT>
concept PropertyKeyTraitsConcept = requires {
    typename PropertyKeyTraitsT::runtime_type;
    RuntimePropertyKeyConcept<typename PropertyKeyTraitsT::runtime_type>;

    typename PropertyKeyTraitsT::compiletime_type;

    std::same_as<decltype(PropertyKeyTraitsT::fmtid_value), GUID>;
    std::same_as<decltype(PropertyKeyTraitsT::pid_value), long>;
};

template<PropertyKeyTraitsConcept PropertyKeyTraitsT,
         typename MetaTypeT,
         MetaTypeT MetaTypeValue>
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
        using base_type                                                        \
            = PropertyKeyWithMetatype<decltype(KeyName),                       \
                                      std::decay_t<decltype(MetaTypeValue)>,   \
                                      MetaTypeValue>;                          \
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
    } KeyName##_With_Metatype