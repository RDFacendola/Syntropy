
/// \file fix_array.h
/// \brief This header is part of the Syntropy core module. It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/ntuple.h"

#include "syntropy/core/foundation/span.h"

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
        /// \remarks This method participates in overload resolution only if the number of elements matches the array size.
        template <typename... TTypes, typename = Templates::EnableIf<sizeof...(TTypes) == VSize>>
        constexpr BaseFixArray(Forwarding<TTypes>... elements) noexcept;

        /// \brief Default copy constructor.
        constexpr BaseFixArray(Immutable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default move constructor.
        constexpr BaseFixArray(Movable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<BaseFixArray> operator=(Immutable<BaseFixArray> rhs) noexcept = default;

        /// \brief Default move-assignment operator.
        constexpr Mutable<BaseFixArray> operator=(Movable<BaseFixArray> rhs) noexcept = default;

        /// \brief Converting copy-constructor operator.
        template <typename UType, typename UTraits>
        constexpr BaseFixArray(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting move-constructor operator.
        template <typename UType, typename UTraits>
        constexpr BaseFixArray(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting copy-assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseFixArray> operator=(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Converting move-assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseFixArray> operator=(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseFixArray() noexcept = default;

        /// \brief Access the array by index.
        [[nodiscard]] constexpr TReference operator[](Int index) noexcept;

        /// \brief Access the array by index.
        [[nodiscard]] constexpr Immutable<TType> operator[](Int index) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr Ptr<TType> GetData() const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr RWPtr<TType> GetData() noexcept;

        /// \brief Get the number of elements in the array.
        [[nodiscard]] constexpr Int GetSize() const noexcept;

    private:

        /// \brief Tag type used to construct a fix array by unwinding another fix array.
        struct UnwindTag {};

        /// \brief Construct a fix-array by unwinding another fix array.
        template<typename TFixArray, Int... VIndexes>
        constexpr BaseFixArray(UnwindTag, Forwarding<TFixArray> other, Syntropy::Templates::Sequence<VIndexes...>) noexcept;

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
    [[nodiscard]] constexpr Immutable<TType> Get(Immutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Mutable<TType> Get(Mutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immovable<TType> Get(Immovable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks The program is ill-formed if no such element exists.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Movable<TType> Get(Movable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename TTraits, typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, typename TTraits, typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Get a full view of a read-write fix-array.
    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ranges::RWSpan<TType> RangeOf(Mutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept;

    /// \brief Get a full view of a read-only fix-array.
    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ranges::Span<TType> RangeOf(Immutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct ElementReferenceTypeTraits<BaseFixArray<TType, VSize, TTraits>> : Syntropy::Templates::Alias<typename TTraits::TReference> {};
}

// ===========================================================================

namespace Syntropy::Tuples::Templates
{
    /************************************************************************/
    /* TUPLE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for fix-arrays.
    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    struct ElementTypeTraits<VIndex, BaseFixArray<TType, VSize, TTraits>> : Syntropy::Templates::Alias<TType> {};

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct RankTypeTraits<BaseFixArray<TType, VSize, TTraits>> : Syntropy::Templates::IntConstant<VSize> {};
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BaseFixArray.
    // =============

    template <typename TType, Int VSize, typename TTraits>
    template <typename... TTypes, typename>
    constexpr BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Forwarding<TTypes>... elements) noexcept
        : elements_{ Forward<TTypes>(elements)... }
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr  BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
        : BaseFixArray(UnwindTag{}, rhs, Syntropy::Templates::MakeSequence<VSize>{})
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr  BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
        : BaseFixArray(UnwindTag{}, Move(rhs), Syntropy::Templates::MakeSequence<VSize>{})
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr   Mutable<BaseFixArray<TType, VSize, TTraits>> BaseFixArray<TType, VSize, TTraits>::operator=(Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        Ranges::Copy(RangeOf(rhs), RangeOf(*this));

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr   Mutable<BaseFixArray<TType, VSize, TTraits>> BaseFixArray<TType, VSize, TTraits>::operator=(Movable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        Ranges::Move(RangeOf(rhs), RangeOf(*this));

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr typename BaseFixArray<TType, VSize, TTraits>::TReference BaseFixArray<TType, VSize, TTraits>::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immutable<TType> BaseFixArray<TType, VSize, TTraits>::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ptr<TType> BaseFixArray<TType, VSize, TTraits>::GetData() const noexcept
    {
        return (VSize > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr RWPtr<TType> BaseFixArray<TType, VSize, TTraits>::GetData() noexcept
    {
        return (VSize > 0) ? PtrOf(elements_[0]) : nullptr;
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Int BaseFixArray<TType, VSize, TTraits>::GetSize() const noexcept
    {
        return VSize;
    }

    template <typename TType, Int VSize, typename TTraits>
    template<typename TFixArray, Int... VIndexes>
    constexpr BaseFixArray<TType, VSize, TTraits>::BaseFixArray(UnwindTag, Forwarding<TFixArray> other, Syntropy::Templates::Sequence<VIndexes...>) noexcept
        : BaseFixArray(Get<VIndexes>(Forward<TFixArray>(other))...)
    {

    }

    // Non-member functions.
    // =====================

    // N-Tuple.

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immutable<TType> Get(Immutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize), "Index out-of-range.");

        return ToImmutable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Mutable<TType> Get(Mutable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize), "Index out-of-range.");

        return ToMutable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Immovable<TType> Get(Immovable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize), "Index out-of-range.");

        return ToImmovable(tuple[VIndex]);
    }

    template <Int VIndex, typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Movable<TType> Get(Movable<BaseFixArray<TType, VSize, TTraits>> tuple) noexcept
    {
        static_assert((VIndex >= 0) && (VIndex < VSize), "Index out-of-range.");

        return Move(tuple[VIndex]);
    }

    // Comparison.

    template <typename TType, typename TTraits, typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        return Ranges::AreEquivalent(RangeOf(lhs), RangeOf(rhs));
    }

    template <typename TType, typename TTraits, typename UType, typename UTraits, Int VSize>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, VSize, UTraits>> rhs) noexcept
    {
        return Ranges::Compare(RangeOf(lhs), RangeOf(rhs));
    }

    // Utilities.

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ranges::RWSpan<TType> RangeOf(Mutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept
    {
        return Ranges::MakeSpan(rhs.GetData(), rhs.GetSize());
    }

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr Ranges::Span<TType> RangeOf(Immutable<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept
    {
        return Ranges::MakeSpan(rhs.GetData(), rhs.GetSize());
    }
}

// ===========================================================================