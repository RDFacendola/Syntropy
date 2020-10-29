
/// \file predicates.h
/// \brief This header is part of the Syntropy core module. It contains definitions for predicates
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"

// ===========================================================================

namespace Syntropy::Algorithm
{
    /************************************************************************/
    /* PREDICATES                                                           */
    /************************************************************************/

    /// \brief Check whether a predicate holds true for at least one element in a collection.
    template <typename TIterable, typename TPredicate>
    Bool Any(Reference<TIterable> iterable, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds true for all the elements in a collection.
    template <typename TIterable, typename TPredicate>
    Bool All(Reference<TIterable> iterable, TPredicate&& predicate) noexcept;

    /// \brief Check whether a predicate holds false for all the elements in a collection.
    template <typename TIterable, typename TPredicate>
    Bool None(Reference<TIterable> iterable, TPredicate&& predicate) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Predicates.
    // ===========

    template <typename TIterable, typename TPredicate>
    inline Bool Any(Reference<TIterable> rhs, TPredicate&& predicate) noexcept
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
    inline Bool All(Reference<TIterable> rhs, TPredicate&& predicate) noexcept
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
    inline Bool None(Reference<TIterable> rhs, TPredicate&& predicate) noexcept
    {
        return !Any(rhs, Forward<TPredicate>(predicate));
    }

}

// ===========================================================================