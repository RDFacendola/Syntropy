
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#if 0

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/foundation/tuple.h"
#include "syntropy/core/foundation/range.h"

// ===========================================================================

namespace Syntropy
{

    // Set operations.
    // ===============

    /// \brief Get the smallest span including both lhs and rhs.
    /// \remarks This function may introduce elements that do not belong to either lhs and rhs. If those elements refer to an invalid memory region, accessing those elements results in undefined behavior.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Union(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Get the largest span shared between lhs and rhs.
    /// \remarks If lhs and rhs are disjoint, this method returns an unspecified empty span.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Intersection(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Reduce lhs from the back until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceFront(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Reduce lhs from the front until the intersection between lhs and rhs becomes empty or lhs is exhausted.
    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> DifferenceBack(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Check whether rhs is identical to any subset in lhs.
    template <typename TType, typename UType>
    constexpr Bool Contains(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    // Comparisons.
    // ============

    /// \brief Check whether two spans are element-wise equivalent.
    template <typename TType, typename UType>
    constexpr Bool Equals(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Check whether lhs starts with rhs.
    /// \remarks If lhs starts with rhs, return true, otherwise return false.
    template <typename TType, typename UType>
    constexpr Bool StartsWith(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Check whether lhs ends with lhs.
    /// \remarks If lhs ends with rhs, return true, otherwise return false.
    template <typename TType, typename UType>
    constexpr Bool EndsWith(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    /// \brief Reduce lhs until lhs starts with rhs or lhs is exhausted.
    /// \return Returns the reduced range starting from the first occurrence of rhs in lhs or an empty range if no occurrence was found.
    ///         If rhs is empty lhs is returned instead.
    template <typename TType, typename UType>
    constexpr Span<TType> Find(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Set operations.

    template <typename TType, typename UType>
    constexpr CommonSpan<TType, UType> Union(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
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
    constexpr CommonSpan<TType, UType> Intersection(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
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
    constexpr CommonSpan<TType, UType> DifferenceFront(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
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
    constexpr CommonSpan<TType, UType> DifferenceBack(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
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
    constexpr Bool Contains(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        return Intersection(lhs, rhs) == rhs;
    }

    template <typename TType, typename UType>
    constexpr Bool Equals(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
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
    constexpr Bool StartsWith(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Front(lhs, rhs_count), rhs));
    }

    template <typename TType, typename UType>
    constexpr Bool EndsWith(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        auto lhs_count = Count(lhs);
        auto rhs_count = Count(rhs);

        return (lhs_count >= rhs_count) && (Equals(Back(lhs, rhs_count), rhs));
    }

    template <typename TType, typename UType>
    constexpr Span<TType> Find(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        if (rhs)
        {
            auto result = Algorithms::Find(lhs, Front(rhs));

            for (; Count(result) >= Count(rhs); result = Algorithms::Find(PopFront(result), Front(rhs)))
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
}

// ===========================================================================

#endif