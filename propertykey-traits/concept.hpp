#pragma once

#if HAS_CONCEPT_IS_CHAR_OR_WIDE_CHAR

#include <concepts>
#include <type_traits>

template <typename CharT>
concept IsChar = std::same_as<CharT, char>;

template <typename CharT>
concept IsWideChar = std::same_as<CharT, wchar_t>;

template <typename CharT>
concept IsCharOrWideChar = IsChar<CharT> || IsWideChar<CharT>;

#else

#define IsChar typename
#define IsWideChar typename
#define IsCharOrWideChar typename

#endif