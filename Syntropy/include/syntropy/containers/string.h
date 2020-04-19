
/// \file string.h
/// \brief This header is part of the syntropy containers. It contains utility methods for strings.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <string_view>

#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    /// \brief Alias type for std::string with polymorphic allocator type.
    using String = std::basic_string<char, std::char_traits<char>, PolymorphicAllocator<char>>;

    /************************************************************************/
    /* STRING VIEW                                                          */
    /************************************************************************/

    /// \brief Alias type for std::string_view.
    using StringView = std::basic_string_view<char>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether lhs is a prefix of rhs.
    /// \return Returns true if lhs is a prefix of rhs, returns false otherwise.
    bool IsPrefix(const StringView& lhs, const StringView& rhs);

    /// \brief Check whether lhs is a suffix of rhs.
    /// \return Returns true if lhs is a suffix of rhs, returns false otherwise.
    bool IsSuffix(const StringView& lhs, const StringView& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    inline bool IsPrefix(const StringView& lhs, const StringView& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(0, lhs.length(), lhs) == 0);
    }

    inline bool IsSuffix(const StringView& lhs, const StringView& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(rhs.length() - lhs.length(), lhs.length(), lhs) == 0);
    }
}
