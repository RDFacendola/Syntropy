
/// \file syntropy.h
/// \brief Common header for Syntropy library.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>

#define SYNTROPY_UNUSED(x) (x)

#ifdef _DEBUG
#define SYNTROPY_DEBUG_ONLY(x) x
#else
#define SYNTROPY_DEBUG_ONLY(x)
#endif

#ifdef _DEBUG
#define SYNTROPY_RELEASE_ONLY(x) 
#else
#define SYNTROPY_RELEASE_ONLY(x) x
#endif

namespace syntropy 
{

    /************************************************************************/
    /* STRING CONVERSION                                                    */
    /************************************************************************/

    /// \brief Convert a string to a wstring.
    /// \param string String to convert.
    std::wstring to_wstring(const std::string& string);

    /// \brief Converts a wstring to a string.
    /// \param string String to convert.
    std::string to_string(const std::wstring& wstring);

}