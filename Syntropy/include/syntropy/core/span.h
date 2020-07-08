
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
        Span() = default;

        /// \brief Create a span given an iterator to the first element and the span size.
        template <typename TBegin>
        Span(TBegin begin, Int count);

        /// \brief Create a span given an iterator to the first element and an iterator past the last element.
        template <typename TBegin, typename TEnd, typename = EnableIfT<!IsConvertibleV<TEnd, Int>>>
        Span(TBegin begin, TEnd end);

        /// \brief Default copy constructor.
        Span(const Span<TElement>& rhs) = default;

        /// \brief Default move constructor.
        Span(Span<TElement>&& rhs) = default;

        /// \brief Default destructor.
        ~Span() = default;

        /// \brief Default copy assignment operator.
        Span& operator=(const Span<TElement>& rhs) = default;

        /// \brief Default move assignment operator.
        Span& operator=(Span<TElement>&& rhs) = default;

        /// \brief Check whether the span is non-empty.
        operator Bool() const;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        TElement& operator[](Int index) const;

        /// \brief Get the number of elements in the span.
        Int GetCount() const;

        /// \brief Access the underlying buffer or nullptr if the span is empty.
        ObserverPtr<TElement> GetData() const;

    private:

        /// \brief Pointer to the first element in the rage.
        ObserverPtr<TElement> data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    inline ObserverPtr<TElement> begin(const Span<TElement>& span);

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    inline ObserverPtr<TElement> end(const Span<TElement>& span);

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether two spans are not element-wise equivalent.
    template <typename TElement, typename UElement>
    bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether a span is empty.
    template <typename TElement>
    Bool IsEmpty(const Span<TElement>& span);

    /// \brief Get the number of elements in a span.
    template <typename TElement>
    Int Count(const Span<TElement>& span);

    /// \brief Access the first element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    TElement& Front(const Span<TElement>& span);

    /// \brief Access the last element in a span.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    TElement& Back(const Span<TElement>& span);

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// If the sub-span would exceed original span bounds, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> Subspan(const Span<TElement>& span, Int offset, Int count);

    /// \brief Discard the first element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopFront(const Span<TElement>& span);

    /// \brief Discard the first count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopFront(const Span<TElement>& span, Int count);

    /// \brief Discard the last element in a span and return the resulting subspan.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopBack(const Span<TElement>& span);

    /// \brief Discard the last count-elements in a span and return the resulting subspan.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopBack(const Span<TElement>& span, Int count);

    /// \brief Obtain a span consisting of the first elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> First(const Span<TElement>& span, Int count);

    /// \brief Obtain a span consisting of the last elements of another span.
    /// If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> Last(const Span<TElement>& span, Int count);

    /// \brief Check whether two spans are identical (ie: they both refer to the same memory).
    template <typename TElement>
    Bool AreIdentical(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    Bool AreEquivalent(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether rhs is a a member-wise prefix of lhs.
    template <typename TElement, typename UElement>
    Bool HasPrefix(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether rhs is a a member-wise suffix of lhs.
    template <typename TElement, typename UElement>
    Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether exists a subspan in lhs which compares equivalent to rhs.
    /// \return Returns true if lhs has a subset which compares equivalent to rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Reduce lhs until rhs becomes a member-wise prefix for lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether rhs refers to a memory location identical or contained inside lhs.
    template <typename TElement>
    Bool Contains(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether lhs and lhs overlaps the same memory location.
    /// Empty spans are not considered to be overlapping with any other span.
    template <typename TElement>
    Bool IsOverlapping(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.

    template <typename TElement>
    template <typename TBegin>
    inline Span<TElement>::Span(TBegin begin, Int count)
        : data_((count > 0) ? &(*begin) : nullptr)
        , count_(count)
    {

    }

    template <typename TElement>
    template <typename TBegin, typename TEnd, typename>
    inline Span<TElement>::Span(TBegin begin, TEnd end)
        : Span(begin, ToInt(end - begin))
    {

    }

    template <typename TElement>
    inline Span<TElement>::operator Bool() const
    {
        return count_ > 0;
    }

    template <typename TElement>
    inline  TElement& Span<TElement>::operator[](Int index) const
    {
        return data_[index];
    }

    template <typename TElement>
    inline Int Span<TElement>::GetCount() const
    {
        return count_;
    }

    template <typename TElement>
    inline ObserverPtr<TElement> Span<TElement>::GetData() const
    {
        return data_;
    }

    template <typename TElement>
    inline ObserverPtr<TElement> begin(const Span<TElement>& span)
    {
        return span.GetData();
    }

    template <typename TElement>
    inline ObserverPtr<TElement> end(const Span<TElement>& span)
    {
        return span.GetData() + span.count_;
    }

    // Non-member functions.

    template <typename TElement, typename UElement>
    inline bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return AreEquivalent(lhs, rhs);
    }

    template <typename TElement, typename UElement>
    inline bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TElement>
    inline Bool IsEmpty(const Span<TElement>& span)
    {
        return !span;
    }

    template <typename TElement>
    inline Int Count(const Span<TElement>& span)
    {
        return span.GetCount();
    }

    template <typename TElement>
    inline TElement& Front(const Span<TElement>& span)
    {
        return span[0];
    }

    template <typename TElement>
    inline TElement& Back(const Span<TElement>& span)
    {
        return span[span.GetCount() - 1];
    }

    template <typename TElement>
    inline Span<TElement> Subspan(const Span<TElement>& span, Int offset, Int count)
    {
        return { &span[offset], count };
    }

    template <typename TElement>
    inline Span<TElement> PopFront(const Span<TElement>& span)
    {
        return PopFront(span, 1);
    }

    template <typename TElement>
    inline Span<TElement> PopFront(const Span<TElement>& span, Int count)
    {
        return { &span[count], span.GetCount() - count };
    }

    template <typename TElement>
    inline Span<TElement> PopBack(const Span<TElement>& span)
    {
        return PopBack(span, 1);
    }

    template <typename TElement>
    inline Span<TElement> PopBack(const Span<TElement>& span, Int count)
    {
        return { &span[0], span.GetCount() - count };
    }

    template <typename TElement>
    inline Span<TElement> First(const Span<TElement>& span, Int count)
    {
        return { &span[0], count };
    }

    template <typename TElement>
    inline Span<TElement> Last(const Span<TElement>& span, Int count)
    {
        return { &span[span.GetCount() - count], count };
    }

    template <typename TElement>
    inline Bool AreIdentical(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    template <typename TElement, typename UElement>
    Bool AreEquivalent(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        // Early out if the number of elements is different.

        if (Count(lhs) != Count(rhs))
        {
            return false;
        }

        // Early out if the underlying data is the same.

        auto lhs_data = lhs.GetData();
        auto rhs_data = rhs.GetData();

        if constexpr (IsSameV<TElement, UElement>)
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
    inline Bool HasPrefix(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (First(lhs, rhs_count) == rhs);
    }

    template <typename TElement, typename UElement>
    inline Bool HasSuffix(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Last(lhs, rhs_count) == rhs);
    }

    template <typename TElement, typename UElement>
    inline Bool HasSubspan(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return Count(Search(lhs, rhs)) >= Count(rhs);
    }

    template <typename TElement, typename UElement>
    inline Span<TElement> Search(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        auto result = lhs;

        for (; result && !HasPrefix(result, rhs); result = PopFront(result));

        return result;
    }

    template <typename TElement>
    inline Bool Contains(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (!rhs) || ((lhs_begin <= rhs_begin) && (lhs_end >= rhs_end));
    }

    template <typename TElement>
    inline Bool IsOverlapping(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        auto lhs_begin = lhs.GetData();
        auto lhs_end = lhs_begin + Count(lhs);

        auto rhs_begin = rhs.GetData();
        auto rhs_end = rhs_begin + Count(rhs);

        return (lhs_begin < rhs_end) && (rhs_begin < lhs_end);
    }

}
