
/// \file explicit.h
/// \brief This header is part of Syntropy language module. It contains definitions for explicit types used during overload resolution.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* EXPLICIT                                                             */
    /************************************************************************/

    /// \brief Type that bounds to an explicit type only.
    /// This class is meant to be used when implicit type conversion would cause overload resolution to be ambiguous.
    template <typename TTo, typename... TFrom>
    class Explicit
    {
    public:

        /// \brief Create a new explicit wrapper.
        template <typename UTo, typename = Templates::EnableIf<(Templates::IsSame<UTo, TFrom> || ...)>>
        constexpr Explicit(const UTo& value) noexcept;

        /// \brief Implicitly convert the wrapper to its underlying type.
        constexpr operator TTo() const noexcept;

    private:

        /// \brief Actual value.
        TTo value_{};

    };

    /// \brief Explicit boolean value.
    using ExplicitBool = Explicit<Bool, Bool>;

    /// \brief Explicit integer type.
    using ExplicitInt = Explicit<Int, Int, int>;

    /// \brief Explicit floating-point type.
    using ExplicitFloat = Explicit<Float, Float>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Explicit<TTo, TFrom...>.
    // ========================

    template <typename TTo, typename... TFrom>
    template <typename UTo, typename>
    constexpr Explicit<TTo, TFrom...>::Explicit(const UTo& value) noexcept
        : value_(static_cast<TTo>(value))
    {

    }

    template <typename TTo, typename... TFrom>
    constexpr Explicit<TTo, TFrom...>::operator TTo() const noexcept
    {
        return value_;
    }

}