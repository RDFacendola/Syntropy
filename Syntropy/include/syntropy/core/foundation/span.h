
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/foundation/tuple.h"
#include "syntropy/core/foundation/range.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* SPAN COUNT                                                           */
    /************************************************************************/

    /// \brief Models a type that can be used as a range count type for spans.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TCount, typename TType>
    concept SpanCount = requires(Immutable<TCount> lhs)
        {
            // SizedRange.

            /// \brief Spans' count shall be comparable with each other.
            { lhs } -> TotallyOrdered;
        }
        && requires(Immutable<TCount> lhs, Immutable<TCount> rhs)
        {
            // RandomAccessRange.

            /// \brief Count type shall be closed under addition.
            { lhs + rhs } -> SameAs<TCount>;

            /// \brief Count type shall be closed under subtraction.
            { lhs - rhs } -> SameAs<TCount>;
        }
        && requires(Pointer<TType> data, Immutable<TCount> offset)
        {
            // ContiguousRange.

            /// \brief Data pointer shall move forwards when a positive offset is applied.
            { data + offset } -> SameAs<Pointer<TType>>;

            /// \brief Data pointer shall move backwards when a negative offset is applied.
            { data - offset } -> SameAs<Pointer<TType>>;
        };
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType, Concepts::SpanCount<TType> TCount = Int>
    class Span
    {
        template <typename TType, Concepts::SpanCount<TType> TCount>
        friend constexpr TCount Count(Immutable<Span<TType, TCount>> rhs) noexcept;

        template <typename TType, Concepts::SpanCount<TType> TCount>
        friend constexpr Pointer<TType> Data(Immutable<Span<TType, TCount>> rhs) noexcept;

    public:

        /// \brief Create an empty span.
        constexpr Span() noexcept = default;

        /// \brief Create an empty span.
        constexpr Span(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and their number.
        template <typename TBegin>
        constexpr Span(TBegin begin, Immutable<TCount> count) noexcept;

        /// \brief Copy constructor.
        template <typename UType, Concepts::SpanCount<TType> UCount>
        constexpr Span(Immutable<Span<UType, UCount>> rhs) noexcept;

        /// \brief Default destructor.
        ~Span() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType, Concepts::SpanCount<TType> UCount>
        constexpr Mutable<Span> operator=(Immutable<Span<UType, UCount>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        constexpr operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        constexpr Reference<TType> operator[](Immutable<TCount> index) const noexcept;

        /// \brief Swap this span with rhs.
        constexpr void Swap(Mutable<Span> rhs) noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        Pointer<TType> data_{ nullptr };

        /// \brief Number of elements in the span.
        TCount count_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, Concepts::SpanCount<TType> TCount, typename UType, Concepts::SpanCount<TType> UCount>
    constexpr Bool operator==(Immutable<Span<TType, TCount>> lhs, Immutable<Span<UType, UCount>> rhs) noexcept;

    // Forward range.
    // ==============

    /// \brief Access the first element in a span.
    /// \remarks Accessing the first element of an empty span results in undefined behavior.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Front(Immutable<Span<TType, TCount>> rhs) noexcept;

    /// \brief Discard the first count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> PopFront(Immutable<Span<TType, TCount>> rhs) noexcept;

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Bool IsEmpty(Immutable<Span<TType, TCount>> rhs) noexcept;

    // Sized range.
    // ============

    /// \brief Get the number of elements in a span.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr TCount Count(Immutable<Span<TType, TCount>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a span.
    /// \remarks Accessing the last element of an empty span results in undefined behavior.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Back(Immutable<Span<TType, TCount>> rhs) noexcept;

    /// \brief Discard the last count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> PopBack(Immutable<Span<TType, TCount>> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> Select(Immutable<Span<TType, TCount>> rhs, Immutable<TCount> offset, Immutable<TCount> count) noexcept;

    /// \brief Obtain a span element at given index.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Select(Immutable<Span<TType, TCount>> rhs, Immutable<TCount> index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Pointer<TType> Data(Immutable<Span<TType, TCount>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> MakeSpan(Pointer<TType> begin, Immutable<TCount> count) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for spans.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    struct RangeEnableTypeTraits<Span<TType, TCount>> : Alias<void> {};

    /// \brief Specialization for spans.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    struct RangeElementReferenceTypeTraits<Span<TType, TCount>> : Alias<Reference<TType>> {};

    /// \brief Specialization for spans.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    struct RangeElementPointerTypeTraits<Span<TType, TCount>> : Alias<Pointer<TType>> {};

    /// \brief Specialization for spans.
    template <typename TType, Concepts::SpanCount<TType> TCount>
    struct RangeElementCountTypeTraits<Span<TType, TCount>> : Alias<TCount> {};
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TType, TCount>.
    // ===============

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount>::Span(Null) noexcept
    {

    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    template <typename TBegin>
    constexpr Span<TType, TCount>::Span(TBegin begin, Immutable<TCount> count) noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    template <typename UType, Concepts::SpanCount<TType> UCount>
    constexpr Span<TType, TCount>::Span(Immutable<Span<UType, UCount>> rhs) noexcept
        : data_(ToPointer<TType>(Data(rhs)))
        , count_(Count(rhs))
    {

    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    template <typename UType, Concepts::SpanCount<TType> UCount>
    constexpr Mutable<Span<TType, TCount>> Span<TType, TCount>::operator=(Immutable<Span<UType, UCount>> rhs) noexcept
    {
        data_ = ToPointer<TType>(Data(rhs));
        count_ = Count(rhs);

        return *this;
    }
    
    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount>::operator Bool() const noexcept
    {
        return count_ > TCount{ 0 };
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Span<TType, TCount>::operator[](Immutable<TCount> index) const noexcept
    {
        return data_[index];
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr void Span<TType, TCount>::Swap(Mutable<Span<TType, TCount>> rhs) noexcept
    {
        Syntropy::Swap(data_, rhs.data_);
        Syntropy::Swap(count_, rhs.count_);
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename TType, Concepts::SpanCount<TType> TCount, typename UType, Concepts::SpanCount<TType> UCount>
    constexpr Bool operator==(Immutable<Span<TType, TCount>> lhs, Immutable<Span<UType, UCount>> rhs) noexcept
    {
        return AreEqual(lhs, rhs) || AreEquivalent(lhs, rhs);
    }

    // Forward Range.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Front(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return *Data(rhs);
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> PopFront(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return Select(rhs, TCount{ 1 }, Count(rhs) - TCount{ 1 });
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Bool IsEmpty(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return !rhs;
    }

    // Sized range.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr TCount Count(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return rhs.count_;
    }

    // Bidirectional range.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Back(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return *(Data(rhs) + Count(rhs) - TCount{ 1 });
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> PopBack(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return Select(rhs, TCount{ 0 }, Count(rhs) - TCount{ 1 });
    }

    // Random access range.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> Select(Immutable<Span<TType, TCount>> rhs, Immutable<TCount> offset, Immutable<TCount> count) noexcept
    {
        return { Data(rhs) + offset, count };
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Reference<TType> Select(Immutable<Span<TType, TCount>> rhs, Immutable<TCount> index) noexcept
    {
        return rhs[index];
    }

    // Contiguous range.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Pointer<TType> Data(Immutable<Span<TType, TCount>> rhs) noexcept
    {
        return rhs.data_;
    }

    // Utilities.

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr Span<TType, TCount> MakeSpan(Pointer<TType> begin, Immutable<TCount> count) noexcept
    {
        return { begin, count };
    }

    template <typename TType, Concepts::SpanCount<TType> TCount>
    constexpr void Swap(Mutable<Span<TType, TCount>> lhs, Mutable<Span<TType, TCount>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

// ===========================================================================