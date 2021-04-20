
/// \file ignore.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IGNORE                                                               */
    /************************************************************************/

    template <typename... TArguments>
    constexpr
    Ignore
    ::Ignore(Forwarding<TArguments>...) noexcept
    {

    }

    template <typename... TArguments>
    constexpr Mutable<Ignore>
    Ignore
    ::operator=(Forwarding<TArguments>...) noexcept
    {
        return *this;
    }

    template <typename... TArguments>
    constexpr void
    Ignore
    ::operator()(Forwarding<TArguments>...) noexcept
    {

    }
}

// ===========================================================================
