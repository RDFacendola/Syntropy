
/// \file ignore.h
/// \brief This header is part of Syntropy language module. It contains definition for placeholders used to ignore function arguments.
///
/// \author Raffaele D. Facendola - Nov 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IGNORE                                                               */
    /************************************************************************/

    /// \brief Placeholder class type which can be constructed or called with any argument and does nothing in either case.
    /// This type can be used to discard arguments the caller is not interested to.
    /// \author Raffaele D. Facendola - September 2016.
    struct Ignore
    {
        /// \brief Construct from any argument number and types.
        constexpr Ignore() noexcept = default;

        /// \brief Construct from any argument number and types.
        template <typename... TArguments>
        constexpr Ignore(ForwardRef<TArguments>...) noexcept;

        /// \brief Assign from any argument number and types.
        template <typename... TArguments>
        constexpr Mutable<Ignore> operator=(ForwardRef<TArguments>...) noexcept;
    };

    /// \brief Constant of the placeholder type Ignore used to discard any number of arguments.
    inline constexpr Ignore kIgnore = Ignore{};
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Ignore.
    // =======

    template <typename... TArguments>
    constexpr Ignore::Ignore(ForwardRef<TArguments>...) noexcept
    {

    }

    template <typename... TArguments>
    constexpr Mutable<Ignore> Ignore::operator=(ForwardRef<TArguments>...) noexcept
    {
        return *this;
    }
}

// ===========================================================================