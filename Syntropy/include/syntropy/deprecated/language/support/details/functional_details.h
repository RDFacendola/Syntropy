
/// \file tuple_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details for functional-style templates.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    template <typename TCallable, typename... TArguments>
    constexpr Templates::InvokeResult<TCallable, TArguments...> Invoke(TCallable&& callable, TArguments&&... arguments) noexcept
    {
        return std::invoke(Forward<TCallable>(callable), Forward<TArguments>(arguments)...);
    }

}

// ===========================================================================

namespace Syntropy::Functional
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <Int VIndex, typename TFunction, typename... TTuples>
    constexpr decltype(auto) SliceApply(TFunction&& function, TTuples&&... tuples) noexcept;
}

// ===========================================================================

namespace Syntropy::Functional::Details
{
    /************************************************************************/
    /* LOCKSTEP                                                             */
    /************************************************************************/

    // LockstepApply.
    // ==============

    template <typename TFunction, typename... TTuples, Int... VIndexes>
    constexpr void LockstepApply(Templates::Sequence<VIndexes...>, TFunction&& function, TTuples&&... tuples) noexcept
    {
        (SliceApply<VIndexes>(Forward<TFunction>(function), Forward<TTuples>(tuples)...), ...);
    }
}

// ===========================================================================