
/// \file string.h
/// \brief This header is part of the Syntropy core module. It contains classes and functions for string definition and manipulation.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <string_view>

#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* BASIC STRING                                                         */
    /************************************************************************/

    /// \brief Type alias for std::basic_string with polymorphic allocator type.
    template <typename TElement, typename TTraits = std::char_traits<TElement>, typename TAllocator = PolymorphicAllocator<TElement>>
    using BasicString = std::basic_string<TElement, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC STRING VIEW                                                    */
    /************************************************************************/

    /// \brief Type alias for std::basic_string_view
    template <typename TElement, typename TTraits = std::char_traits<TElement>>
    using BasicStringView = std::basic_string_view<TElement, TTraits>;

    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    /// \brief Alias type for std::basic_string<char> with polymorphic allocator type.
    using String = BasicString<char>;

    /************************************************************************/
    /* STRING VIEW                                                          */
    /************************************************************************/

    /// \brief Alias type for std::basic_string_view<char>.
    using StringView = BasicStringView<char>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether lhs is a prefix of rhs.
    bool IsPrefix(const StringView& lhs, const StringView& rhs);

    /// \brief Check whether lhs is a suffix of rhs.
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
