// Attributed to: str_const from Scott Schurr - 2012.cppnow.org -> schurr_cpp11_tools_for_class_authors.pdf
#pragma once
#include <cstddef>
#include <stdexcept>

class str_const
{
private:
    const char * const p_;
    const std::size_t  sz_;

public:
    // constexpr string
    template<std::size_t N>
    constexpr str_const(const char (&a)[N])
        : p_(a)
        , sz_(N - 1)
    {
    }

    constexpr char
    operator[](std::size_t n)
    {
        return n < sz_ ? p_[n] : throw std::out_of_range("");
    }

    constexpr std::size_t
    size()
    {
        return sz_;
    }

    constexpr const char *
    data() const
    {
        return p_;
    }
};
