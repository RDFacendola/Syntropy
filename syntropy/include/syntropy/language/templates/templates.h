
/// \file templates.h
///
/// \brief This header is part of Syntropy language module.
///        It contains fundamental templates definitions.
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/details/templates_details.h"

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* COMPILE-TIME DIAGNOSTICS                                             */
    /************************************************************************/

    /// \brief Boolean constant which consume any template argument and
    ///        evaluates to false. Useful for static_assert that should always
    ///        trigger a compilation error.
    template <typename... TTypes>
    inline constexpr Bool
    AlwaysFalse = false;

    /// \brief Strucure which consume any template argument and result in the
    //         program to be ill-formed.
    template <typename TType, typename... TTypes>
    struct IllFormed
    {
        static_assert(AlwaysFalse<TTypes...>, "The program is ill-formed");

        /// \brief Dummy value.
        TType kValue = TType{};
    };

    /************************************************************************/
    /* TYPES                                                                */
    /************************************************************************/

    /// \brief Exposes a member Type equal to TType.
    template <typename TType>
    struct Alias
    {
        using Type = TType;
    };

    /************************************************************************/
    /* CONSTANTS                                                            */
    /************************************************************************/

    /// \brief Exposes a member Value equal to VValue.
    template <typename TType, TType VValue>
    struct Constant
    {
        static constexpr TType kValue = VValue;
    };

    /// \brief Boolean constant.
    template <Bool TValue>
    using BoolConstant = Constant<Bool, TValue>;

    /// \brief True boolean constant.
    using True = BoolConstant<true>;

    /// \brief False boolean constant.
    using False = BoolConstant<false>;

    /// \brief Integer constant.
    template <Int TValue>
    using IntConstant = Constant<Int, TValue>;

    /************************************************************************/
    /* SFINAE                                                               */
    /************************************************************************/

    /// \brief Metafunction that maps a sequence of types to void.
    template <typename... TTypes>
    using Void = Details::Void<TTypes...>;

    /// \brief Type equal to TType if TEnable is true, otherwise
    ///        there's no such type.
    template <Bool TEnable>
    using EnableIf = Details::EnableIf<TEnable>;

    /// \brief Boolean value equal to true if TExpression<TTypes...> is a
    ///        valid expression, false otherwise.
    template <template<typename...> typename TExpression,
              typename... TTypes>
    inline constexpr Bool IsValidExpression
        = Details::IsValidExpression<TExpression, TTypes...>;

    /// \brief If TExpression<TTypes> is a valid expression, exposes a member
    ///        typedef equal to void, otherwise there's no such type.
    template <template<typename...> typename TExpression, typename... TTypes>
    using EnableIfValidExpression
        = Details::EnableIfValidExpression<TExpression, TTypes...>;

}

// ===========================================================================
