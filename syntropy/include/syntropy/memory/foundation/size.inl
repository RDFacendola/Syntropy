
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

    template <Concepts::RatioType TUnit> onstexpr Size<TUnit>
    ::Size(Int count) noexcept
        : count_(count)
    {

    }

    template <Concepts::RatioType TUnit>
    template <Concepts::RatioType UUnit>
    constexpr Size<TUnit>
    ::Size(Immutable<Size<UUnit>> rhs) noexcept
        : count_(ToInt(FromBytes<Size<TUnit>>(rhs)))
    {

    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    ::operator Int() const noexcept
    {
        return count_;
    }

    /************************************************************************/
    /* AIRTHMETIC OPERATIONS                                                */
    /************************************************************************/

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator++(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs += Size<TUnit>{ 1 };

        return rhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator++(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <Concepts::RatioType TUnit>
    constexpr
    Mutable<Size<TUnit>> operator--(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs -= Size<TUnit>{ 1 };

        return rhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator--(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ -ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) * rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ lhs * ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) / rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Int
    operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

     template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
     [[nodiscard]] constexpr Ordering
     operator<=>(Immutable<Size<TUnit>> lhs,
                 Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
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

    template <Concepts::RatioType TUnit>
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

    template <Concepts::RatioType TUnit>
    [[nodiscard]] constexpr Bytes
    ToBytes(Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <typename TSize, Concepts::RatioType TUnitTo>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Int rhs) noexcept
    {
        return Size<TUnitTo>{ rhs };
    }

    template <typename TSize,
              Concepts::RatioType TUnitFrom,
              Concepts::RatioType TUnitTo>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
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

    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + ToInt(ToSize(rhs));

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr
    operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr
    operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr BytePtr
    operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
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
    ::operator "" _Bytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs);
    }

    [[nodiscard]] constexpr KiloBytes
    ::operator "" _KBytes(std::size_t lhs) noexcept
    {
        return KiloBytes(lhs);
    }

    [[nodiscard]] constexpr MegaBytes
    ::operator "" _MBytes(std::size_t lhs) noexcept
    {
        return MegaBytes(lhs);
    }

    [[nodiscard]] constexpr GigaBytes
    ::operator "" _GBytes(std::size_t lhs) noexcept
    {
        return GigaBytes(lhs);
    }

    [[nodiscard]] constexpr TeraBytes
    ::operator "" _TBytes(std::size_t lhs) noexcept
    {
        return TeraBytes(lhs);
    }

    [[nodiscard]] constexpr KibiBytes
    ::operator "" _KiBytes(std::size_t lhs) noexcept
    {
        return KibiBytes(lhs);
    }

    [[nodiscard]] constexpr MebiBytes
    ::operator "" _MiBytes(std::size_t lhs) noexcept
    {
        return MebiBytes(lhs);
    }

    [[nodiscard]] constexpr GibiBytes
    ::operator "" _GiBytes(std::size_t lhs) noexcept
    {
        return GibiBytes(lhs);
    }

    [[nodiscard]] constexpr TebiBytes
    ::operator "" _TiBytes(std::size_t lhs) noexcept
    {
        return TebiBytes(lhs);
    }
}

// ===========================================================================
