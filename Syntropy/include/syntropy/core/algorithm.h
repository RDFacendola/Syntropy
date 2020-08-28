
/// \file algorithm.h
/// \brief This header is part of the Syntropy core module. It contains definitions for basic algorithms.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <initializer_list>
#include <algorithm>

#include "syntropy/language/support.h"
#include "syntropy/memory/byte_span.h"

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Rotate the order of the element in a range, in such a way that the element pointed by middle becomes the new first element.
    /// If middle doesn't belong to range, the result of this method is undefined.
    template <typename TElement>
    void Rotate(const RWSpan<TElement>& span, RWPointer<TElement> middle) noexcept;

    /// \brief Check whether a predicate holds true for at least one element in a collection.
    template <typename TIterable, typename TPredicate>
    Bool Any(const TIterable& iterable, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds true for all the elements in a collection.
    template <typename TIterable, typename TPredicate>
    Bool All(const TIterable& iterable, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds false for all the elements in a collection.
    template <typename TIterable, typename TPredicate>
    Bool None(const TIterable& iterable, TPredicate&& predicate) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TElement>
    inline void Rotate(const RWSpan<TElement>& span, RWPointer<TElement> middle) noexcept
    {
        std::rotate(Begin(span), middle, End(span));
    }

    template <typename TIterable, typename TPredicate>
    inline Bool Any(const TIterable& rhs, TPredicate&& predicate) noexcept
    {
        for (auto&& element : rhs)
        {
            if (predicate(ReadOnly(element)))
            {
                return true;
            }
        }

        return false;
    }

    template <typename TIterable, typename TPredicate>
    inline Bool All(const TIterable& rhs, TPredicate&& predicate) noexcept
    {
        for (auto&& element : rhs)
        {
            if (!predicate(ReadOnly(element)))
            {
                return false;
            }
        }

        return true;
    }

    template <typename TIterable, typename TPredicate>
    inline Bool None(const TIterable& rhs, TPredicate&& predicate) noexcept
    {
        return !Any(rhs, Forward<TPredicate>(predicate));
    }

}
