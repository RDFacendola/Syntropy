
/// \file fix_array.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - January 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/support/tuple.h"
#include "syntropy/core/ranges/span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FIX ARRAY                                                            */
    /************************************************************************/

    /// \brief Represents a fixed-size array of elements of the same type.
    /// \author Raffaele D. Facendola - January 2021
    template <typename TType, Int TCount>
    class FixArray
    {
    public:

        /// \brief Default constructor.
        constexpr
        FixArray() = default;

        /// \brief Direct constructor.
        template <typename... TTypes>
        requires (sizeof...(TTypes) == TCount)
        constexpr
        FixArray(Forwarding<TTypes>... elements) noexcept;

        /// \brief Converting copy-constructor.
        template <typename UType>
        constexpr
        FixArray(Immutable<FixArray<UType, TCount>> rhs) noexcept;

        /// \brief Converting move-constructor.
        template <typename UType>
        constexpr
        FixArray(Movable<FixArray<UType, TCount>> rhs) noexcept;

        /// \brief Converting copy-assignment operator.
        template <typename UType>
        constexpr Mutable<FixArray>
        operator=(Immutable<FixArray<UType, TCount>> rhs) noexcept;

        /// \brief Converting move-assignment operator.
        template <typename UType>
        constexpr Mutable<FixArray>
        operator=(Movable<FixArray<UType, TCount>> rhs) noexcept;

        /// \brief Default destructor.
        ~FixArray() noexcept = default;

        /// \brief Implicit conversion to Span.
        constexpr
        operator Span<TType>() const noexcept;

        /// \brief Implicit conversion to RWSpan.
        constexpr
        operator RWSpan<TType>() noexcept;

        /// \brief Access an element by index.
        [[nodiscard]] constexpr Mutable<TType>
        operator[](Int index) noexcept;

        /// \brief Access an element by index.
        [[nodiscard]] constexpr Immutable<TType>
        operator[](Int index) const noexcept;

    private:

        /// \brief Tag type used to element-wise-construct a fix-array.
        struct ElementwiseTag {};

        /// \brief Construct a fix-array by from another fix-array.
        template<typename TFixArray, Int... TIndexes>
        constexpr
        FixArray(ElementwiseTag,
                 Templates::Sequence<TIndexes...>,
                 Forwarding<TFixArray> other) noexcept;

        /// \brief Array elements.
        TType elements_[TCount];

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Element access.
    // ===============

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access a tuple element by index.
    ///
    /// \remarks Ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Movable<TType>
    Get(Movable<FixArray<TType, TCount>> fix_array) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename UType, Int TCount>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<FixArray<TType, TCount>> lhs,
               Immutable<FixArray<UType, TCount>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, typename UType, Int TCount>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<FixArray<TType, TCount>> lhs,
                Immutable<FixArray<UType, TCount>> rhs) noexcept;

    // Ranges.
    // =======

    /// \brief Get a read-only view to a fix-array.
    template <typename TType, Int TCount>
    [[nodiscard]] constexpr Span<TType>
    ViewOf(Immutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Get a read-write view to a fix-array.
    template <typename TType, Int TCount>
    [[nodiscard]] constexpr RWSpan<TType>
    ViewOf(Mutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Prevent from getting a view to a temporary array.
    template <typename TType, Int TCount>
    void
    ViewOf(Immovable<FixArray<TType, TCount>> fix_array) noexcept = delete;

    // Swap.
    // =====

    /// \brief Member-wise swap two fix-arrays.
    template <typename TType, typename UType, Int TCount>
    constexpr void
    Swap(Mutable<FixArray<TType, TCount>> lhs,
         Mutable<FixArray<UType, TCount>> rhs) noexcept;

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Partial template specialization for fix-arrays.
    template <typename TType, Int TCount>
    struct Records::RankTrait<FixArray<TType, TCount>>
        : Templates::IntType<TCount> {};

    /// \brief Partial template specialization fix-arrays.
    template <Int TIndex, typename TType, Int TCount>
    struct Records::ElementTypeTrait<TIndex, FixArray<TType, TCount>>
        : Templates::Alias<TType> {};

}

// ===========================================================================

#include "details/fix_array.inl"

// ===========================================================================
