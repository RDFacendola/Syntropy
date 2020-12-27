
/// \file size.h
/// \brief This header is part of the Syntropy memory module. It contains definitions of memory unit types and related functions.
///
/// \author Raffaele D. Facendola - Aug 2018

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/support/compare.h"

#include "syntropy/memory/byte.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* SIZE                                                                 */
    /************************************************************************/

    /// \brief Represents a data size amount.
    /// \tparam TUnit Units used to express the data amount.
    /// \author Raffaele D. Facendola - September 2020.
    template <Concepts::RatioType TUnit>
    class Size
    {
    public:

        /// \brief Unit type the amount is expressed in.
        using Unit = TUnit;

        /// \brief Zero memory amount.
        constexpr Size() noexcept = default;

        /// \brief Copy constructor with ratio conversion.
        template <Concepts::RatioType UUnit>
        constexpr Size(Immutable<Size<UUnit>> rhs) noexcept;

        /// \brief Create a data size amount, in Units.
        constexpr explicit Size(Int count) noexcept;

        /// \brief Get the data size amount, in Units.
        constexpr explicit operator Int() const noexcept;

        /// \brief Trivial destructor.
        ~Size() noexcept = default;

        /// \brief Default copy assignment operator.
        constexpr Mutable<Size> operator=(Immutable<Size>) noexcept = default;

    private:

        /// \brief Actual amount, in TByteUnits.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* AIRTHMETIC OPERATIONS                                                */
    /************************************************************************/

    /// \brief Pre-increment a data size amount rhs by one unit.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator++(Mutable<Size<TUnit>> rhs) noexcept;

    /// \brief Post-increment a data size amount rhs by one unit.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator++(Mutable<Size<TUnit>> rhs, int) noexcept;

    /// \brief Pre-decrement a data size amount by one unit.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator--(Mutable<Size<TUnit>> rhs) noexcept;

    /// \brief Post-decrement a data size amount by one unit.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator--(Mutable<Size<TUnit>> rhs, int) noexcept;

    /// \brief Add a data size amount rhs to lhs and return a reference to the latter.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs to lhs and return a reference to the latter. 
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount lhs by rhs and return a reference to the former.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by rhs and return a reference to the former.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the division remainder of a data size amount lhs and an integral number rhs, updating lhs with the result and returning a reference to it.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Negate a data size amount.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator-(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Add two data size amounts together.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs from lhs.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Divide a data size amount by a number, rounding the result towards zero.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by another data size amounts rhs, rounding the result towards zero.
    template <Concepts::RatioType TUnit>
    constexpr Int operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by a number.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by another amount.
    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Compare two data size amounts.
    template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
    constexpr Ordering operator<=>(Immutable<Size<TUnit>> lhs, Immutable<Size<UUnit>> rhs) noexcept;

    /// \brief Check whether two data size amounts refer to the same quantity.
    template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
    constexpr Bool operator==(Immutable<Size<TUnit>> lhs, Immutable<Size<UUnit>> rhs) noexcept;

    /************************************************************************/
    /* UNITS                                                                */
    /************************************************************************/

    /// \brief Type alias for a data size amount, in Bits.
    using Bits = Size<Templates::Ratio<1, 8>>;

    /// \brief Type alias for a data size amount, in Bytes.
    using Bytes = Size<Templates::Ratio<1, 1>>;

    /// \brief Type alias for a data size amount, in KByes.
    using KiloBytes = Size<Templates::Kilo>;

    /// \brief Type alias for a data size amount, in MBytes.
    using MegaBytes = Size<Templates::Mega>;

    /// \brief Type alias for a data size amount, in GBytes.
    using GigaBytes = Size<Templates::Giga>;

    /// \brief Type alias for a data size amount, in TBytes.
    using TeraBytes = Size<Templates::Tera>;

    /// \brief Type alias for a data size amount, in KiBytes.
    using KibiBytes = Size<Templates::Kibi>;

    /// \brief Type alias for a data size amount, in MiBytes.
    using MebiBytes = Size<Templates::Mebi>;

    /// \brief Type alias for a data size amount, in GiBytes.
    using GibiBytes = Size<Templates::Gibi>;

    /// \brief Type alias for a data size amount, in TiBytes.
    using TebiBytes = Size<Templates::Tebi>;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert a data size amount to integer (in TUnits).
    template <Concepts::RatioType TUnit>
    constexpr Int ToInt(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Convert a data size amount in any unit to bytes units.
    constexpr Bytes ToBytes(Int rhs) noexcept;

    /// \brief Convert a data size amount in any unit to bytes units.
    template <Concepts::RatioType TUnit>
    constexpr Bytes ToBytes(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Convert an integer number to a data size amount (in TSize::Unit).
    template <typename TSize, Concepts::RatioType TUnitTo = typename TSize::Unit>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
    constexpr TSize ToSize(Int rhs) noexcept;

    /// \brief Convert a data size amount to another amount with different units, rounding the result towards zero.
    template <typename TSize, Concepts::RatioType TUnitFrom, Concepts::RatioType TUnitTo = typename TSize::Unit>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
    constexpr TSize ToSize(Immutable<Size<TUnitFrom>> rhs) noexcept;

    /************************************************************************/
    /* BASIC                                                                */
    /************************************************************************/

    /// \brief Get the size of rhs, in Bytes.
    template <typename TType>
    constexpr Bytes SizeOf(Immutable<TType> rhs) noexcept;

    /// \brief Get the size of TType, in Bytes.
    template <typename TType>
    constexpr Bytes SizeOf() noexcept;

    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr> operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr> operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr> operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr> operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr BytePtr operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Concepts::RatioType TUnit>
    constexpr BytePtr operator-(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory::Literals
{
    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief User-defined literal used to convert a number from Bytes to Bytes.
    /// \param number Number to convert.
    constexpr Bytes operator "" _Bytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in KiloBytes.
    /// \param number Number to convert.
    constexpr KiloBytes operator "" _KBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in MegaBytes.
    /// \param number Number to convert.
    constexpr MegaBytes operator "" _MBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in GigaBytes.
    /// \param number Number to convert.
    constexpr GigaBytes operator "" _GBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in TeraBytes.
    /// \param number Number to convert.
    constexpr TeraBytes operator "" _TBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in KibiBytes.
    /// \param number Number to convert.
    constexpr KibiBytes operator "" _KiBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in MebiBytes.
    /// \param number Number to convert.
    constexpr MebiBytes operator "" _MiBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in GibiBytes.
    /// \param number Number to convert.
    constexpr GibiBytes operator "" _GiBytes(std::size_t lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount in TebiBytes.
    /// \param number Number to convert.
    constexpr TebiBytes operator "" _TiBytes(std::size_t lhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Size<TUnit>.
    // ============

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>::Size(Int count) noexcept
        : count_(count)
    {

    }

    template <Concepts::RatioType TUnit>
    template <Concepts::RatioType UUnit>
    constexpr Size<TUnit>::Size(Immutable<Size<UUnit>> rhs) noexcept
        : count_(ToInt(FromBytes<Size<TUnit>>(rhs)))
    {

    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit>::operator Int() const noexcept
    {
        return count_;
    }

    // Arithmetic.
    // ===========

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator++(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs += Size<TUnit>{ 1 };

        return rhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator++(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator--(Mutable<Size<TUnit>> rhs) noexcept
    {
        rhs -= Size<TUnit>{ 1 };

        return rhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator--(Mutable<Size<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<Size<TUnit>> operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator-(Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ -ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) * rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ lhs * ToInt(rhs) };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) / rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Int operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % rhs };
    }

    template <Concepts::RatioType TUnit>
    constexpr Size<TUnit> operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return Size<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

    // Comparison.
    // ===========

     template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
     constexpr Ordering operator<=>(Immutable<Size<TUnit>> lhs, Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;
 
         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);
 
         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <Concepts::RatioType TUnit, Concepts::RatioType UUnit>
     constexpr Bool operator==(Immutable<Size<TUnit>> lhs, Immutable<Size<UUnit>> rhs) noexcept
     {
         using TCommonUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = ToSize<Size<TCommonUnit>>(lhs);
         auto common_rhs = ToSize<Size<TCommonUnit>>(rhs);

         return ToInt(common_lhs) == ToInt(common_rhs);
     }

    // Conversion.
    // ===========

    template <Concepts::RatioType TUnit>
    constexpr Int ToInt(Immutable<Size<TUnit>> rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    constexpr Bytes ToBytes(Int rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <Concepts::RatioType TUnit>
    constexpr Bytes ToBytes(Immutable<Size<TUnit>> rhs) noexcept
    {
        return ToSize<Bytes>(rhs);
    }

    template <typename TSize, Concepts::RatioType TUnitTo>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
    constexpr TSize ToSize(Int rhs) noexcept
    {
        return Size<TUnitTo>{ rhs };
    }

    template <typename TSize, Concepts::RatioType TUnitFrom, Concepts::RatioType TUnitTo>
    requires Concepts::SameAs<TSize, Size<TUnitTo>>
    constexpr TSize ToSize(Immutable<Size<TUnitFrom>> rhs) noexcept
    {
        using TUnit = Templates::RatioDivide<TUnitFrom, TUnitTo>;

        auto count = (ToInt(rhs) * TUnit::kNumerator) / TUnit::kDenominator;

        return TSize{ count };
    }

    // Basic.
    // ======

    template <typename TType>
    constexpr Bytes SizeOf(Immutable<TType> rhs) noexcept
    {
        return Bytes{ sizeof(rhs) };
    }

    template <typename TType>
    constexpr Bytes SizeOf() noexcept
    {
        return Bytes{ sizeof(TType) };
    }

    // Pointers.
    // =========

    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr> operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + ToInt(ToSize(rhs));

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<RWBytePtr> operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
    constexpr RWBytePtr operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr> operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr Mutable<BytePtr> operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <Concepts::RatioType TUnit>
    constexpr BytePtr operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <Concepts::RatioType TUnit>
    constexpr BytePtr operator-(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    // Literals.
    // =========

    constexpr Bytes Literals::operator "" _Bytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs);
    }

    constexpr KiloBytes Literals::operator "" _KBytes(std::size_t lhs) noexcept
    {
        return KiloBytes(lhs);
    }

    constexpr MegaBytes Literals::operator "" _MBytes(std::size_t lhs) noexcept
    {
        return MegaBytes(lhs);
    }

    constexpr GigaBytes Literals::operator "" _GBytes(std::size_t lhs) noexcept
    {
        return GigaBytes(lhs);
    }

    constexpr TeraBytes Literals::operator "" _TBytes(std::size_t lhs) noexcept
    {
        return TeraBytes(lhs);
    }

    constexpr KibiBytes Literals::operator "" _KiBytes(std::size_t lhs) noexcept
    {
        return KibiBytes(lhs);
    }

    constexpr MebiBytes Literals::operator "" _MiBytes(std::size_t lhs) noexcept
    {
        return MebiBytes(lhs);
    }

    constexpr GibiBytes Literals::operator "" _GiBytes(std::size_t lhs) noexcept
    {
        return GibiBytes(lhs);
    }

    constexpr TebiBytes Literals::operator "" _TiBytes(std::size_t lhs) noexcept
    {
        return TebiBytes(lhs);
    }
}

// ===========================================================================


