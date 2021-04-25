
/// \file definitions.inl
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* DEFINITIONS                                                          */
    /************************************************************************/

    template <typename TType>
    constexpr Immutable<TType>
    Copy(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Forwarding<Templates::QualifiedOf<TType>>
    Move(Forwarding<TType> rhs) noexcept
    {
        return static_cast<Forwarding<Templates::QualifiedOf<TType>>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Reference<Templates::QualifiedOf<TType>> rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Forwarding<Templates::QualifiedOf<TType>> rhs) noexcept
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
