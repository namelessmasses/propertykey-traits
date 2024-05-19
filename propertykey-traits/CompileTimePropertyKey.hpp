#pragma once

#include "GUID.hpp"

#define HAS_CONCEPT_IS_CHAR_OR_WIDE_CHAR 1
#include "concept.hpp"

#include <type_traits>
#include <format>
#include <cstdint>
#include <ostream>

#define HAS_CONCEPT_COMPILE_TIME_PROPERTY_KEY 1
#if HAS_CONCEPT_COMPILE_TIME_PROPERTY_KEY
template<typename GuidT,
         typename PidT,
         GuidT g,
         PidT  p,
         template<GuidT fmtid_arg, PidT pid_arg>
         class CompileTimePropertyKeyT>
concept CompileTimePropertyKeyConcept = requires {
    std::same_as<decltype(CompileTimePropertyKeyT<g, p>::fmtid_value), GuidT>;

    std::same_as<decltype(CompileTimePropertyKeyT<g, p>::pid_value), PidT>;

    std::same_as<
        std::bool_constant<CompileTimePropertyKeyT<g, p>::fmtid_value == g>,
        std::true_type>;

    std::same_as<
        std::bool_constant<CompileTimePropertyKeyT<g, p>::pid_value == p>,
        std::true_type>;
};
#else
#define ComplieTimePropertyKeyConcept typename
#endif

template<GUID fmtid, long pid> struct CompileTimePropertyKey
{
    static constexpr GUID fmtid_value = fmtid;
    static constexpr long pid_value   = pid;

    constexpr CompileTimePropertyKey() {}
};

static_assert(
    CompileTimePropertyKey<CONSTEXPR_GUID_NULL, 0x00000000>::fmtid_value
    == CONSTEXPR_GUID_NULL);
static_assert(CompileTimePropertyKey<CONSTEXPR_GUID_NULL, 0x00000000>::pid_value
              == 0x00000000);

template<GUID g,
         long p,
         template<GUID, long>
         class CompileTimePropertyKeyT,
         IsCharOrWideChar CharT>
requires CompileTimePropertyKeyConcept<GUID,
                                       long,
                                       g,
                                       p,
                                       CompileTimePropertyKeyT>
struct std::formatter<CompileTimePropertyKeyT<g, p>, CharT>
{

    template<typename FormatParseContext>
    constexpr auto
    parse(FormatParseContext &ctx) const
    {
        return ctx.begin();
    }

    using compile_time_type = CompileTimePropertyKey<g, p>;

    template<typename FormatContext>
    constexpr auto
    format(const compile_time_type &ctp, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(),
                              "({},{})",
                              compile_time_type::fmtid_value,
                              compile_time_type::pid_value);
    }
};

template<IsCharOrWideChar CharT,
         GUID             g,
         long             p,
         template<GUID, long>
         class CompileTimePropertyKeyT>
requires CompileTimePropertyKeyConcept<GUID,
                                       long,
                                       g,
                                       p,
                                       CompileTimePropertyKeyT>
constexpr std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT>           &os,
           const CompileTimePropertyKeyT<g, p> &ctp)
{
    std::format_to(std::ostreambuf_iterator<CharT>(os.rdbuf()), "{0}", ctp);
    return os;
}