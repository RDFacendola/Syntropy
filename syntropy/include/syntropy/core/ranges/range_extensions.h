
/// \file range_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for range-related customization-point
///        objects.
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

// ===========================================================================

namespace Syntropy::Ranges::Extensions
{
    /************************************************************************/
    /* RANGE EXTENSIONS                                                     */
    /************************************************************************/

    /// \brief Get a view to a range' elements
    template <typename TRange, typename = void>
    struct ViewOf;

    /// \brief Check whether a range view is empty.
    template <typename TRangeView, typename = void>
    struct IsEmpty;

    /// \brief Get the number of elements in a range view.
    template <typename TRangeView, typename = void>
    struct Count;

    /// \brief Select a subrange o elements.
    template <typename TRangeView, typename = void>
    struct Select;

    /// \brief Access the first element in a range view.
    template <typename TRangeView, typename = void>
    struct Front;

    /// \brief Access the last element in a range view.
    template <typename TRangeView, typename = void>
    struct Back;

    /// \brief Discard the first element in a range view and return a view to
    ///        the remaining elements.
    template <typename TRangeView, typename = void>
    struct PopFront;

    /// \brief Discard the last element in a range view and return a view to
    ///        the remaining elements.
    template <typename TRangeView, typename = void>
    struct PopBack;

    /// \brief Access an element of a range view by index.
    ///
    /// \remarks Exceeding range view boundaries results in undefined behavior.
    template <typename TRangeView, typename = void>
    struct At;

    /// \brief Access the element storage of a range view.
    template <typename TRangeView, typename = void>
    struct Data;

}

// ===========================================================================
