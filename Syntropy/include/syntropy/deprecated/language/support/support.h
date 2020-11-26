
/// \file support.h
/// \brief This header is part of Syntropy language module. It contains generic support definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any constructor.
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    Templates::AddRValueReference<TType> Declval() noexcept;

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    /// \brief Swap lhs with rhs.
    template <typename TType, typename = Templates::EnableIf<Templates::IsMoveAssignable<TType> && Templates::IsMoveConstructible<TType>>>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept;

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
        constexpr Ignore(TArguments&&...) noexcept;

        /// \brief Assign from any argument number and types.
        template <typename... TArguments>
        constexpr Ignore& operator=(TArguments&&...) const noexcept;
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

    // Swap.
    // =====

    template <typename TType, typename>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept
    {
        auto xhs = Move(lhs);

        lhs = Move(rhs);
        rhs = Move(xhs);
    }

    // Ignore.
    // =======

    template <typename... TArguments>
    constexpr Ignore::Ignore(TArguments&&...) noexcept
    {

    }

    template <typename... TArguments>
    constexpr Ignore& Ignore::operator=(TArguments&&...) const noexcept
    {
        return *this;
    }
}

// ===========================================================================