
/// \file syntropy.h
/// \brief Common header for Syntropy library.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <cassert>

#include <string>
#include <locale>
#include <codecvt>

#define SYN_UNUSED(x) (x)

#ifdef _MSC_VER
#define SYN_NO_INLINE __declspec(noinline)
#else
#define SYN_NO_INLINE 
#endif

#ifdef _DEBUG
#define SYN_DEBUG_ONLY(x) x
#else
#define SYN_DEBUG_ONLY(x)
#endif

#ifdef _DEBUG
#define SYN_RELEASE_ONLY(x) 
#else
#define SYN_RELEASE_ONLY(x) x
#endif

namespace syntropy {

    //////////////// STRING CONVERSION ////////////////

    /// \brief Convert a string to a wstring.
    /// \param string String to convert.
    std::wstring to_wstring(const std::string& string);

    /// \brief Converts a wstring to a string.
    /// \param string String to convert.
    std::string to_string(const std::wstring& wstring);

}