
/// \file string.h
/// \brief This header is part of the Syntropy core module. It contains classes and functions for string definition and manipulation.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstring>
#include <string>
#include <string_view>
#include <iterator>

#include "syntropy/core/types.h"
#include "syntropy/core/range.h"
#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* BASIC STRING                                                         */
    /************************************************************************/

    /// \brief Type alias for std::basic_string with polymorphic allocator type.
    template <typename TCharacter, typename TTraits = std::char_traits<TCharacter>, typename TAllocator = PolymorphicAllocator<TCharacter>>
    using BasicString = std::basic_string<TCharacter, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC STRING VIEW                                                    */
    /************************************************************************/

    /// \brief Type alias for std::basic_string_view
    template <typename TCharacter, typename TTraits = std::char_traits<TCharacter>>
    using BasicStringView = std::basic_string_view<TCharacter, TTraits>;

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Alias type for std::basic_string<char> with polymorphic allocator type.
    using String = BasicString<char>;

    /// \brief Alias type for std::basic_string_view<char>.
    using StringView = BasicStringView<char>;

    /// \brief Type alias for a range of characters.
    using StringRange = Range<String::iterator>;

    /// \brief Type alias for a range of const characters.
    using ConstStringRange = Range<String::const_iterator>;

    /// \brief Type alias for a range of characters.
    using StringViewRange = Range<StringView::iterator>;

    /// \brief Type alias for a range of const characters.
    using ConstStringViewRange = Range<StringView::const_iterator>;

    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    /// \brief Exposes utility functions used to manipulate strings.
    /// #TODO Generalize StringView and String templates.
    namespace Strings
    {
        /// \brief Check whether lhs is a prefix of rhs.
        Bool IsPrefix(const StringView& lhs, const StringView& rhs);

        /// \brief Check whether lhs is a suffix of rhs.
        Bool IsSuffix(const StringView& lhs, const StringView& rhs);

        /// \brief Create a string view from a pair of iterators.
        /// \remarks BasicStringView can only constructor from a pair since C++20.
        StringView MakeView(String::iterator begin, String::iterator end);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the memory range associated to a string.
    StringRange GetRange(String& string);

    /// \brief Get the memory range associated to a string.
    ConstStringRange GetRange(const String& string);

    /// \brief R-values cannot be safely wrapped in a range.
    void GetRange(const String&& string) = delete;

    /// \brief Get the const memory range associated to a string.
    ConstStringRange GetConstRange(const String& string);

    /// \brief R-values cannot be safely wrapped in a const range.
    void GetConstRange(const String&& string) = delete;

    /// \brief Get the memory range associated to a string view.
    StringViewRange GetRange(StringView& string);

    /// \brief Get the memory range associated to a string view.
    ConstStringViewRange GetRange(const StringView& string);

    /// \brief R-values cannot be safely wrapped in a range.
    void GetRange(const StringView&& string) = delete;

    /// \brief Get the const memory range associated to a string view.
    ConstStringViewRange GetConstRange(const StringView& string);

    /// \brief R-values cannot be safely wrapped in a const range.
    void GetConstRange(const StringView&& string) = delete;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Strings.

    inline Bool Strings::IsPrefix(const StringView& lhs, const StringView& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(0, lhs.length(), lhs) == 0);
    }

    inline Bool Strings::IsSuffix(const StringView& lhs, const StringView& rhs)
    {
        return (lhs.length() <= rhs.length()) && (rhs.compare(rhs.length() - lhs.length(), lhs.length(), lhs) == 0);
    }

    inline StringView Strings::MakeView(String::iterator begin, String::iterator end)
    {
        return { &(*begin), static_cast<std::size_t>(std::distance(begin, end)) };
    }

    // Non-member functions.

    inline StringRange GetRange(String& string)
    {
        auto length = ToBytes(string.size());

        return { string.begin(), string.end() };
    }

    inline ConstStringRange GetRange(const String& string)
    {
        return GetConstRange(string);
    }

    inline ConstStringRange GetConstRange(const String& string)
    {
        return { string.cbegin(), string.cend() };
    }

    inline StringViewRange GetRange(StringView& string)
    {
        return { string.begin(), string.end() };
    }

    inline ConstStringViewRange GetRange(const StringView& string)
    {
        return GetConstRange(string);
    }

    inline ConstStringViewRange GetConstRange(const StringView& string)
    {
        return { string.cbegin(), string.cend() };
    }

}
