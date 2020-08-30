
/// \file placeholders.h
/// \brief This header is part of Syntropy language module. It contains definition for placeholder types.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

namespace Syntropy::Placeholders
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
        template <typename... TArguments>
        constexpr Ignore(TArguments&&...) {};

        /// \brief Assign from any argument number and types.
        template <typename... TArguments>
        constexpr void operator=(TArguments&&...) const {};
    };

    /// \brief Constant of the placeholder type Ignore used to discard any number of arguments.
    inline constexpr Ignore kIgnore = Ignore{};
}