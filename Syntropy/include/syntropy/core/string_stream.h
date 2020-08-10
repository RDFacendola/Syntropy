
/// \file string_stream.h
/// \brief This header is part of Syntropy core module. It contains extensions to standard header <sstream> as well as functions to manipulate string streams.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <sstream>

#include "syntropy/experimental/core/polymorphic_allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* BASIC STRING BUF                                                     */
    /************************************************************************/

    /// \brief Type alias for a basic string buffer.
    template <typename TElement, typename TTraits = std::char_traits<TElement>, typename TAllocator = PolymorphicAllocator<TElement>>
    using BasicStringBuf = std::basic_stringbuf<TElement, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC ISTRING STREAM                                                 */
    /************************************************************************/

    /// \brief Type alias for a basic input string stream.
    template <typename TElement, typename TTraits = std::char_traits<TElement>, typename TAllocator = PolymorphicAllocator<TElement>>
    using BasicIStringStream = std::basic_istringstream<TElement, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC OSTRING STREAM                                                 */
    /************************************************************************/

    /// \brief Type alias for a basic output string stream.
    template <typename TElement, typename TTraits = std::char_traits<TElement>, typename TAllocator = PolymorphicAllocator<TElement>>
    using BasicOStringStream = std::basic_ostringstream<TElement, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC STRING STREAM                                                  */
    /************************************************************************/

/// \brief Type alias for a basic string stream.
    template <typename TElement, typename TTraits = std::char_traits<TElement>, typename TAllocator = PolymorphicAllocator<TElement>>
    using BasicStringStream = std::basic_stringstream<TElement, TTraits, TAllocator>;

    /************************************************************************/
    /* BASIC STRING VIEW                                                    */
    /************************************************************************/

    /// \brief Type alias for a basic string view.
    template <typename TElement, typename TTraits = std::char_traits<TElement>>
    using BasicStringView = std::basic_string_view<TElement, TTraits>;

    /************************************************************************/
    /* OSTRINGSTREAM                                                        */
    /************************************************************************/

    /// \brief Type alias for an output string stream.
    using StringBuf = BasicStringBuf<char>;

    /// \brief Type alias for an output string stream.
    using IStringStream = BasicIStringStream<char>;

    /// \brief Type alias for an output string stream.
    using OStringStream = BasicOStringStream<char>;

    /// \brief Type alias for an output string stream.
    using StringStream = BasicStringStream<char>;

}
