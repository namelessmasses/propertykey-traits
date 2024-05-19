#pragma once

#include "GUID.hpp"
#include "concept.hpp"

#include <format>
#include <ostream>
#include <compare>

/// \brief The requirements of a runtime property key.
template<typename RuntimePropertyKeyT>
concept RuntimePropertyKeyConcept
    = requires(RuntimePropertyKeyT lhs, RuntimePropertyKeyT rhs) {
          std::same_as<decltype(lhs.fmtid), GUID>;
          std::same_as<decltype(lhs.pid), long>;
      };

template<RuntimePropertyKeyConcept RuntimePropertyKeyT>
constexpr std::strong_ordering
operator<=>(const RuntimePropertyKeyT &lhs, const RuntimePropertyKeyT &rhs)
{
    if (auto cmp = lhs.pid <=> rhs.pid; cmp != 0)
        return cmp;
    return lhs.fmtid <=> rhs.fmtid;
}

template<RuntimePropertyKeyConcept RuntimePropertyKeyT>
constexpr auto
operator==(const RuntimePropertyKeyT &lhs, const RuntimePropertyKeyT &rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::equal;
}

template<RuntimePropertyKeyConcept RuntimePropertyKeyT>
constexpr auto
operator!=(const RuntimePropertyKeyT &lhs, const RuntimePropertyKeyT &rhs)
{
    return !(lhs == rhs);
}

template<RuntimePropertyKeyConcept RuntimePropertyKeyT>
constexpr auto
operator<(const RuntimePropertyKeyT &lhs, const RuntimePropertyKeyT &rhs)
{
    return (lhs <=> rhs) == std::strong_ordering::less;
}

template<RuntimePropertyKeyConcept RuntimePropertyKeyT, IsCharOrWideChar CharT>
struct std::formatter<RuntimePropertyKeyT, CharT>
{
    template<typename FormatParseContext>
    constexpr auto
    parse(FormatParseContext &ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto
    format(const RuntimePropertyKeyT &rtp, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(),
                              "{}{},{}{}",
                              '(',
                              rtp.fmtid,
                              rtp.pid,
                              ')');
    }
};

template<RuntimePropertyKeyConcept RuntimePropertyKeyT, IsCharOrWideChar CharT>
std::basic_ostream<CharT> &
operator<<(std::basic_ostream<CharT> &os, const RuntimePropertyKeyT &rtp)
{
    std::format_to(std::ostreambuf_iterator<CharT>(os.rdbuf()), "{0}", rtp);
    return os;
}
