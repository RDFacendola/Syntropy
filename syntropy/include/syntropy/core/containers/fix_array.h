
/// \file fix_array.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/tuples/ntuple.h"
#include "syntropy/core/ranges/span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TType, Int VSize, typename TTraits>
    class BaseFixArray;

    template <typename TType>
    struct FixArrayTypeTraits;

    template <typename TType>
    struct RWFixArrayTypeTraits;

    /************************************************************************/
    /* ALIAS TYPES                                                          */
    /************************************************************************/

    /// \brief A fixed-size array of read-write elements.
    template <typename TType, Int VSize>
    using RWFixArray = BaseFixArray<TType, VSize, RWFixArrayTypeTraits<TType>>;

    /// \brief A fixed-size array of read-only elements.
    template <typename TType, Int VSize>
    using FixArray = BaseFixArray<TType, VSize, FixArrayTypeTraits<TType>>;

    /************************************************************************/
    /* BASE FIX ARRAY                                                       */
    /************************************************************************/

    /// \brief Represents a fixed-size array of elements of the same type.
    /// \author Raffaele D. Facendola - January 2021
    template <typename TType, Int VSize, typename TTraits>
    class BaseFixArray
    {
        template <typename UType, Int USize, typename UTraits>
        friend class BaseFixArray;

    public:

        /// \brief Mutable reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Default constructor.
        constexpr BaseFixArray() = default;

        /// \brief Direct constructor.
        ///
        /// \remarks This method participates in overload resolution only if
        ///          the number of elements matches the array size.
        template <
            typename... TTypes,
            typename = Templates::EnableIf<sizeof...(TTypes) == VSize>>
        constexpr BaseFixArray(Forwarding<TTypes>... elements) noexcept;

        /// \brief Default copy constructor.
        constexpr BaseFixArray(Immutable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default move constructor.
        constexpr BaseFixArray(Movable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<BaseFixArray>
        operator=(Immutable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        constexpr Mutable<BaseFixArray>
        operator=(Movable<BaseFixArray> rhs) noexcept = default;

        /// \brief Converting copy-constructor operator.
        template <typename UType, typename UTraits>
        constexpr
        BaseFixArray(
            Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting move-constructor operator.
        template <typename UType, typename UTraits>
        constexpr
        BaseFixArray(
            Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting copy-assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseFixArray>
        operator=(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting move-assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseFixArray>
        operator=(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseFixArray() noexcept = default;

        /// \brief Implicit conversion to Span.
        constexpr
        operator Span<TType>() const noexcept;

        /// \brief Implicit conversion to RWSpan.
        constexpr
        operator RWSpan<TType>() noexcept;

        /// \brief Access the array by index.
        [[nodiscard]] constexpr TReference
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
        GetSize() const noexcept;

    private:

        /// \brief Tag type used to construct a fix array by unwinding
        ///        another fix array.
        struct UnwindTag {};

        /// \brief Construct a fix-array by unwinding another fix array.
        template<typename TFixArray, Int... VIndexes>
        constexpr
        BaseFixArray(UnwindTag,
                     Forwarding<TFixArray> other,
                     Syntropy::Templates::Sequence<VIndexes...>) noexcept;

        /// \brief Array elements.
        TType elements_[VSize];

    };

    /************************************************************************/
    /* FIX ARRAY                                                            */
    /************************************************************************/

    /// \brief Traits for read-only fixed-size arrays.
    template <typename TType>
    struct FixArrayTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = Ptr<TType>;

        /// \brief Reference type.
        using TReference = Immutable<TType>;
    };

    /************************************************************************/
    /* RW FIX ARRAY                                                         */
    /************************************************************************/

    /// \brief Traits for read-write fixed-size arrays.
    template <typename TType>
    struct RWFixArrayTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = RWPtr<TType>;

        /// \brief Reference type.
        using TReference = Mutable<TType>;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // N-Tuple.
    // ========

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immutable<TType>
    Get(Immutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Mutable<TType>
    Get(Mutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immovable<TType>
    Get(Immovable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Movable<TType>
    Get(Movable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs,
               Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs,
                Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

    // Ranges.
    // =======

    /// \brief Get a read-only view to a fix-array.
    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Span<TType>
    ViewOf(Immutable<BaseFixArray<TType, VSize, TTraits>> fix_array) noexcept;

    /// \brief Get a read-write view to a fix-array.
    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType>
    ViewOf(Mutable<BaseFixArray<TType, VSize, TTraits>> fix_array) noexcept;

}

// ===========================================================================

namespace Syntropy::Tuples::Templates
{
    /************************************************************************/
    /* TUPLE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for fix-arrays.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    struct ElementTypeTraits<VIndex, BaseFixArray<TType, VSize, TTraits>>
        : Syntropy::Templates::Alias<TType> {};

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct RankTypeTraits<BaseFixArray<TType, VSize, TTraits>>
        : Syntropy::Templates::IntConstant<VSize> {};
}

// ===========================================================================

#include "fix_array.inl"

// ===========================================================================
