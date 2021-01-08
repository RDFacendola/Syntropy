
/// \file fix_array.h
/// \brief This header is part of the Syntropy core module. It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/foundation/span.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE FIX ARRAY                                                       */
    /************************************************************************/

    /// \brief Represents a fixed-size array of elements o the same type..
    /// \author Raffaele D. Facendola - January 2021
    template <typename TType, Int VSize, typename TTraits>
    class BaseFixArray
    {
        template <typename TType, Int VSize, typename TTraits>
        friend constexpr auto RangeOf(Reference<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept;

    public:

        /// \brief Mutable reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Default constructor. Default-initializes each element in the array.
        BaseFixArray() noexcept;

        /// \brief Direct constructor. Direct-initializes each element in the array.
        template <typename... TTypes>
        BaseFixArray(Forwarding<TTypes>... elements) noexcept;

        /// \brief Element-wise copy-constructor.
        BaseFixArray(Immutable<BaseFixArray> rhs) noexcept;

        /// \brief Element-wise move-constructor.
        BaseFixArray(Movable<BaseFixArray> rhs) noexcept;

        /// \brief Destructor. Destroy each element in the array.
        ~BaseFixArray() noexcept;

        /// \brief Element-wise copy-assignment operator.
        Mutable<BaseFixArray> operator=(Immutable<BaseFixArray> rhs) noexcept;

        /// \brief Element-wise move-assignment operator.
        Mutable<BaseFixArray> operator=(Movable<BaseFixArray> rhs) noexcept;

        /// \brief Access the array by index.
        TReference operator[](Int index) noexcept;

        /// \brief Access the array by index.
        Immutable<TType> operator[](Int index) const noexcept;

    private:

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

    /// \brief Represents a fixed-size array of read-only elements.
    template <typename TType, Int VSize>
    using FixArray = BaseFixArray<TType, VSize, FixArrayTypeTraits<TType>>;

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

    /// \brief Represents a fixed-size array of read-write elements.
    template <typename TType, Int VSize>
    using RWFixArray = BaseFixArray<TType, VSize, RWFixArrayTypeTraits<TType>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, Int VSize, typename TTraits, typename UType, Int USize, typename UTraits>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, USize, UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, Int VSize, typename TTraits, typename UType, Int USize, typename UTraits>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseFixArray<TType, VSize, TTraits>> lhs, Immutable<BaseFixArray<UType, USize, UTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Get a full view of a fix-array.
    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr auto RangeOf(Forwarding<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct RangeElementReferenceTypeTraits<BaseFixArray<TType, VSize, TTraits>> : Alias<typename TTraits::TReference> {};

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct RangeElementPointerTypeTraits<BaseFixArray<TType, VSize, TTraits>> : Alias<typename TTraits::TPointer> {};

    /// \brief Specialization for fix-arrays.
    template <typename TType, Int VSize, typename TTraits>
    struct RangeElementCountTypeTraits<BaseFixArray<TType, VSize, TTraits>> : Alias<Int> {};
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
    inline BaseFixArray<TType, VSize, TTraits>::BaseFixArray() noexcept
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    template <typename... TTypes>
    inline BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Forwarding<TTypes>... elements) noexcept
        : elements_{ Forward<TTypes>(elements)... }
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    inline BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Immutable<BaseFixArray> rhs) noexcept
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    inline BaseFixArray<TType, VSize, TTraits>::BaseFixArray(Movable<BaseFixArray> rhs) noexcept
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    inline BaseFixArray<TType, VSize, TTraits>::~BaseFixArray() noexcept
    {

    }

    template <typename TType, Int VSize, typename TTraits>
    inline Mutable<BaseFixArray<TType, VSize, TTraits>> BaseFixArray<TType, VSize, TTraits>::operator=(Immutable<BaseFixArray> rhs) noexcept
    {

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    inline Mutable<BaseFixArray<TType, VSize, TTraits>> BaseFixArray<TType, VSize, TTraits>::operator=(Movable<BaseFixArray> rhs) noexcept
    {

        return *this;
    }

    template <typename TType, Int VSize, typename TTraits>
    inline typename BaseFixArray<TType, VSize, TTraits>::TReference BaseFixArray<TType, VSize, TTraits>::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TType, Int VSize, typename TTraits>
    inline Immutable<TType> BaseFixArray<TType, VSize, TTraits>::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    // Utilities.
    // ==========

    template <typename TType, Int VSize, typename TTraits>
    [[nodiscard]] constexpr auto RangeOf(Reference<BaseFixArray<TType, VSize, TTraits>> rhs) noexcept
    {
        return Ranges::MakeSpan(PtrOf(rhs.elements_[0]), VSize);
    }

}

// ===========================================================================