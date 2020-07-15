
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"

namespace syntropy
{
    /************************************************************************/
    /* SPAN <ELEMENT>                                                       */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TElement>
    class Span
    {
    public:

        /// \brief Create an empty span.
        constexpr Span() noexcept = default;

        /// \brief Create a span given a pointer to the first element and the number of elements.
        template <typename TBegin>
        constexpr Span(TBegin begin, Int count) noexcept;

        /// \brief Create a span given a pointer to the first element and a pointer past the last element.
        template <typename TBegin, typename TEnd, typename = EnableIfT<!IsConvertibleV<TEnd, Int>>>
        constexpr Span(TBegin begin, TEnd end) noexcept;

        /// \brief Copy constructor.
        template <typename UElement>
        constexpr Span(const Span<UElement>& rhs) noexcept;

        /// \brief Default destructor.
        ~Span() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UElement>
        constexpr Span& operator=(const Span<UElement>& rhs) noexcept;

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

    /// \brief Alias for a span of constant elements.
    template <typename TType>
    using ReadOnlySpan = Span<const TType>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Iterators.

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> begin(const Span<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> end(const Span<TElement>& span) noexcept;

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr Pointer<TElement> Begin(const Span<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr Pointer<TElement> End(const Span<TElement>& span) noexcept;

    // Observers.

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TElement>
    constexpr Bool IsEmpty(const Span<TElement>& span) noexcept;

    /// \brief Get the number of elements in a span.
    template <typename TElement>
    constexpr Int Count(const Span<TElement>& span) noexcept;

    // Accessors.

    /// \brief Access the first element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Front(const Span<TElement>& span) noexcept;

    /// \brief Access the last element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Back(const Span<TElement>& span) noexcept;

    // Sub-spans.

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// If the sub-span would exceed original span bounds, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> Subspan(const Span<TElement>& span, Int offset, Int count) noexcept;

    /// \brief Discard the first element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> PopFront(const Span<TElement>& span) noexcept;

    /// \brief Discard the first count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> PopFront(const Span<TElement>& span, Int count) noexcept;

    /// \brief Discard the last element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> PopBack(const Span<TElement>& span) noexcept;

    /// \brief Discard the last count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> PopBack(const Span<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the first elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> First(const Span<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the last elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> Last(const Span<TElement>& span, Int count) noexcept;

    // Set operations.

    /// \brief Obtain the smallest span which includes all the elements in both lhs and rhs exclusively.
    /// If lhs and rhs are not contiguous the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> Union(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept;

    /// \brief Obtain the smallest span which includes all the elements of lhs which are also contained in rhs and vice-versa.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> Intersection(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept;

    /// \brief Reduce lhs from the back until no element of lhs is contained in rhs or lhs is exhausted.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> LeftDifference(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept;

    /// \brief Reduce lhs from the front until no element of lhs is contained in rhs or lhs is exhausted.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> RightDifference(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept;

    /// \brief Check whether rhs is contained inside lhs, that is if their intersection is rhs.
    template <typename TElement, typename UElement>
    constexpr Bool Contains(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether lhs and lhs overlap, that is if their intersection is non-empty.
    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    // Comparisons.

    /// \brief Check whether two spans are identical, i.e. they both refer to the same underlying memory span.
    template <typename TElement, typename UElement>
    constexpr Bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are different, i.e. they both refer to different underlying memory span.
    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool Equals(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a member-wise prefix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasPrefix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a member-wise suffix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether exists a subspan in lhs which compares equivalent to rhs.
    /// \return Returns true if lhs has a subset which compares equivalent to rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    // Algorithms.

    /// \brief Reduce lhs until rhs becomes the first element in lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr Span<TElement> Search(const Span<TElement>& lhs, const UElement& rhs) noexcept;

    /// \brief Reduce lhs until rhs becomes a member-wise prefix for lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    // Utilities.

    /// \brief Stream insertion for Spans.
    template <typename TElement>
    std::ostream& operator<<(std::ostream& lhs, const Span<TElement>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.
    // ===============

    template <typename TElement>
    template <typename TBegin>
    constexpr Span<TElement>::Span(TBegin begin, Int count) noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TElement>
    template <typename TBegin, typename TEnd, typename>
    constexpr Span<TElement>::Span(TBegin begin, TEnd end) noexcept
        : Span(begin, ToInt(end - begin))
    {

    }

    template <typename TElement>
    template <typename UElement>
    constexpr Span<TElement>::Span(const Span<UElement>& rhs) noexcept
        : data_(rhs.data_)
        , count_(rhs.count_)
    {

    }

    template <typename TElement>
    template <typename UElement>
    constexpr Span<TElement>& Span<TElement>::operator=(const Span<UElement>& rhs) noexcept
    {
        data_ = rhs.data_;
        count_ = rhs.count_;

        return *this;
    }

    template <typename TElement>
    constexpr Span<TElement>::operator Bool() const noexcept
    {
        return count_ > 0;
    }

    template <typename TElement>
    constexpr TElement& Span<TElement>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TElement>
    constexpr Int Span<TElement>::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TElement>
    constexpr Pointer<TElement> Span<TElement>::GetData() const noexcept
    {
        return data_;
    }

    // Non-member functions.
    // =====================

    // Iterators.

    template <typename TElement>
    constexpr Pointer<TElement> begin(const Span<TElement>& span) noexcept
    {
        return Begin(span);
    }

    template <typename TElement>
    constexpr Pointer<TElement> end(const Span<TElement>& span) noexcept
    {
        return End(span);
    }

    template <typename TElement>
    constexpr Pointer<TElement> Begin(const Span<TElement>& span) noexcept
    {
        return span.GetData();
    }

    template <typename TElement>
    constexpr Pointer<TElement> End(const Span<TElement>& span) noexcept
    {
        return span.GetData() + span.GetCount();
    }

    // Observers.

    template <typename TElement>
    constexpr Bool IsEmpty(const Span<TElement>& span) noexcept
    {
        return !span;
    }

    template <typename TElement>
    constexpr Int Count(const Span<TElement>& span) noexcept
    {
        return span.GetCount();
    }

    // Accessors.

    template <typename TElement>
    constexpr TElement& Front(const Span<TElement>& span) noexcept
    {
        return span[0];
    }

    template <typename TElement>
    constexpr TElement& Back(const Span<TElement>& span) noexcept
    {
        return span[span.GetCount() - 1];
    }

    // Sub-spans.

    template <typename TElement>
    constexpr Span<TElement> Subspan(const Span<TElement>& span, Int offset, Int count) noexcept
    {
        return { &span[offset], count };
    }

    template <typename TElement>
    constexpr Span<TElement> PopFront(const Span<TElement>& span) noexcept
    {
        return PopFront(span, 1);
    }

    template <typename TElement>
    constexpr Span<TElement> PopFront(const Span<TElement>& span, Int count) noexcept
    {
        return { &span[count], span.GetCount() - count };
    }

    template <typename TElement>
    constexpr Span<TElement> PopBack(const Span<TElement>& span) noexcept
    {
        return PopBack(span, 1);
    }

    template <typename TElement>
    constexpr Span<TElement> PopBack(const Span<TElement>& span, Int count) noexcept
    {
        return { &span[0], span.GetCount() - count };
    }

    template <typename TElement>
    constexpr Span<TElement> First(const Span<TElement>& span, Int count) noexcept
    {
        return { &span[0], count };
    }

    template <typename TElement>
    constexpr Span<TElement> Last(const Span<TElement>& span, Int count) noexcept
    {
        return { &span[span.GetCount() - count], count };
    }

    // Set operations.

    template <typename TElement>
    constexpr Span<TElement> Union(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept
    {
        auto begin = Math::Min(Begin(lhs), Begin(rhs));
        auto end = Math::Max(End(lhs), End(rhs));

        auto union_span = Span<TElement>(begin, end);

        SYNTROPY_PRECONDITION(union_span.GetCount() <= (lhs.GetCount() + rhs.GetCount()));

        return union_span;
    }

    template <typename TElement>
    constexpr Span<TElement> Intersection(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept
    {
        auto begin = Math::Max(Begin(lhs), Begin(rhs));
        auto end = Math::Min(End(lhs), End(rhs));

        if (begin <= end)
        {
            return { begin, end };
        }

        return {};
    }

    template <typename TElement>
    constexpr Span<TElement> LeftDifference(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept
    {
        auto begin = Begin(lhs);
        auto end = Math::Min(End(lhs), Begin(rhs));

        if (begin <= end)
        {
            return { begin, end };
        }

        return {};
    }

    template <typename TElement>
    constexpr Span<TElement> RightDifference(const Span<TElement>& lhs, const Span<TElement>& rhs) noexcept
    {
        auto begin = Math::Max(Begin(lhs), End(rhs));
        auto end = End(lhs);

        if (begin <= end)
        {
            return { begin, end };
        }

        return {};
    }

    template <typename TElement, typename UElement>
    constexpr Bool Contains(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        static_assert(IsSameV<StripT<TElement>, StripT<UElement>>, "TElement and UElement must refer to the same underlying type");

        // Empty spans do not contain any other span.

        if (!lhs)
        {
            return false;
        }

        // Empty spans are contained in any other span, except empty spans.

        if (!rhs)
        {
            return true;
        }

        // Test span boundaries.

        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return ((lhs_begin <= rhs_begin) && (lhs_end >= rhs_end));
    }

    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        static_assert(IsSameV<StripT<TElement>, StripT<UElement>>, "TElement and UElement must refer to the same underlying type");

        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (lhs_begin < rhs_end) && (rhs_begin < lhs_end);
    }

    // Comparisons.

    template <typename TElement, typename UElement>
    constexpr Bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Bool Equals(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        // Early out if the number of elements is different.

        if (Count(lhs) != Count(rhs))
        {
            return false;
        }

        // Early out if the underlying data is the same.

        auto lhs_data = lhs.GetData();
        auto rhs_data = rhs.GetData();

        if constexpr (IsSameV<StripT<TElement>, StripT<UElement>>)
        {
            if (lhs_data == rhs_data)
            {
                return true;
            }
        }

        // Consume both spans looking for differences.

        for (auto lhs_end = lhs_data + Count(lhs); lhs_data != lhs_end; ++lhs_data, ++rhs_data)
        {
            if (*lhs_data != *rhs_data)
            {
                return false;
            }
        }

        return true;
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasPrefix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(First(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Last(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return Count(Search(lhs, rhs)) >= Count(rhs);
    }

    // Algorithms.

    template <typename TElement, typename UElement>
    constexpr Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        auto result = lhs;

        for (; result && !HasPrefix(result, rhs); result = PopFront(result));

        return result;
    }

    // Utility.

    template <typename TElement>
    std::ostream& operator<<(std::ostream& lhs, const Span<TElement>& rhs)
    {
        lhs << "{";

        for (auto&& element : rhs)
        {
            lhs << element << ((&element != &Back(rhs)) ? ", " : "");
        }

        lhs << "}";

        return lhs;
    }
}
