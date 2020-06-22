
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

    /// \brief Represents a contiguous range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TElement>
    class Span
    {
        template <typename UElement>
        friend ObserverPtr<UElement> Begin(const Span<UElement>& span);

        template <typename UElement>
        friend ObserverPtr<UElement> End(const Span<UElement>& span);

        template <typename UElement>
        friend class Span;

    public:

        /// \brief Create an empty span.
        Span() = default;

        /// \brief Create a span given the first element and the span size.
        template <typename TFirst>
        Span(TFirst first, Int count);

        /// \brief Create a spawn given a pointer to both the first and past the last element in the span.
        template <typename TFirst, typename TLast, typename = EnableIfT<!IsConvertibleV<TLast, Int>>>
        Span(TFirst first, TLast last);

        /// \brief Default copy constructor.
        Span(const Span<TElement>& rhs) = default;

        /// \brief Default destructor.
        ~Span() = default;

        /// \brief Default copy assignment operator.
        Span& operator=(const Span<TElement>& rhs) = default;

        /// \brief Check whether the span is non-empty.
        operator Bool() const;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        TElement& operator[](Int index) const;

        /// \brief Get the number of elements in the span.
        Int GetCount() const;

    private:

        /// \brief Pointer to the first element in the rage.
        ObserverPtr<TElement> begin_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* ITERATORS                                                            */
    /************************************************************************/

    /// \brief Get an iterator to the first element in the span.
    /// Function needed for range-based loops.
    template <typename TElement>
    inline ObserverPtr<TElement> begin(const Span<TElement>& span);

    /// \brief Get an iterator past the last element in the span.
    /// Function needed for range-based loops.
    template <typename TElement>
    inline ObserverPtr<TElement> end(const Span<TElement>& span);

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether two spans are not element-wise equivalent.
    template <typename TElement, typename UElement>
    bool operator!=(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether a span is empty.
    template <typename TElement>
    Bool IsEmpty(const Span<TElement>& span);

    /// \brief Get the number of elements in the span.
    template <typename TElement>
    Int Count(const Span<TElement>& span);

    /// \brief Get an iterator to the first element of a span.
    template <typename TElement>
    TElement* Begin(const Span<TElement>& span);

    /// \brief Get an iterator past the last element of a span.
    template <typename TElement>
    TElement* End(const Span<TElement>& span);

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

    /// \brief Shrink a span by moving its start forward by one element.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopFront(const Span<TElement>& span);

    /// \brief Shrink a span by moving its start forward by some elements.
    /// If this method would cause the start to exceed the span range, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopFront(const Span<TElement>& span, Int count);

    /// \brief Shrink a span by moving its end backward by one element.
    /// If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopBack(const Span<TElement>& span);

    /// \brief Shrink a span by moving its end backward by some elements.
    /// If this method would cause the end to exceed the span range, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> PopBack(const Span<TElement>& span, Int count);

    /// \brief Obtain a span consisting of the first elements of another span.
    /// If this method would cause the end to exceed the span range, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> First(const Span<TElement>& span, Int count);

    /// \brief Obtain a span consisting of the last elements of another span.
    /// If this method would cause the end to exceed the span range, the behavior of this method is undefined.
    template <typename TElement>
    Span<TElement> Last(const Span<TElement>& span, Int count);

    /// \brief Check whether two spans are identical.
    /// \return Returns true if both lhs and rhs refer to the same memory region, returns false otherwise.
    template <typename TElement>
    Bool EqualsStrong(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether rhs is a a member-wise prefix of lhs, using a strongly equivalence metric between elements of both spans.
    template <typename TElement>
    Bool HasPrefixStrong(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether rhs is a a member-wise suffix of lhs, using a strongly equivalence metric between elements of both spans.
    template <typename TElement>
    Bool HasSuffixStrong(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether exists a subset in lhs which strongly compares equal to rhs.
    /// \return Returns true if lhs has a subset which strongly compares equal to rhs, returns false otherwise.
    template <typename TElement>
    Bool ContainsStrong(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Reduce lhs until rhs becomes a strong prefix for lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement>
    Span<TElement> SearchStrong(const Span<TElement>& lhs, const Span<TElement>& rhs);

    /// \brief Check whether two spans are equivalent, converting elements in rhs if necessary.
    /// \remarks This method comes with additional overhead, prefer EqualsStrong where possible.
    /// \return Returns true if each element in lhs weakly compares equal to the element in the same position in rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    Bool EqualsWeak(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether rhs is a a member-wise prefix of lhs, using a weak equivalence metric between elements of both spans.
    /// \remarks This method comes with additional overhead, prefer HasPrefixStrong where possible.
    template <typename TElement, typename UElement>
    Bool HasPrefixWeak(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether rhs is a a member-wise suffix of lhs, using a weak equivalence metric between elements of both spans.
    /// \remarks This method comes with additional overhead, prefer HasSuffixStrong where possible.
    template <typename TElement, typename UElement>
    Bool HasSuffixWeak(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Check whether exists a subset in lhs which weakly compares equal to rhs.
    /// \remarks This method comes with additional overhead, prefer ContainsStrong where possible.
    /// \return Returns true if lhs has a subset which weakly compares equal to rhs, returns false otherwise.
    template <typename TElement, typename UElement>
    Bool ContainsWeak(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /// \brief Reduce lhs until rhs becomes a weak prefix for lhs or lhs is exhausted.
    /// \remarks This method comes with additional overhead, prefer SearchStrong where possible.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    Span<TElement> SearchWeak(const Span<TElement>& lhs, const Span<UElement>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.

    template <typename TElement>
    template <typename TFirst>
    inline Span<TElement>::Span(TFirst first, Int count)
        : begin_(first)
        , count_(count)
    {
        static_assert(!IsConvertibleV<TFirst, TElement>, "TFirst and TElement are uncorrelated.");
    }

    template <typename TElement>
    template <typename TFirst, typename TLast, typename>
    inline Span<TElement>::Span(TFirst first, TLast last)
        : begin_(first)
        , count_(ToInt(last - first))
    {
        static_assert(!IsConvertibleV<TFirst, TElement>, "TFirst and TElement are uncorrelated.");
    }

    template <typename TElement>
    inline Span<TElement>::operator Bool() const
    {
        return count_ > 0;
    }

    template <typename TElement>
    inline  TElement& Span<TElement>::operator[](Int index) const
    {
        return begin_[index];
    }

    template <typename TElement>
    inline Int Span<TElement>::GetCount() const
    {
        return count_;
    }

    // Iterators.

    template <typename TElement>
    inline ObserverPtr<TElement> begin(const Span<TElement>& span)
    {
        return Begin(span);
    }

    template <typename TElement>
    inline ObserverPtr<TElement> end(const Span<TElement>& span)
    {
        return End(span);
    }

    // Non-member functions.

    template <typename TElement, typename UElement>
    inline bool operator==(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return EqualsWeak(lhs, rhs);
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
    inline TElement* Begin(const Span<TElement>& span)
    {
        return span.begin_;
    }

    template <typename TElement>
    inline TElement* End(const Span<TElement>& span)
    {
        return span.begin_ + span.count_;
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
        return { Begin(span) + offset, count };
    }

    template <typename TElement>
    inline Span<TElement> PopFront(const Span<TElement>& span)
    {
        return PopFront(span, 1);
    }

    template <typename TElement>
    inline Span<TElement> PopFront(const Span<TElement>& span, Int count)
    {
        return { Begin(span) + count, End(span) };
    }

    template <typename TElement>
    inline Span<TElement> PopBack(const Span<TElement>& span)
    {
        return PopBack(span, 1);
    }

    template <typename TElement>
    inline Span<TElement> PopBack(const Span<TElement>& span, Int count)
    {
        return { Begin(span), End(span) - count };
    }

    template <typename TElement>
    inline Span<TElement> First(const Span<TElement>& span, Int count)
    {
        return { Begin(span), count };
    }

    template <typename TElement>
    inline Span<TElement> Last(const Span<TElement>& span, Int count)
    {
        return { End(span) - count, End(span) };
    }

    template <typename TElement>
    inline Bool EqualsStrong(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        return (!lhs && !rhs) || (Begin(lhs) == Begin(rhs)) && (End(lhs) == End(rhs));
    }

    template <typename TElement>
    inline Bool HasPrefixStrong(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        return (Count(lhs) >= Count(rhs)) && EqualsStrong(First(lhs, Count(rhs)), rhs);
    }

    template <typename TElement>
    inline Bool HasSuffixStrong(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        return (Count(lhs) >= Count(rhs)) && EqualsStrong(Last(lhs, Count(rhs)), rhs);
    }

    template <typename TElement>
    inline Bool ContainsStrong(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        return (Begin(lhs) <= Begin(rhs)) && (End(rhs) <= End(lhs));
    }

    template <typename TElement>
    inline Span<TElement> SearchStrong(const Span<TElement>& lhs, const Span<TElement>& rhs)
    {
        auto span = lhs;

        for (; span && !HasPrefixStrong(span, rhs); span = PopFront(span));

        return span;
    }

    template <typename TElement, typename UElement>
    Bool EqualsWeak(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        auto lhs_copy = lhs;
        auto rhs_copy = rhs;

        if (lhs_copy.GetCount() == rhs_copy.GetCount())
        {
            for (; (lhs_copy) && (rhs_copy) && (Front(lhs_copy) == Front(rhs_copy));)
            {
                lhs_copy = PopFront(lhs_copy);
                rhs_copy = PopFront(rhs_copy);
            }
        }

        return IsEmpty(lhs_copy) && IsEmpty(rhs_copy);
    }

    template <typename TElement, typename UElement>
    inline Bool HasPrefixWeak(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return (Count(lhs) >= Count(rhs)) && EqualsWeak(First(lhs, Count(rhs)), rhs);
    }

    template <typename TElement, typename UElement>
    inline Bool HasSuffixWeak(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return (Count(lhs) >= Count(rhs)) && EqualsWeak(Last(lhs, Count(rhs)), rhs);
    }

    template <typename TElement, typename UElement>
    inline Bool ContainsWeak(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        return !IsEmpty(SearchWeak(lhs, rhs));
    }

    template <typename TElement, typename UElement>
    inline Span<TElement> SearchWeak(const Span<TElement>& lhs, const Span<UElement>& rhs)
    {
        auto span = lhs;

        for (; span && !HasPrefixWeak(span, rhs); span = PopFront(span));

        return span;
    }

}
