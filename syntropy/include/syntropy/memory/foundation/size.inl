
/// \file size.inl
///
/// \author Raffaele D. Facendola - Aug 2018

#pragma once

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* SIZE                                                                 */
    /************************************************************************/

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    ::Size(Int count) noexcept
        : count_(count)
    {

    }

    template <Templates::RatioType TUnit>
    template <Templates::RatioType UUnit>
    constexpr Size<TUnit>
    ::Size(Immutable<Size<UUnit>> rhs) noexcept
        : count_(ToInt(FromBytes<Size<TUnit>>(rhs)))
    {

    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    ::operator Int() const noexcept
    {
        return count_;
    }

    /************************************************************************/
    /* AIRTHMETIC OPERATIONS                                                */
    /************************************************************************/

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator++(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs += Size<TUnit>{ 1 };

        return rhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator++(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <Templates::RatioType TUnit>
    constexpr
    Mutable<Size<TUnit>> operator--(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs -= Size<TUnit>{ 1 };

        return rhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator--(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ -ToInt(rhs) };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) * rhs };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ lhs * ToInt(rhs) };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) / rhs };
    }

    template <Templates::RatioType TUnit>
    constexpr Int
    operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % rhs };
    }

    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

     template <Templates::RatioType TUnit, Templates::RatioType UUnit>
     [[nodiscard]] constexpr Ordering
     operator<=>(Immutable<Size<TUnit>> lhs,
                 Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <Templates::RatioType TUnit, Templates::RatioType UUnit>
     [[nodiscard]] constexpr Bool
     operator==(Immutable<Size<TUnit>> lhs,
                Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) == ToInt(common_rhs);
     }

     /************************************************************************/
     /* CONVERSION                                                           */
     /************************************************************************/

    template <typename TSize, Templates::RatioType TUnitTo>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Int rhs) noexcept
    {
        return Size<TUnitTo>{ rhs };
    }

    template <Templates::RatioType TUnit>
    [[nodiscard]] constexpr Int
    ToInt(Immutable<Size<TUnit>> rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    [[nodiscard]] constexpr Bytes
    ToBytes(Int rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <Templates::RatioType TUnit>
    [[nodiscard]] constexpr Bytes
    ToBytes(Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <typename TSize,
              Templates::RatioType TUnitFrom,
              Templates::RatioType TUnitTo>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Immutable<Size<TUnitFrom>> rhs) noexcept
    {
        using TUnit = Templates::RatioDivide<TUnitFrom, TUnitTo>;

        auto count = (ToInt(rhs) * TUnit::kNumerator) / TUnit::kDenominator;

        return TSize{ count };
    }

    /************************************************************************/
    /* BASIC                                                                */
    /************************************************************************/

    template <typename TType>
    [[nodiscard]] constexpr Bytes
    SizeOf(Immutable<TType> rhs) noexcept
    {
        return Bytes{ sizeof(rhs) };
    }

    template <typename TType>
    [[nodiscard]] constexpr Bytes
    SizeOf() noexcept
    {
        return Bytes{ sizeof(TType) };
    }

    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    template <Templates::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + ToInt(ToSize(rhs));

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr RWBytePtr
    operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Templates::RatioType TUnit>
    constexpr RWBytePtr
    operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Templates::RatioType TUnit>
    constexpr BytePtr
    operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Templates::RatioType TUnit>
    constexpr BytePtr
    operator-(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }
}

// ===========================================================================

namespace Syntropy::Memory::Literals
{
    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    [[nodiscard]]  constexpr Bytes
    operator "" _Bytes(IntLiteral lhs) noexcept
    {
        return Bytes(lhs);
    }

    [[nodiscard]] constexpr KiloBytes
    operator "" _KBytes(IntLiteral lhs) noexcept
    {
        return KiloBytes(lhs);
    }

    [[nodiscard]] constexpr MegaBytes
    operator "" _MBytes(IntLiteral lhs) noexcept
    {
        return MegaBytes(lhs);
    }

    [[nodiscard]] constexpr GigaBytes
    operator "" _GBytes(IntLiteral lhs) noexcept
    {
        return GigaBytes(lhs);
    }

    [[nodiscard]] constexpr TeraBytes
    operator "" _TBytes(IntLiteral lhs) noexcept
    {
        return TeraBytes(lhs);
    }

    [[nodiscard]] constexpr KibiBytes
    operator "" _KiBytes(IntLiteral lhs) noexcept
    {
        return KibiBytes(lhs);
    }

    [[nodiscard]] constexpr MebiBytes
    operator "" _MiBytes(IntLiteral lhs) noexcept
    {
        return MebiBytes(lhs);
    }

    [[nodiscard]] constexpr GibiBytes
    operator "" _GiBytes(IntLiteral lhs) noexcept
    {
        return GibiBytes(lhs);
    }

    [[nodiscard]] constexpr TebiBytes
    operator "" _TiBytes(IntLiteral lhs) noexcept
    {
        return TebiBytes(lhs);
    }
}

// ===========================================================================
