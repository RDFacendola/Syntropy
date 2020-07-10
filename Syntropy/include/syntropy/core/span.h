
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/language/type_traits.h"
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

        /// \brief Create a span given an iterator to the first element and the span size.
        template <typename TBegin>
        constexpr Span(TBegin begin, Int count) noexcept;

        /// \brief Create a span given an iterator to the first element and an iterator past the last element.
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

        /// \brief Access the underlying buffer or nullptr if the span is empty.
        constexpr ObserverPtr<TElement> GetData() const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        ObserverPtr<TElement> data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  ObserverPtr<TElement> begin(const Span<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  ObserverPtr<TElement> end(const Span<TElement>& span) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are not element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether a span is empty.
    template <typename TElement>
    constexpr Bool IsEmpty(const Span<TElement>& span) noexcept;

    /// \brief Get the number of elements in a span.
    template <typename TElement>
    constexpr Int Count(const Span<TElement>& span) noexcept;

    /// \brief Access the first element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Front(const Span<TElement>& span) noexcept;

    /// \brief Access the last element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr TElement& Back(const Span<TElement>& span) noexcept;

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// If the sub-span would exceed original span bounds, the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> Subspan(const Span<TElement>& span, Int offset, Int count) noexcept;

    /// \brief Discard the first element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> PopFront(const Span<TElement>& span) noexcept;

    /// \brief Discard the first count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> PopFront(const Span<TElement>& span, Int count) noexcept;

    /// \brief Discard the last element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> PopBack(const Span<TElement>& span) noexcept;

    /// \brief Discard the last count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> PopBack(const Span<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the first elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> First(const Span<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the last elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    constexpr Span<TElement> Last(const Span<TElement>& span, Int count) noexcept;

    /// \brief Check whether two spans are identical (ie: they both refer to the same memory).
    template <typename TElement, typename UElement>
    constexpr Bool AreIdentical(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool AreEquivalent(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a a member-wise prefix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasPrefix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is a a member-wise suffix of lhs.
    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether exists a subspan in lhs which compares equivalent to rhs.
    /// \return Returns true if lhs has a subset which compares equivalent to rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Reduce lhs until rhs becomes a member-wise prefix for lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether rhs refers to a memory location identical or contained inside lhs.
    template <typename TElement, typename UElement>
    constexpr Bool Contains(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /// \brief Check whether lhs and lhs overlaps the same memory location.
    /// Empty spans are not considered to be overlapping with any other span.
    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.

    template <typename TElement>
    template <typename TBegin>
    constexpr Span<TElement>::Span(TBegin begin, Int count) noexcept
        : data_((count > 0) ? &(*begin) : nullptr)
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
    constexpr ObserverPtr<TElement> Span<TElement>::GetData() const noexcept
    {
        return data_;
    }

    // Non-member functions.

    template <typename TElement>
    constexpr ObserverPtr<TElement> begin(const Span<TElement>& span) noexcept
    {
        return span.GetData();
    }

    template <typename TElement>
    constexpr ObserverPtr<TElement> end(const Span<TElement>& span) noexcept
    {
        return span.GetData() + span.count_;
    }

    template <typename TElement, typename UElement>
    constexpr Bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return AreEquivalent(lhs, rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

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

    template <typename TElement, typename UElement>
    constexpr Bool AreIdentical(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    template <typename TElement, typename UElement>
    constexpr Bool AreEquivalent(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        // Early out if the number of elements is different.

        if (Count(lhs) != Count(rhs))
        {
            return false;
        }

        // Early out if the underlying data is the same.

        auto lhs_data = lhs.GetData();
        auto rhs_data = rhs.GetData();

        if constexpr (IsSameV<std::decay<TElement>, std::decay<UElement>>)
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

        return (lhs_count >= rhs_count) && (First(lhs, rhs_count) == rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Last(lhs, rhs_count) == rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        return Count(Search(lhs, rhs)) >= Count(rhs);
    }

    template <typename TElement, typename UElement>
    constexpr Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        auto result = lhs;

        for (; result && !HasPrefix(result, rhs); result = PopFront(result));

        return result;
    }

    template <typename TElement, typename UElement>
    constexpr Bool Contains(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        static_assert(IsSameV<std::decay<TElement>, std::decay<UElement>>, "TElement and UElement must refer to the same underlying type");

        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (!rhs) || ((lhs_begin <= rhs_begin) && (lhs_end >= rhs_end));
    }

    template <typename TElement, typename UElement>
    constexpr Bool Overlaps(const Span<TElement>& lhs, const Span<UElement>& rhs) noexcept
    {
        static_assert(IsSameV<std::decay<TElement>, std::decay<UElement>>, "TElement and UElement must refer to the same underlying type");

        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (lhs_begin < rhs_end) && (rhs_begin < lhs_end);
    }

}
