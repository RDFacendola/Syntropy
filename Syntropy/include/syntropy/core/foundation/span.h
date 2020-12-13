
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/concepts/range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    class Span
    {
        template <typename TType>
        friend constexpr Int Count(Immutable<Span<TType>> rhs) noexcept;

        template <typename TType>
        friend constexpr Pointer<TType> Data(Immutable<Span<TType>> rhs) noexcept;

    public:

        /// \brief Create an empty span.
        constexpr Span() noexcept = default;

        /// \brief Create an empty span.
        constexpr Span(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and their number.
        constexpr Span(Pointer<TType> begin, Int count) noexcept;

        /// \brief Create a span given a pointer to both the first and past the last element.
        constexpr Span(Pointer<TType> begin, Pointer<TType> end) noexcept;

        /// \brief Copy constructor.
        template <typename UType>
        constexpr Span(Immutable<Span<UType>> rhs) noexcept;

        /// \brief Default destructor.
        ~Span() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType>
        constexpr Mutable<Span> operator=(Immutable<Span<UType>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        constexpr explicit operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        constexpr Reference<TType> operator[](Int index) const noexcept;

        /// \brief Swap this span with rhs.
        constexpr void Swap(Mutable<Span> rhs) noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        Pointer<TType> data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a span.
    /// \remarks Accessing the first element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Front(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Discard the first count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr Span<TType> PopFront(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TType>
    constexpr Bool IsEmpty(Immutable<Span<TType>> rhs) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a span.
    template <typename TType>
    constexpr Int Count(Immutable<Span<TType>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a span.
    /// \remarks Accessing the last element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Back(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Discard the last count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr Span<TType> PopBack(Immutable<Span<TType>> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr Span<TType> Select(Immutable<Span<TType>> rhs, Int offset, Int count) noexcept;

    /// \brief Obtain a span element at given index.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Select(Immutable<Span<TType>> rhs, Int index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TType>
    constexpr Pointer<TType> Data(Immutable<Span<TType>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    constexpr Span<TType> MakeSpan(Pointer<TType> begin, Int count) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeEnableTypeTraits<Span<TType>> : Alias<void> {};

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeElementReferenceTypeTraits<Span<TType>> : Alias<Reference<TType>> {};

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeElementPointerTypeTraits<Span<TType>> : Alias<Pointer<TType>> {};

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeElementCountTypeTraits<Span<TType>> : Alias<Int> {};
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TType>.
    // ===============

    template <typename TType>
    constexpr Span<TType>::Span(Null) noexcept
    {

    }

    template <typename TType>
    constexpr Span<TType>::Span(Pointer<TType> begin, Int count) noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TType>
    constexpr Span<TType>::Span(Pointer<TType> begin, Pointer<TType> end) noexcept
        : Span(begin, ToInt(end - begin))
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Span<TType>::Span(Immutable<Span<UType>> rhs) noexcept
        : data_(ToPointer<TType>(Data(rhs)))
        , count_(Count(rhs))
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Mutable<Span<TType>> Span<TType>::operator=(Immutable<Span<UType>> rhs) noexcept
    {
        data_ = ToPointer<TType>(Data(rhs));
        count_ = Count(rhs);

        return *this;
    }
    
    template <typename TType>
    constexpr Span<TType>::operator Bool() const noexcept
    {
        return count_ > ToInt(0);
    }

    template <typename TType>
    constexpr Reference<TType> Span<TType>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TType>
    constexpr void Span<TType>::Swap(Mutable<Span<TType>> rhs) noexcept
    {
        Syntropy::Swap(data_, rhs.data_);
        Syntropy::Swap(count_, rhs.count_);
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        return AreEqual(lhs, rhs) || AreEquivalent(lhs, rhs);
    }

    // Forward Range.

    template <typename TType>
    constexpr Reference<TType> Front(Immutable<Span<TType>> rhs) noexcept
    {
        return *Data(rhs);
    }

    template <typename TType>
    constexpr Span<TType> PopFront(Immutable<Span<TType>> rhs) noexcept
    {
        return { Data(rhs) + ToInt(1), Data(rhs) + Count(rhs) };
    }

    template <typename TType>
    constexpr Bool IsEmpty(Immutable<Span<TType>> rhs) noexcept
    {
        return !rhs;
    }

    // Sized range.

    template <typename TType>
    constexpr Int Count(Immutable<Span<TType>> rhs) noexcept
    {
        return rhs.count_;
    }

    // Bidirectional range.

    template <typename TType>
    constexpr Reference<TType> Back(Immutable<Span<TType>> rhs) noexcept
    {
        return *(Data(rhs) + Count(rhs) - ToInt(1));
    }

    template <typename TType>
    constexpr Span<TType> PopBack(Immutable<Span<TType>> rhs) noexcept
    {
        return { Data(rhs), Data(rhs) + Count(rhs) - ToInt(1) };
    }

    // Random access range.

    template <typename TType>
    constexpr Span<TType> Select(Immutable<Span<TType>> rhs, Int offset, Int count) noexcept
    {
        return { Data(rhs) + offset, count };
    }

    template <typename TType>
    constexpr Reference<TType> Select(Immutable<Span<TType>> rhs, Int index) noexcept
    {
        return rhs[index];
    }

    // Contiguous range.

    template <typename TType>
    constexpr Pointer<TType> Data(Immutable<Span<TType>> rhs) noexcept
    {
        return rhs.data_;
    }

    // Utilities.

    template <typename TType>
    constexpr Span<TType> MakeSpan(Pointer<TType> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    constexpr void Swap(Mutable<Span<TType>> lhs, Mutable<Span<TType>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

// ===========================================================================