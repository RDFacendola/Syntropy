
/// \file definitions.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    template <typename TType>
    constexpr Bool
    ToBool(TType rhs) noexcept
    {
        return !!rhs;
    }

    template <typename TNumber>
    constexpr Int
    ToInt(TNumber rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    template <typename TNumber>
    constexpr Float
    ToFloat(TNumber rhs) noexcept
    {
        return static_cast<Float>(rhs);
    }

    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    template <typename TType>
    constexpr Immutable<TType>
    ToImmutable(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Mutable<TType>
    ToMutable(Immutable<TType> rhs) noexcept
    {
        return const_cast<Mutable<TType>>(rhs);
    }

    template <typename TType>
    constexpr Immovable<TType>
    ToImmovable(Immutable<TType> rhs) noexcept
    {
        return static_cast<Immovable<TType>>(rhs);
    }

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    template <typename TType, typename UType>
    constexpr BasePtr<TType>
    ToPtr(BasePtr<UType> rhs) noexcept
    {
        return static_cast<BasePtr<TType>>(rhs);
    }

    template <typename TType>
    constexpr Ptr<TType>
    ToReadOnly(Ptr<TType> rhs) noexcept
    {
        return const_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    constexpr RWPtr<TType>
    ToReadWrite(Ptr<TType> rhs) noexcept
    {
        return const_cast<RWPtr<TType>>(rhs);
    }

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    template <typename TNumber>
    constexpr Fix8
    ToFix8(TNumber rhs) noexcept
    {
        return static_cast<Fix8>(rhs);
    }

    template <typename TNumber>
    constexpr Fix16
    ToFix16(TNumber rhs) noexcept
    {
        return static_cast<Fix16>(rhs);
    }

    template <typename TNumber>
    constexpr Fix32
    ToFix32(TNumber rhs) noexcept
    {
        return static_cast<Fix32>(rhs);
    }

    template <typename TNumber>
    constexpr Fix64
    ToFix64(TNumber rhs) noexcept
    {
        return static_cast<Fix64>(rhs);
    }

    /************************************************************************/
    /* TYPELESS POINTER TYPES                                               */
    /************************************************************************/

    template <Concepts::IsMutable TType>
    inline TypelessPtr
    ToTypelessPtr(BasePtr<TType> rhs) noexcept
    {
        return rhs;
    }

    template <Concepts::IsMutable TType>
    inline RWTypelessPtr
    ToTypelessPtr(BasePtr<TType> rhs) noexcept
    {
        return const_cast<RWTypelessPtr>(rhs);
    }

    template <typename TType>
    inline Ptr<TType>
    FromTypelessPtr(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    inline RWPtr<TType>
    FromTypelessPtr(RWTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<RWPtr<TType>>(rhs);
    }

}

// ===========================================================================
