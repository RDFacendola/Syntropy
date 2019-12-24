
/// \file string.h
/// \brief This header is part of the syntropy containers. It contains utility methods for strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <string_view>

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether lhs is a prefix of rhs.
    /// \return Returns true if lhs is a prefix of rhs, returns false otherwise.
    bool IsPrefix(const std::string_view& lhs, const std::string_view& rhs);

    /// \brief Check whether lhs is a suffix of rhs.
    /// \return Returns true if lhs is a suffix of rhs, returns false otherwise.
    bool IsSuffix(const std::string_view& lhs, const std::string_view& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    inline bool IsPrefix(const std::string_view& lhs, const std::string_view& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(0, lhs.length(), lhs) == 0);
    }

    inline bool IsSuffix(const std::string_view& lhs, const std::string_view& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(rhs.length() - lhs.length(), lhs.length(), lhs) == 0);
    }
}
