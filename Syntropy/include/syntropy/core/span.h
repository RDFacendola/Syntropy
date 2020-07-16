
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* SPAN <ELEMENT>                                                       */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TElement>
    class SpanT
    {
    public:

        /// \brief Create an empty span.
        constexpr SpanT() noexcept = default;

        /// \brief Create a span given a pointer to the first element and the number of elements.
        template <typename TBegin>
        constexpr SpanT(TBegin begin, Int count) noexcept;

        /// \brief Create a span given a pointer to the first element and a pointer past the last element.
        template <typename TBegin, typename TEnd, typename = EnableIfT<!IsConvertibleV<TEnd, Int>>>
        constexpr SpanT(TBegin begin, TEnd end) noexcept;

        /// \brief Copy constructor.
        template <typename UElement>
        constexpr SpanT(const SpanT<UElement>& rhs) noexcept;

        /// \brief Default destructor.
        ~SpanT() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UElement>
        constexpr SpanT& operator=(const SpanT<UElement>& rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        constexpr operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        constexpr TElement& operator[](Int index) const noexcept;

        /// \brief Get the number of elements in the span.
        constexpr Int GetCount() const noexcept;

        /// \brief Access the underlying memory.
        constexpr Pointer<TElement> GetData() const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        Pointer<TElement> data_{ nullptr };

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
    template <typename TType>
    using RWSpan = SpanT<TType>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Iterators.

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> begin(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> end(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr Pointer<TElement> Begin(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr Pointer<TElement> End(const SpanT<TElement>& span) noexcept;

    // Observers.

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TElement>
    constexpr Bool IsEmpty(const SpanT<TElement>& span) noexcept;

    /// \brief Get the number of elements in a span.
    template <typename TElement>
    constexpr Int Count(const SpanT<TElement>& span) noexcept;

    // Accessors.

    /// \brief Access the first element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Front(const SpanT<TElement>& span) noexcept;

    /// \brief Access the last element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Back(const SpanT<TElement>& span) noexcept;

    // Sub-spans.

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// If the sub-span would exceed original span bounds, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Subspan(const SpanT<TElement>& span, Int offset, Int count) noexcept;

    /// \brief Discard the first element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span) noexcept;

    /// \brief Discard the first count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Discard the last element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span) noexcept;

    /// \brief Discard the last count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the first elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> First(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the last elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Last(const SpanT<TElement>& span, Int count) noexcept;

    // Set operations.

    /// \brief Obtain the smallest span which includes all the elements in both lhs and rhs exclusively.
    /// If lhs and rhs are not contiguous the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Union(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept;

    /// \brief Obtain the smallest span which includes all the elements of lhs which are also contained in rhs and vice-versa.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Intersection(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept;

    /// \brief Reduce lhs from the back until no element of lhs is contained in rhs or lhs is exhausted.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> LeftDifference(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept;

    /// \brief Reduce lhs from the front until no element of lhs is contained in rhs or lhs is exhausted.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> RightDifference(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept;

    /// \brief Check whether rhs is contained inside lhs, that is if their intersection is rhs.
    template <typename TElement, typename UElement>
    constexpr Bool Contains(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether lhs and lhs overlap, that is if their intersection is non-empty.
    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    // Comparisons.

    /// \brief Check whether two spans are identical, i.e. they both refer to the same underlying byte span.
    template <typename TElement, typename UElement>
    constexpr Bool operator==(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are different, i.e. they both refer to different underlying byte span.
    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool Equals(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a member-wise prefix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasPrefix(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a member-wise suffix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether exists a subspan in lhs which compares equivalent to rhs.
    /// \return Returns true if lhs has a subset which compares equivalent to rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    // Algorithms.

    /// \brief Reduce lhs until rhs becomes the first element in lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Search(const SpanT<TElement>& lhs, const UElement& rhs) noexcept;

    /// \brief Reduce lhs until rhs becomes a member-wise prefix for lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Search(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    // Utilities.

    /// \brief Create a new span by deducing type from templates.
    template <typename TElement, typename UElement>
    auto MakeSpan(Pointer<TElement> begin, Pointer<UElement> end) noexcept;

    /// \brief Create a new span by deducing type from templates.
    /// If the two iterators do not represent a valid span, returns an empty span.
    template <typename TElement, typename UElement>
    auto MakeSafeSpan(Pointer<TElement> begin, Pointer<UElement> end) noexcept;

    /// \brief Convert a span to its read-only equivalent.
    template <typename TElement>
    [[nodiscard]] constexpr Span<RemoveConstT<TElement>> ReadOnly(const SpanT<TElement>& rhs);

    /// \brief Const rvalue reference deleted to disallow rvalue arguments.
    template <typename TElement>
    constexpr void ReadOnly(const SpanT<TElement>&&) = delete;

    /// \brief Convert a read-only span to its read-write equivalent.
    /// If rhs doesn't refer to an original read-write memory location, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr RWSpan<TElement> ReadWrite(const SpanT<const TElement>& rhs) noexcept;

    /// \brief Stream insertion for Spans.
    template <typename TElement>
    std::ostream& operator<<(std::ostream& lhs, const SpanT<TElement>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.
    // ===============

    template <typename TElement>
    template <typename TIterator>
    constexpr SpanT<TElement>::SpanT(TIterator begin, Int count) noexcept
        : data_((count > 0) ? begin : nullptr)
        , count_(count)
    {

    }

    template <typename TElement>
    template <typename TBegin, typename TEnd, typename>
    constexpr SpanT<TElement>::SpanT(TBegin begin, TEnd end) noexcept
        : SpanT(begin, ToInt(end - begin))
    {

    }

    template <typename TElement>
    template <typename UElement>
    constexpr SpanT<TElement>::SpanT(const SpanT<UElement>& rhs) noexcept
        : data_(static_cast<Pointer<TElement>>(rhs.GetData()))
        , count_(rhs.GetCount())
    {

    }

    template <typename TElement>
    template <typename UElement>
    constexpr SpanT<TElement>& SpanT<TElement>::operator=(const SpanT<UElement>& rhs) noexcept
    {
        data_ = static_cast<Pointer<TElement>>(rhs.GetData());
        count_ = rhs.GetCount();

        return *this;
    }

    template <typename TElement>
    constexpr SpanT<TElement>::operator Bool() const noexcept
    {
        return count_ > 0;
    }

    template <typename TElement>
    constexpr TElement& SpanT<TElement>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TElement>
    constexpr Int SpanT<TElement>::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TElement>
    constexpr Pointer<TElement> SpanT<TElement>::GetData() const noexcept
    {
        return data_;
    }

    // Non-member functions.
    // =====================

    // Iterators.

    template <typename TElement>
    constexpr Pointer<TElement> begin(const SpanT<TElement>& span) noexcept
    {
        return Begin(span);
    }

    template <typename TElement>
    constexpr Pointer<TElement> end(const SpanT<TElement>& span) noexcept
    {
        return End(span);
    }

    template <typename TElement>
    constexpr Pointer<TElement> Begin(const SpanT<TElement>& span) noexcept
    {
        return span.GetData();
    }

    template <typename TElement>
    constexpr Pointer<TElement> End(const SpanT<TElement>& span) noexcept
    {
        return Begin(span) + Count(span);
    }

    // Observers.

    template <typename TElement>
    constexpr Bool IsEmpty(const SpanT<TElement>& span) noexcept
    {
        return !span;
    }

    template <typename TElement>
    constexpr Int Count(const SpanT<TElement>& span) noexcept
    {
        return span.GetCount();
    }

    // Accessors.

    template <typename TElement>
    constexpr TElement& Front(const SpanT<TElement>& span) noexcept
    {
        return *Begin(span);
    }

    template <typename TElement>
    constexpr TElement& Back(const SpanT<TElement>& span) noexcept
    {
        return *(Begin(span) + Count(span) - 1);
    }

    // Sub-spans.

    template <typename TElement>
    constexpr SpanT<TElement> Subspan(const SpanT<TElement>& span, Int offset, Int count) noexcept
    {
        return { Begin(span) + offset, count };
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span) noexcept
    {
        return PopFront(span, 1);
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span, Int count) noexcept
    {
        return Subspan(span, count, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span) noexcept
    {
        return PopBack(span, 1);
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span, Int count) noexcept
    {
        return Subspan(span, 0, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> First(const SpanT<TElement>& span, Int count) noexcept
    {
        return PopBack(span, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> Last(const SpanT<TElement>& span, Int count) noexcept
    {
        return PopFront(span, Count(span) - count);
    }

    // Set operations.

    template <typename TElement>
    constexpr SpanT<TElement> Union(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept
    {
        auto begin = Math::Min(Begin(lhs), Begin(rhs));

        auto end = Math::Max(End(lhs), End(rhs));

        SYNTROPY_PRECONDITION(Count(SpanT<TElement>(begin, end)) <= (Count(lhs) + Count(rhs)));

        return { begin, end };
    }

    template <typename TElement>
    constexpr SpanT<TElement> Intersection(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept
    {
        auto begin = Math::Max(Begin(lhs), Begin(rhs));

        auto end = Math::Min(End(lhs), End(rhs));

        return MakeSpan(begin, end);
    }

    template <typename TElement>
    constexpr SpanT<TElement> LeftDifference(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept
    {
        auto begin = Begin(lhs);

        auto end = Math::Min(End(lhs), Begin(rhs));

        return MakeSpan(begin, end);
    }

    template <typename TElement>
    constexpr SpanT<TElement> RightDifference(const SpanT<TElement>& lhs, const SpanT<TElement>& rhs) noexcept
    {
        auto begin = Math::Max(Begin(lhs), End(rhs));

        auto end = End(lhs);

        return MakeSpan(begin, end);
    }

    template <typename TElement, typename UElement>
    constexpr Bool Contains(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        // Empty span are contained in every non-empty span.

        return lhs && (!rhs || (Begin(lhs) <= Begin(rhs)) && (End(rhs) <= End(lhs)));
    }

    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        return (Begin(lhs) < End(rhs)) && (Begin(rhs) < End(lhs));
    }

    // Comparisons.

    template <typename TElement, typename UElement>
    constexpr Bool operator==(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Bool Equals(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (Count(lhs) != Count(rhs))
        {
            return false;                           // Early out if the number of elements is different.
        }

        if constexpr (IsValidExpressionV<HasEqualityComparison, Pointer<TElement>, Pointer<UElement>>)
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

    template <typename TElement, typename UElement>
    constexpr Bool HasPrefix(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(First(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Last(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        return Count(Search(lhs, rhs)) == Count(rhs);
    }

    // Algorithms.

    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Search(const SpanT<TElement>& lhs, const UElement& rhs) noexcept
    {
        auto result = lhs;

        for (; result && (Front(result) != rhs); result = PopFront(result));

        return result;
    }

    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Search(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        auto result = lhs;

        for (; result && !HasPrefix(result, rhs); result = PopFront(result));

        return result;
    }

    // Utilities.

    template <typename TElement, typename UElement>
    auto MakeSpan(Pointer<TElement> begin, Pointer<UElement> end) noexcept
    {
        return SpanT<CommonTypeT<TElement, UElement>>{ begin, end };
    }

    template <typename TElement, typename UElement>
    auto MakeSafeSpan(Pointer<TElement> begin, Pointer<UElement> end) noexcept
    {
        if (begin <= end)
        {
            return MakeSpan(begin, end);
        }

        return {};
    }

    template <typename TElement>
    constexpr Span<RemoveConstT<TElement>> ReadOnly(const SpanT<TElement>& rhs)
    {
        return { rhs };
    }

    template <typename TElement>
    constexpr RWSpan<TElement> ReadWrite(const SpanT<const TElement>& rhs) noexcept
    {
        using TPointer = Pointer<TElement>;

        return { const_cast<TPointer>(Begin(rhs)), const_cast<TPointer>(End(rhs)) };
    }

    template <typename TElement>
    std::ostream& operator<<(std::ostream& lhs, const SpanT<TElement>& rhs)
    {
        lhs << "{";

        if (!IsEmpty(rhs))
        {
            for (auto&& element : PopBack(rhs))
            {
                lhs << element << ", ";
            }

            lhs << Back(rhs);
        }

        lhs << "}";

        return lhs;
    }

}
