
/// \file tuple_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details for functional-style templates.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) ApplyAt(TFunction&& function, TTuples&&... tuples) noexcept;
}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* LOCKSTEP                                                             */
    /************************************************************************/

    // ApplyLockstep.
    // ==============

    template <typename TFunction, typename... TTuples, Int... VIndexes>
    constexpr void ApplyLockstep(Sequence<VIndexes...>, TFunction&& function, TTuples&&... tuples) noexcept
    {
        (ApplyAt<VIndexes>(Forward<TFunction>(function), Forward<TTuples>(tuples)...), ...);
    }
}

// ===========================================================================