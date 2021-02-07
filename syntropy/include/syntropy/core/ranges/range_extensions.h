
/// \file range_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for range-related customization-point
///        objects
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* FORWARD RANGE EXTENSIONS                                             */
    /************************************************************************/

    /// \brief Access range's first element.
    template <typename TType, typename = void>
    struct Front;

    /// \brief Discard range's first element and return the resulting range.
    template <typename TType, typename = void>
    struct PopFront;

    /// \brief Check whether a range is empty.
    template <typename TType, typename = void>
    struct IsEmpty;

    /************************************************************************/
    /* SIZED RANGE EXTENSIONS                                               */
    /************************************************************************/

    /// \brief Get range's elements count.
    template <typename TType, typename = void>
    struct Count;

    /************************************************************************/
    /* BIDIRECTIONAL RANGE EXTENSIONS                                       */
    /************************************************************************/

    /// \brief Access range's last element.
    template <typename TType, typename = void>
    struct Back;

    /// \brief Discard range's last element and return the resulting range.
    template <typename TType, typename = void>
    struct PopBack;

    /************************************************************************/
    /* RANDOM ACCESS RANGE EXTENSIONS                                       */
    /************************************************************************/

    /// \brief Access range's element by index.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    template <typename TType, typename = void>
    struct At;

    /// \brief Obtain a view to a sub-range.
    /// \remarks Exceeding range boundaries results in undefined behavior.
    /// \brief Obtain a view to a sub-range.
    template <typename TType, typename = void>
    struct Slice;

    /************************************************************************/
    /* CONTIGUOUS RANGE EXTENSIONS                                          */
    /************************************************************************/

    /// \brief Access range's element storage.
    template <typename TType, typename = void>
    struct Data;

}

// ===========================================================================
