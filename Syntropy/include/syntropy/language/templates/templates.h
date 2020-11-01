
/// \file templates.h
/// \brief This header is part of Syntropy language module. It contains basic definitions for template machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/templates_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* CONSTANT \ ALIAS                                                     */
    /************************************************************************/

    /// \brief Wraps a static constant of type TType.
    template <typename TType, TType VValue>
    using Constant = Details::Constant<TType, VValue>;

    /// \brief Wraps an alias type Type.
    template <typename TType>
    using Alias = Details::Alias<TType>;

    /************************************************************************/
    /* TRUE \ FALSE                                                         */
    /************************************************************************/

    /// \brief Boolean constant equal to true.
    using True = Details::True;

    /// \brief Boolean constant equal to false.
    using False = Details::False;

    /************************************************************************/
    /* ALWAYS TRUE \ FALSE                                                  */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and evaluates to true.
    template <typename... TTypes>
    inline constexpr Bool AlwaysTrue = Details::AlwaysTrue<TTypes...>;

    /// \brief Boolean constant which consume any template argument and evaluates to false.
    template <typename... TTypes>
    inline constexpr Bool AlwaysFalse = Details::AlwaysFalse<TTypes...>;

    /************************************************************************/
    /* VOID                                                                 */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = Details::Void<TTypes...>;

    /************************************************************************/
    /* ILL-FORMED                                                           */
    /************************************************************************/

    /// \brief An alias type that results in an ill-formed program.
    template <typename... TTypes>
    using IllFormed = Details::IllFormed<TTypes...>;

    /************************************************************************/
    /* IDENTITY                                                             */
    /************************************************************************/

    /// \brief Type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    using Identity = typename Details::Identity<TType>::Type;

}

// ===========================================================================