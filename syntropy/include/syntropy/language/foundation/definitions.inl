
/// \file definitions.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* DEFINITIONS                                                          */
    /************************************************************************/

    template <typename TType>
    constexpr TType
    Copy(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Forwarding<Templates::RemoveReference<TType>>
    Move(Forwarding<TType> rhs) noexcept
    {
        return static_cast<Movable<Templates::RemoveReference<TType>>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Reference<Templates::RemoveReference<TType>> rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Movable<Templates::RemoveReference<TType>> rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr BasePtr<TType>
    PtrOf(Reference<TType> rhs) noexcept
    {
        return std::addressof(rhs);
    }
}

// ===========================================================================
