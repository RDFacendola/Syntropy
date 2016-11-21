
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

#include "type_traits.h"

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
    
    //////////////// UTILITIES ////////////////

    /// \brief Delete an object and set its pointer to nullptr. If the pointer is already nullptr, this method does nothing.
    template <typename T>
    void SafeDelete(T*& object);

    /// \brief Delete an array and set its pointer to nullptr. If the pointer is already nullptr, this method does nothing.
    template <typename T>
    void SafeDeleteArray(T*& object);

    //////////////// STRING CONVERSION ////////////////

    /// \brief Convert a string to a wstring.
    /// \param string String to convert.
    std::wstring to_wstring(const std::string& string);

    /// \brief Converts a wstring to a string.
    /// \param string String to convert.
    std::string to_string(const std::wstring& wstring);

    /// \brief Template specialization for wstring -> string conversions.
    template <>
    struct convert<std::wstring, std::string> : std::true_type{

        std::string operator()(const std::wstring& from) {

            return to_string(from);

        }

    };

    /// \brief Template specialization for string -> wstring conversions.
    template <>
    struct convert<std::string, std::wstring> : std::true_type{

        std::wstring operator()(const std::string& from) {

            return to_wstring(from);

        }

    };

}

namespace syntropy {

    // Implementation

    //////////////// UTILITIES ////////////////

    template <typename T>
    void SafeDelete(T*& object) {

        if (object) {

            delete object;
            object = nullptr;

        }

    }

    template <typename T>
    void SafeDeleteArray(T*& object) {

        if (object) {

            delete[] object;
            object = nullptr;

        }

    }

}