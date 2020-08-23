
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/language/tuple.h"
#include "syntropy/language/language.h"
#include "syntropy/math/math.h"

namespace Syntropy
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
        template <typename TBegin, typename TEnd, typename = EnableIfT<!Traits::IsConvertible<TEnd, Int>>>
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
        constexpr PointerT<TElement> GetData() const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        PointerT<TElement> data_{ nullptr };

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
    template <typename TType, typename = EnableIfT<!Traits::IsConst<TType>>>
    using RWSpan = SpanT<TType>;

    /// \brief Alias for a common type between two or more spans.
    template <typename... TElements>
    using CommonSpan = SpanT<Traits::RemovePointer<CommonTypeT<Traits::AddPointer<TElements>...>>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Iterators.

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  PointerT<TElement> begin(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  PointerT<TElement> end(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr PointerT<TElement> Begin(const SpanT<TElement>& span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr PointerT<TElement> End(const SpanT<TElement>& span) noexcept;

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
    /// \remarks Accessing the first element of an empty span results in undefined behavior.
    template <typename TElement>
    [[nodiscard]]  constexpr TElement& Front(const SpanT<TElement>& span) noexcept;

    /// \brief Access the last element in a span.
    /// \remarks Accessing the last element of an empty span results in undefined behavior.
    template <typename TElement>
    [[nodiscard]]  constexpr TElement& Back(const SpanT<TElement>& span) noexcept;

    /// \brief Obtain a span consisting of the first elements of another span.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Front(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Obtain a span consisting of the last elements of another span.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Back(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> Select(const SpanT<TElement>& span, Int offset, Int count) noexcept;

    /// \brief Discard the first count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span, Int count = 1) noexcept;

    /// \brief Discard the last count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span, Int count = 1) noexcept;

    /// \brief Slice a span returning the first element and a subspan to the remaining ones.
    /// \remarks If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Tuple<TElement&, SpanT<TElement>> SliceFront(const SpanT<TElement>& span) noexcept;

    /// \brief Slice a span returning the last element and a subspan to the remaining ones.
    /// \remarks If the span is empty the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Tuple<TElement&, SpanT<TElement>> SliceBack(const SpanT<TElement>& span) noexcept;

    /// \brief Slice a span returning a subspan to the first count-elements and a subspan to the remaining ones.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Tuple<SpanT<TElement>, SpanT<TElement>> SliceFront(const SpanT<TElement>& span, Int count) noexcept;

    /// \brief Slice a span returning a subspan to the last-count elements and a subspan to the remaining ones.
    /// \remarks If this method would cause any of the two subspans to exceed the original span, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr Tuple<SpanT<TElement>, SpanT<TElement>> SliceBack(const SpanT<TElement>& span, Int count) noexcept;

    // Set operations.

    /// \brief Extend lhs to the smallest smallest span which includes both itself and rhs.
    /// \remarks This function may introduce elements that do not belong to either lhs and rhs. If those elements refer
    ///          to an invalid memory region, the behavior of this method is undefined.
    template <typename TElement, typename UElement>
    [[nodiscard]] constexpr CommonSpan<TElement, UElement> Union(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Reduce lhs to the smallest span shared between itself and rhs.
    template <typename TElement, typename UElement>
    [[nodiscard]] constexpr CommonSpan<TElement, UElement> Intersection(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Reduce lhs from the back until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TElement, typename UElement>
    [[nodiscard]] constexpr CommonSpan<TElement, UElement> DifferenceFront(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Reduce lhs from the front until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TElement, typename UElement>
    [[nodiscard]] constexpr CommonSpan<TElement, UElement> DifferenceBack(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether rhs is identical to any subset in lhs.
    template <typename TElement, typename UElement>
    constexpr Bool Contains(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    // Comparisons.

    /// \brief Check whether lhs and rhs are identical.
    template <typename TElement, typename UElement>
    constexpr Bool operator==(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether lhs and rhs are different.
    template <typename TElement, typename UElement>
    constexpr Bool operator!=(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TElement, typename UElement>
    constexpr Bool Equals(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether lhs starts with rhs.
    /// \remarks If lhs starts with rhs, return true, otherwise return false.
    template <typename TElement, typename UElement>
    constexpr Bool StartsWith(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Check whether lhs ends with lhs.
    /// \remarks If lhs ends with rhs, return true, otherwise return false.
    template <typename TElement, typename UElement>
    constexpr Bool EndsWith(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Reduce lhs until rhs becomes the first element in lhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Find(const SpanT<TElement>& lhs, const UElement& rhs) noexcept;

    /// \brief Reduce lhs until lhs starts with rhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    ///         If rhs is empty lhs is returned instead.
    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Find(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    // Utilities.

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin>
    constexpr SpanT<TBegin> MakeSpan(PointerT<TBegin> begin, Int count) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin, typename TEnd>
    constexpr auto MakeSpan(PointerT<TBegin> begin, PointerT<TEnd> end) noexcept;

    /// \brief Convert a span to its read-only equivalent.
    template <typename TElement>
    [[nodiscard]] constexpr Span<TElement> ReadOnly(const SpanT<TElement>& rhs);

    /// \brief Const rvalue reference deleted to disallow rvalue arguments.
    template <typename TElement>
    constexpr void ReadOnly(const SpanT<TElement>&&) = delete;

    /// \brief Convert a read-only span to its read-write equivalent.
    /// If rhs doesn't refer to an original read-write memory location, the behavior of this method is undefined.
    template <typename TElement>
    [[nodiscard]] constexpr RWSpan<Traits::RemoveConst<TElement>> ReadWrite(const SpanT<TElement>& rhs) noexcept;

    /// \brief Return either lhs if non-empty or lhs otherwise.
    template <typename TElement, typename UElement>
    [[nodiscard]] constexpr CommonSpan<TElement, UElement> Either(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept;

    /// \brief Stream insertion for Spans.
    template <typename TElement>
    std::ostream& operator<<(std::ostream& lhs, const SpanT<TElement>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TElement>.
    // ===============

    template <typename TElement>
    template <typename TBegin>
    constexpr SpanT<TElement>::SpanT(TBegin begin, Int count) noexcept
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
    constexpr PointerT<TElement> SpanT<TElement>::GetData() const noexcept
    {
        return data_;
    }

    // Non-member functions.
    // =====================

    // Iterators.

    template <typename TElement>
    constexpr PointerT<TElement> begin(const SpanT<TElement>& span) noexcept
    {
        return Begin(span);
    }

    template <typename TElement>
    constexpr PointerT<TElement> end(const SpanT<TElement>& span) noexcept
    {
        return End(span);
    }

    template <typename TElement>
    constexpr PointerT<TElement> Begin(const SpanT<TElement>& span) noexcept
    {
        return span.GetData();
    }

    template <typename TElement>
    constexpr PointerT<TElement> End(const SpanT<TElement>& span) noexcept
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

    template <typename TElement>
    constexpr SpanT<TElement> Front(const SpanT<TElement>& span, Int count) noexcept
    {
        return PopBack(span, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> Back(const SpanT<TElement>& span, Int count) noexcept
    {
        return PopFront(span, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> Select(const SpanT<TElement>& span, Int offset, Int count) noexcept
    {
        return { Begin(span) + offset, count };
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopFront(const SpanT<TElement>& span, Int count) noexcept
    {
        return Select(span, count, Count(span) - count);
    }

    template <typename TElement>
    constexpr SpanT<TElement> PopBack(const SpanT<TElement>& span, Int count) noexcept
    {
        return Select(span, 0, Count(span) - count);
    }

    template <typename TElement>
    constexpr Tuple<TElement&, SpanT<TElement>> SliceFront(const SpanT<TElement>& span) noexcept
    {
        return { Front(span), PopFront(span) };
    }

    template <typename TElement>
    constexpr Tuple<TElement&, SpanT<TElement>> SliceBack(const SpanT<TElement>& span) noexcept
    {
        return { Back(span), PopBack(span) };
    }

    template <typename TElement>
    constexpr Tuple<SpanT<TElement>, SpanT<TElement>> SliceFront(const SpanT<TElement>& span, Int count) noexcept
    {
        return { Front(span, count), PopFront(span, count) };
    }

    template <typename TElement>
    constexpr Tuple<SpanT<TElement>, SpanT<TElement>> SliceBack(const SpanT<TElement>& span, Int count) noexcept
    {
        return { Back(span, count), PopBack(span, count) };
    }

    // Set operations.

    template <typename TElement, typename UElement>
    constexpr CommonSpan<TElement, UElement> Union(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (lhs && rhs)
        {
            auto begin = Math::Min(Begin(lhs), Begin(rhs));
            auto end = Math::Max(End(lhs), End(rhs));

            return { begin, end };
        }

        return Either(lhs, rhs);
    }

    template <typename TElement, typename UElement>
    constexpr CommonSpan<TElement, UElement> Intersection(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (lhs && rhs)
        {
            auto begin = Math::Max(Begin(lhs), Begin(rhs));
            auto end = Math::Min(End(lhs), End(rhs));

            return { begin, Math::Max(begin, end) };
        }

        return {};
    }

    template <typename TElement, typename UElement>
    constexpr CommonSpan<TElement, UElement> DifferenceFront(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (rhs)
        {
            auto begin = Begin(lhs);
            auto end = Math::Min(End(lhs), Begin(rhs));

            return { begin, Math::Max(begin, end) };
        }

        return lhs;
    }

    template <typename TElement, typename UElement>
    constexpr CommonSpan<TElement, UElement> DifferenceBack(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (rhs)
        {
            auto begin = Math::Max(Begin(lhs), End(rhs));
            auto end = End(lhs);

            return { Math::Min(begin, end), end };
        }

        return lhs;
    }

    template <typename TElement, typename UElement>
    constexpr Bool Contains(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        return Intersection(lhs, rhs) == rhs;
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
            return false;                           // Early out if spans sizes are different.
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
    constexpr Bool StartsWith(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Front(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr Bool EndsWith(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Back(lhs, rhs_count), rhs));
    }

    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Find(const SpanT<TElement>& lhs, const UElement& rhs) noexcept
    {
        auto result = lhs;

        for (; result && (Front(result) != rhs); result = PopFront(result));

        return result;
    }

    template <typename TElement, typename UElement>
    constexpr SpanT<TElement> Find(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (rhs)
        {
            auto result = Find(lhs, Front(rhs));

            for (; Count(result) >= Count(rhs); result = Find(PopFront(result), Front(rhs)))
            {
                if (StartsWith(result, rhs))
                {
                    return result;
                }
            }

            return {};
        }

        return lhs;
    }

    // Utilities.

    template <typename TBegin>
    constexpr SpanT<TBegin> MakeSpan(PointerT<TBegin> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TBegin, typename TEnd>
    constexpr auto MakeSpan(PointerT<TBegin> begin, PointerT<TEnd> end) noexcept
    {
        using TSpan = CommonTypeT<TBegin, TEnd>;

        return Span<TSpan>{ begin, end };
    }

    template <typename TElement>
    constexpr Span<TElement> ReadOnly(const SpanT<TElement>& rhs)
    {
        return rhs;
    }

    template <typename TElement>
    constexpr RWSpan<Traits::RemoveConst<TElement>> ReadWrite(const SpanT<TElement>& rhs) noexcept
    {
        using TPointer = PointerT<Traits::RemoveConst<TElement>>;

        auto begin = const_cast<TPointer>(Begin(rhs));
        auto end = const_cast<TPointer>(End(rhs));

        return { begin, end };
    }

    template <typename TElement, typename UElement>
    constexpr CommonSpan<TElement, UElement> Either(const SpanT<TElement>& lhs, const SpanT<UElement>& rhs) noexcept
    {
        if (lhs)
        {
            return lhs;
        }
        else
        {
            return rhs;
        }
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
