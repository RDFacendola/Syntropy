
/// \file fix_array.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/records/record.h"
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
        constexpr FixArray() = default;

        /// \brief Direct constructor.
        ///
        /// \remarks This method participates in overload resolution only if
        ///          the number of elements matches the array size.
        template <
            typename... TTypes,
            typename = Templates::EnableIf<sizeof...(TTypes) == TCount>>
        constexpr FixArray(Forwarding<TTypes>... elements) noexcept;

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

        /// \brief Access the array by index.
        [[nodiscard]] constexpr Mutable<TType>
        operator[](Int index) noexcept;

        /// \brief Access the array by index.
        [[nodiscard]] constexpr Immutable<TType>
        operator[](Int index) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr Ptr<TType>
        GetData() const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr RWPtr<TType>
        GetData() noexcept;

        /// \brief Get the number of elements in the array.
        [[nodiscard]] constexpr Int
        GetCount() const noexcept;

        /// \brief Swap the content of two fix-array.
        constexpr void
        Swap(Movable<FixArray> rhs) noexcept;

    private:

        /// \brief Tag type used to construct a fix array by unwinding
        ///        another fix array.
        struct UnwindTag {};

        /// \brief Construct a fix-array by unwinding another fix array.
        template<typename TFixArray, Int... VIndexes>
        constexpr
        FixArray(UnwindTag,
                 Forwarding<TFixArray> other,
                 Syntropy::Templates::Sequence<VIndexes...>) noexcept;

        /// \brief Array elements.
        TType elements_[TCount];

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // N-Tuple.
    // ========

    /// \brief Access the VIndex-th element in a fix-array.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access the VIndex-th element in a fix-array.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access the VIndex-th element in a fix-arrayfix-array.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int TIndex, typename TType, Int TCount>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<FixArray<TType, TCount>> fix_array) noexcept;

    /// \brief Access the VIndex-th element in a fix-array.
    /// \remarks The program is ill-formed if no such element exists.
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
}

// ===========================================================================

namespace Syntropy::Records::Templates
{
    /************************************************************************/
    /* TUPLE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int TCount>
    struct RecordRankTrait<FixArray<TType, TCount>>
        : Syntropy::Templates::IntConstant<TCount> {};
}

// ===========================================================================

#include "fix_array.inl"

// ===========================================================================
