
/// \file size.inl
///
/// \author Raffaele D. Facendola - August 2018.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SIZE                                                                 */
    /************************************************************************/

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    ::Size(Int count) noexcept
        : count_(count)
    {

    }

    template <Templates::IsRatio TUnit>
    template <Templates::IsRatio UUnit>
    constexpr Size<TUnit>
    ::Size(Immutable<Size<UUnit>> rhs) noexcept
        : count_(ToInt(FromBytes<Size<TUnit>>(rhs)))
    {

    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    ::operator Int() const noexcept
    {
        return count_;
    }

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    template <typename TType>
    [[nodiscard]] constexpr Bytes
    Memory
    ::SizeOf(Immutable<TType> rhs) noexcept
    {
        return Bytes{ sizeof(rhs) };
    }

    template <typename TType>
    [[nodiscard]] constexpr Bytes
    Memory
    ::SizeOf() noexcept
    {
        return Bytes{ sizeof(TType) };
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator++(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs += Size<TUnit>{ 1 };

        return rhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator++(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <Templates::IsRatio TUnit>
    constexpr
    Mutable<Size<TUnit>> operator--(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs -= Size<TUnit>{ 1 };

        return rhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator--(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<Size<TUnit>>
    operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ -ToInt(rhs) };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) * rhs };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ lhs * ToInt(rhs) };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) / rhs };
    }

    template <Templates::IsRatio TUnit>
    constexpr Int
    operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % rhs };
    }

    template <Templates::IsRatio TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

     template <Templates::IsRatio TUnit, Templates::IsRatio UUnit>
     [[nodiscard]] constexpr Ordering
     operator<=>(Immutable<Size<TUnit>> lhs,
                 Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <Templates::IsRatio TUnit, Templates::IsRatio UUnit>
     [[nodiscard]] constexpr Bool
     operator==(Immutable<Size<TUnit>> lhs,
                Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) == ToInt(common_rhs);
     }


    template <Templates::IsRatio TUnit>
    constexpr Mutable<RWBytePtr>
    operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + ToInt(ToSize(rhs));

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<RWBytePtr>
    operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr RWBytePtr
    operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Templates::IsRatio TUnit>
    constexpr RWBytePtr
    operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<BytePtr>
    operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr Mutable<BytePtr>
    operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Templates::IsRatio TUnit>
    constexpr BytePtr
    operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Templates::IsRatio TUnit>
    constexpr BytePtr
    operator-(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <typename TSize, Templates::IsRatio TUnitTo>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Int rhs) noexcept
    {
        return Size<TUnitTo>{ rhs };
    }

    template <Templates::IsRatio TUnit>
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

    template <Templates::IsRatio TUnit>
    [[nodiscard]] constexpr Bytes
    ToBytes(Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <typename TSize,
              Templates::IsRatio TUnitFrom,
              Templates::IsRatio TUnitTo>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Immutable<Size<TUnitFrom>> rhs) noexcept
    {
        using TUnit = Templates::RatioDivide<TUnitFrom, TUnitTo>;

        auto count = (ToInt(rhs) * TUnit::kNumerator) / TUnit::kDenominator;

        return TSize{ count };
    }

    /************************************************************************/
    /* MEMORY LITERALS                                                      */
    /************************************************************************/

    [[nodiscard]]  constexpr Bytes
    MemoryLiterals
    ::operator "" _Bytes(IntLiteral lhs) noexcept
    {
        return Bytes(lhs);
    }

    [[nodiscard]] constexpr KiloBytes
    MemoryLiterals
    ::operator "" _KBytes(IntLiteral lhs) noexcept
    {
        return KiloBytes(lhs);
    }

    [[nodiscard]] constexpr MegaBytes
    MemoryLiterals
    ::operator "" _MBytes(IntLiteral lhs) noexcept
    {
        return MegaBytes(lhs);
    }

    [[nodiscard]] constexpr GigaBytes
    MemoryLiterals
    ::operator "" _GBytes(IntLiteral lhs) noexcept
    {
        return GigaBytes(lhs);
    }

    [[nodiscard]] constexpr TeraBytes
    MemoryLiterals
    ::operator "" _TBytes(IntLiteral lhs) noexcept
    {
        return TeraBytes(lhs);
    }

    [[nodiscard]] constexpr KibiBytes
    MemoryLiterals
    ::operator "" _KiBytes(IntLiteral lhs) noexcept
    {
        return KibiBytes(lhs);
    }

    [[nodiscard]] constexpr MebiBytes
    MemoryLiterals
    ::operator "" _MiBytes(IntLiteral lhs) noexcept
    {
        return MebiBytes(lhs);
    }

    [[nodiscard]] constexpr GibiBytes
    MemoryLiterals
    ::operator "" _GiBytes(IntLiteral lhs) noexcept
    {
        return GibiBytes(lhs);
    }

    [[nodiscard]] constexpr TebiBytes
    MemoryLiterals
    ::operator "" _TiBytes(IntLiteral lhs) noexcept
    {
        return TebiBytes(lhs);
    }

}

// ===========================================================================
