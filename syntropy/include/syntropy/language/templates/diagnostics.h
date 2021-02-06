
/// \file diagnostics.h
///
/// \brief This header is part of Syntropy language module.
///        It contains compile-time diagnostics.
///
/// \author Raffaele D. Facendola - February 2021.

#pragma once

#include "syntropy/language/foundation/types.h"

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

}

// ===========================================================================
