
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

namespace Syntropy
{
    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    class SpanT
    {

    public:

        /// \brief Create an empty span.
        constexpr SpanT() noexcept = default;

        /// \brief Create an empty span.
        constexpr SpanT(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and the number of elements.
        template <typename TBegin>
        constexpr SpanT(TBegin begin, Int count) noexcept;

        /// \brief Create a span given a pointer to the first element and a pointer past the last element.
        template <typename TBegin, typename TEnd, typename = Templates::EnableIf<!Templates::IsConvertible<TEnd, Int>>>
        constexpr SpanT(TBegin begin, TEnd end) noexcept;

        /// \brief Copy constructor.
        template <typename UType>
        constexpr SpanT(Immutable<SpanT<UType>> rhs) noexcept;

        /// \brief Default destructor.
        ~SpanT() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType>
        constexpr Mutable<SpanT> operator=(Immutable<SpanT<UType>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        constexpr operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        constexpr Reference<TType> operator[](Int index) const noexcept;

        /// \brief Get the number of elements in the span.
        constexpr Int GetCount() const noexcept;

        /// \brief Access the underlying memory.
        constexpr Pointer<TType> GetData() const noexcept;

        /// \brief Swap this span with rhs.
        constexpr void Swap(Mutable<SpanT> rhs) noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        Pointer<TType> data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Alias for a span of read-only elements.
    template <typename TType>
    using Span = SpanT<const TType>;

    /// \brief Alias for a span of read-write elements.
    template <typename TType, typename = Templates::EnableIf<!Templates::IsConst<TType>>>
    using RWSpan = SpanT<TType>;

    /// \brief Alias for a common type between two or more spans.
    template <typename... TTypes>
    using CommonSpan = SpanT<Templates::RemovePointer<Templates::CommonType<Templates::AddPointer<TTypes>...>>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range.
    // ==============

    /// \brief Access the first element in a span.
    /// \remarks Accessing the first element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Front(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Discard the first count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr SpanT<TType> PopFront(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TType>
    constexpr Bool IsEmpty(Immutable<SpanT<TType>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a span.
    /// \remarks Accessing the last element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Back(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Discard the last count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr SpanT<TType> PopBack(Immutable<SpanT<TType>> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Get the number of elements in a span.
    template <typename TType>
    constexpr Int Count(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr SpanT<TType> Select(Immutable<SpanT<TType>> rhs, Int offset, Int count) noexcept;

    /// \brief Obtain a span element at given index.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Select(Immutable<SpanT<TType>> rhs, Int index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TType>
    constexpr Pointer<TType> Data(Reference<SpanT<TType>> rhs) noexcept;

    // Set operations.
    // ===============

    /// \brief Get the smallest span including both lhs and rhs.
    /// \remarks This function may introduce elements that do not belong to either lhs and rhs. If those elements refer to an invalid memory region, accessing those elements results in undefined behavior.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Union(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Get the largest span shared between lhs and rhs.
    /// \remarks If lhs and rhs are disjoint, this method returns an unspecified empty span.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Intersection(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Reduce lhs from the back until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceFront(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Reduce lhs from the front until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceBack(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Check whether rhs is identical to any subset in lhs.
    template <typename TType, typename UType>
    constexpr Bool Contains(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    // Comparisons.
    // ============

    /// \brief Check whether lhs and rhs are identical.
    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TType, typename UType>
    constexpr Bool Equals(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Check whether lhs starts with rhs.
    /// \remarks If lhs starts with rhs, return true, otherwise return false.
    template <typename TType, typename UType>
    constexpr Bool StartsWith(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Check whether lhs ends with lhs.
    /// \remarks If lhs ends with rhs, return true, otherwise return false.
    template <typename TType, typename UType>
    constexpr Bool EndsWith(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    /// \brief Reduce lhs until lhs starts with rhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    ///         If rhs is empty lhs is returned instead.
    template <typename TType, typename UType>
    constexpr SpanT<TType> Find(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin>
    constexpr SpanT<TBegin> MakeSpan(Pointer<TBegin> begin, Int count) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin, typename TEnd>
    constexpr Templates::CommonType<TBegin, TEnd> MakeSpan(Pointer<TBegin> begin, Pointer<TEnd> end) noexcept;

    /// \brief Swap two spans
    template <typename TType>
    constexpr void Swap(Mutable<SpanT<TType>> lhs, Mutable<SpanT<TType>> rhs) noexcept;

    /// \brief Convert a span to its read-only equivalent.
    template <typename TType>
    constexpr Span<TType> ReadOnly(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Convert a span to its read-only equivalent.
    template <typename TType>
    constexpr Span<TType> ReadOnly(Movable<SpanT<TType>> rhs) noexcept;

    /// \brief Convert a read-only span to its read-write equivalent.
    /// If rhs doesn't refer to an original read-write memory location, the behavior of this method is undefined.
    template <typename TType>
    constexpr RWSpan<Templates::RemoveConst<TType>> ReadWrite(Immutable<SpanT<TType>> rhs) noexcept;

    /// \brief Convert a read-only span to its read-write equivalent.
    /// If rhs doesn't refer to an original read-write memory location, the behavior of this method is undefined.
    template <typename TType>
    constexpr RWSpan<Templates::RemoveConst<TType>> ReadWrite(Movable<SpanT<TType>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeTraits<SpanT<TType>>
    {
        /// \brief Type of a reference to a range element.
        using ElementReferenceType = Reference<TType>;
    };
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
    constexpr SpanT<TType>::SpanT(Null) noexcept
    {

    }

    template <typename TType>
    template <typename TBegin>
    constexpr SpanT<TType>::SpanT(TBegin begin, Int count) noexcept
        : data_((count > 0) ? begin : nullptr)
        , count_(count)
    {

    }

    template <typename TType>
    template <typename TBegin, typename TEnd, typename>
    constexpr SpanT<TType>::SpanT(TBegin begin, TEnd end) noexcept
        : SpanT(begin, ToInt(end - begin))
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr SpanT<TType>::SpanT(Immutable<SpanT<UType>> rhs) noexcept
        : data_(ToPtr<TType>(rhs.GetData()))
        , count_(rhs.GetCount())
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Mutable<SpanT<TType>> SpanT<TType>::operator=(Immutable<SpanT<UType>> rhs) noexcept
    {
        data_ = ToPtr<TType>(rhs.GetData());
        count_ = rhs.GetCount();

        return *this;
    }

    template <typename TType>
    constexpr SpanT<TType>::operator Bool() const noexcept
    {
        return count_ > 0;
    }

    template <typename TType>
    constexpr Reference<TType> SpanT<TType>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TType>
    constexpr Int SpanT<TType>::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TType>
    constexpr Pointer<TType> SpanT<TType>::GetData() const noexcept
    {
        return data_;
    }

    template <typename TType>
    constexpr void SpanT<TType>::Swap(Mutable<SpanT<TType>> rhs) noexcept
    {
        Syntropy::Swap(data_, rhs.data_);
        Syntropy::Swap(count_, rhs.count_);
    }

    // Non-member functions.
    // =====================

    // Forward Range.

    template <typename TType>
    constexpr Reference<TType> Front(Immutable<SpanT<TType>> rhs) noexcept
    {
        return *Data(rhs);
    }

    template <typename TType>
    constexpr SpanT<TType> PopFront(Immutable<SpanT<TType>> rhs) noexcept
    {
        return PopFront(rhs, 1);
    }

    template <typename TType>
    constexpr Bool IsEmpty(Immutable<SpanT<TType>> rhs) noexcept
    {
        return !rhs;
    }

    // Bidirectional range.

    template <typename TType>
    constexpr Reference<TType> Back(Immutable<SpanT<TType>> rhs) noexcept
    {
        return *(Data(rhs) + Count(rhs) - 1);
    }

    template <typename TType>
    constexpr SpanT<TType> PopBack(Immutable<SpanT<TType>> rhs) noexcept
    {
        return PopBack(rhs, 1);
    }

    // Random access range.

    template <typename TType>
    constexpr Int Count(Immutable<SpanT<TType>> rhs) noexcept
    {
        return rhs.GetCount();
    }

    template <typename TType>
    constexpr SpanT<TType> Select(Immutable<SpanT<TType>> rhs, Int offset, Int count) noexcept
    {
        return { Data(rhs) + offset, count };
    }

    template <typename TType>
    constexpr Reference<TType> Select(Immutable<SpanT<TType>> rhs, Int index) noexcept
    {
        return rhs[index];
    }

    // Contiguous range.

    template <typename TType>
    constexpr Pointer<TType> Data(Reference<SpanT<TType>> rhs) noexcept
    {
        return rhs.GetData();
    }

    // Set operations.

    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Union(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        if (lhs && rhs)
        {
            auto begin = (Begin(lhs) < Begin(rhs)) ? Begin(lhs) : Begin(rhs);
            auto end = (End(lhs) > End(rhs)) ? End(lhs) : End(rhs);

            return { begin, end };
        }

        return lhs ? lhs : rhs;
    }

    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Intersection(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        if (lhs && rhs)
        {
            auto begin = (Begin(lhs) > Begin(rhs)) ? Begin(lhs) : Begin(rhs);
            auto end = (End(lhs) > End(rhs)) ? End(lhs) : End(rhs);

            return { begin, (begin > end) ? begin : end };
        }

        return {};
    }

    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceFront(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        if (rhs)
        {
            auto begin = Begin(lhs);
            auto end = (End(lhs) < Begin(rhs)) ? End(lhs) : Begin(rhs);

            return { begin, (begin > end) ? begin : end };
        }

        return lhs;
    }

    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceBack(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        if (rhs)
        {
            auto begin = (Begin(lhs) > End(rhs)) ? Begin(lhs) : End(rhs);
            auto end = End(lhs);

            return { (begin < end) ? begin : end , end };
        }

        return lhs;
    }

    template <typename TType, typename UType>
    constexpr Bool Contains(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        return Intersection(lhs, rhs) == rhs;
    }

    // Comparisons.

    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    template <typename TType, typename UType>
    constexpr Bool Equals(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        if (Count(lhs) != Count(rhs))
        {
            return false;                           // Early out if spans sizes are different.
        }

        if constexpr (Templates::IsComparableForEquality<Pointer<TType>, Pointer<UType>>)
        {
            if (lhs.GetData() == rhs.GetData())
            {
                return true;                        // Early out if the two spans are identical.
            }
        }

        for (auto index = 0; index < Count(lhs); ++index)
        {
            if (lhs[index] != rhs[index])
            {
                return false;                       // Return on the first mismatch.
            }
        }

        return true;
    }

    template <typename TType, typename UType>
    constexpr Bool StartsWith(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Front(lhs, rhs_count), rhs));
    }

    template <typename TType, typename UType>
    constexpr Bool EndsWith(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Back(lhs, rhs_count), rhs));
    }

    //template <typename TType, typename UType>
    //constexpr SpanT<TType> Find(Immutable<SpanT<TType>> lhs, Immutable<SpanT<UType>> rhs) noexcept
    //{
    //    if (rhs)
    //    {
    //        auto result = Algorithm::Find(lhs, Front(rhs));

    //        for (; Count(result) >= Count(rhs); result = Algorithm::Find(PopFront(result), Front(rhs)))
    //        {
    //            if (StartsWith(result, rhs))
    //            {
    //                return result;
    //            }
    //        }

    //        return {};
    //    }

    //    return lhs;
    //}

    // Utilities.

    template <typename TBegin>
    constexpr SpanT<TBegin> MakeSpan(Pointer<TBegin> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TBegin, typename TEnd>
    constexpr Templates::CommonType<TBegin, TEnd> MakeSpan(Pointer<TBegin> begin, Pointer<TEnd> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType>
    constexpr void Swap(Mutable<SpanT<TType>> lhs, Mutable<SpanT<TType>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    template <typename TType>
    constexpr Span<TType> ReadOnly(Immutable<SpanT<TType>> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Span<TType> ReadOnly(Movable<SpanT<TType>> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWSpan<Templates::RemoveConst<TType>> ReadWrite(Immutable<SpanT<TType>> rhs) noexcept
    {
        auto begin = ReadWrite(Begin(rhs));
        auto end = ReadWrite(End(rhs));

        return { begin, end };
    }

    template <typename TType>
    constexpr RWSpan<Templates::RemoveConst<TType>> ReadWrite(Movable<SpanT<TType>> rhs) noexcept
    {
        return ReadWrite(rhs);
    }

}

// ===========================================================================