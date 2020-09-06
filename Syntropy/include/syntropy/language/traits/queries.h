
/// \file queries.h
/// \brief This header is part of Syntropy language module. It contains query-related type traits.
///
/// \author Raffaele D. Facendola - August 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/traits/details/queries.h"

namespace Syntropy::Traits
{
    /************************************************************************/
    /* ELEMENT TYPE                                                         */
    /************************************************************************/

    /// \brief Type alias equal to the kIndex-th type in a type list.
    /// If the provided index exceeds the type-list range, the program is ill-formed.
    template <Int VIndex, typename... TTypes>
    using ElementType = typename Details::ElementType<VIndex, TTypes...>::Type;

    /************************************************************************/
    /* ELEMENT INDEX                                                        */
    /************************************************************************/

    /// \brief Constant equal to the index of the element with type TMatch among a list of types [TTypes...].
    /// If no matching element could be found, the program is ill-formed.
    template <typename TMatch, typename... TTypes>
    constexpr Int ElementIndex = Details::ElementIndex<TMatch, TTypes...>::value_;

    /************************************************************************/
    /* FUNCTION ARGUMENTS                                                   */
    /************************************************************************/

    /// \brief Type alias equal to the argument types a callable object can be called with.
    /// If no matching element could be found, the program is ill-formed.
    template <typename TCallable>
    using FunctionArguments = typename Details::FunctionArguments<TCallable>::Type;

    /************************************************************************/
    /* FUNCTION ARGUMENT                                                    */
    /************************************************************************/

    /// \brief Type alias for the kIndex-th argument a callable object can be called with.
    template <Int VIndex, typename TCallable>
    using FunctionArgument = ElementType<VIndex, FunctionArguments<TCallable>>;

}