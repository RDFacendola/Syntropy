
/// \file data_size.h
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

namespace Syntropy
{
    /************************************************************************/
    /* DATA SIZE                                                            */
    /************************************************************************/

    /// \brief Represents a data size amount.
    /// \tparam TUnit Units used to express the data amount.
    /// \author Raffaele D. Facendola - September 2020.
    template <typename TUnit>
    class DataSize
    {
    public:

        /// \brief Unit type the amount is expressed in.
        using Unit = TUnit;

        /// \brief Zero memory amount.
        constexpr DataSize() noexcept = default;

        /// \brief Copy constructor with ratio conversion.
        template <typename UUnit>
        constexpr DataSize(Immutable<DataSize<UUnit>> rhs) noexcept;

        /// \brief Create a data size amount, in Units.
        constexpr explicit DataSize(Int count) noexcept;

        /// \brief Get the data size amount, in Units.
        constexpr explicit operator Int() const noexcept;

        /// \brief Trivial destructor.
        ~DataSize() noexcept = default;

        /// \brief Default copy assignment operator.
        constexpr Mutable<DataSize> operator=(Immutable<DataSize>) noexcept = default;

    private:

        /// \brief Actual amount, in TByteUnits.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* AIRTHMETIC OPERATIONS                                                */
    /************************************************************************/

    /// \brief Pre-increment a data size amount rhs by one unit.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator++(Mutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Post-increment a data size amount rhs by one unit.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator++(Mutable<DataSize<TUnit>> rhs, int) noexcept;

    /// \brief Pre-decrement a data size amount by one unit.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator--(Mutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Post-decrement a data size amount by one unit.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator--(Mutable<DataSize<TUnit>> rhs, int) noexcept;

    /// \brief Add a data size amount rhs to lhs and return a reference to the latter.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator+=(Mutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs to lhs and return a reference to the latter. 
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator-=(Mutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount lhs by rhs and return a reference to the former.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator*=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by rhs and return a reference to the former.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator/=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the division remainder of a data size amount lhs and an integral number rhs, updating lhs with the result and returning a reference to it.
    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator%=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Negate a data size amount.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator-(Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Add two data size amounts together.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator+(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs from lhs.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator-(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator*(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator*(Int lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Divide a data size amount by a number, rounding the result towards zero.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator/(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by another data size amounts rhs, rounding the result towards zero.
    template <typename TUnit>
    constexpr Int operator/(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by a number.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator%(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by another amount.
    template <typename TUnit>
    constexpr DataSize<TUnit> operator%(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Compare two data size amounts.
    template <typename TUnit, typename UUnit>
    constexpr Ordering operator<=>(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<UUnit>> rhs) noexcept;

    /// \brief Check whether two data size amounts refer to the same quantity.
    template <typename TUnit, typename UUnit>
    constexpr Bool operator==(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<UUnit>> rhs) noexcept;

    /************************************************************************/
    /* UNITS                                                                */
    /************************************************************************/

    /// \brief Type alias for a data size amount, in Bits.
    using Bits = DataSize<Templates::Ratio<1, 8>>;

    /// \brief Type alias for a data size amount, in Bytes.
    using Bytes = DataSize<Templates::Ratio<1, 1>>;

    /// \brief Type alias for a data size amount, in KByes.
    using KiloBytes = DataSize<Templates::Kilo>;

    /// \brief Type alias for a data size amount, in MBytes.
    using MegaBytes = DataSize<Templates::Mega>;

    /// \brief Type alias for a data size amount, in GBytes.
    using GigaBytes = DataSize<Templates::Giga>;

    /// \brief Type alias for a data size amount, in TBytes.
    using TeraBytes = DataSize<Templates::Tera>;

    /// \brief Type alias for a data size amount, in KiBytes.
    using KibiBytes = DataSize<Templates::Kibi>;

    /// \brief Type alias for a data size amount, in MiBytes.
    using MebiBytes = DataSize<Templates::Mebi>;

    /// \brief Type alias for a data size amount, in GiBytes.
    using GibiBytes = DataSize<Templates::Gibi>;

    /// \brief Type alias for a data size amount, in TiBytes.
    using TebiBytes = DataSize<Templates::Tebi>;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert a data size amount to integer (in TUnits).
    template <typename TUnit>
    constexpr Int ToInt(Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Convert an integer number to a data size amount (in TDataSize::Unit).
    template <typename TDataSize = Bytes>
    constexpr TDataSize ToDataSize(Int rhs) noexcept;

    /// \brief Convert a data size amount in any unit to bytes units.
    template <typename TUnit>
    constexpr Bytes ToBytes(Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Convert a data size amount to another amount with different units, rounding the result towards zero.
    template <typename TDataSizeTo, typename TUnitFrom>
    constexpr TDataSizeTo FromDataSize(Immutable<DataSize<TUnitFrom>> rhs) noexcept;

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
    template <typename TUnit>
    constexpr Mutable<MutableBytePtr> operator+=(Mutable<MutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <typename TUnit>
    constexpr Mutable<MutableBytePtr> operator-=(Mutable<MutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <typename TUnit>
    constexpr MutableBytePtr operator+(MutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <typename TUnit>
    constexpr MutableBytePtr operator-(MutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <typename TUnit>
    constexpr Mutable<ImmutableBytePtr> operator+=(Mutable<ImmutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <typename TUnit>
    constexpr Mutable<ImmutableBytePtr> operator-=(Mutable<ImmutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <typename TUnit>
    constexpr ImmutableBytePtr operator+(ImmutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <typename TUnit>
    constexpr ImmutableBytePtr operator-(ImmutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Literals
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

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // DataSize<TUnit>.
    // ===============

    template <typename TUnit>
    constexpr DataSize<TUnit>::DataSize(Int count) noexcept
        : count_(count)
    {

    }

    template <typename TUnit>
    template <typename UUnit>
    constexpr DataSize<TUnit>::DataSize(Immutable<DataSize<UUnit>> rhs) noexcept
        : count_(ToInt(FromBytes<DataSize<TUnit>>(rhs)))
    {

    }

    template <typename TUnit>
    constexpr DataSize<TUnit>::operator Int() const noexcept
    {
        return count_;
    }

    // Arithmetic.
    // ===========

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator++(Mutable<DataSize<TUnit>> rhs) noexcept
    {
        rhs += DataSize<TUnit>{ 1 };

        return rhs;
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator++(Mutable<DataSize<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator--(Mutable<DataSize<TUnit>> rhs) noexcept
    {
        rhs -= DataSize<TUnit>{ 1 };

        return rhs;
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator--(Mutable<DataSize<TUnit>> rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator+=(Mutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator-=(Mutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator*=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator/=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<DataSize<TUnit>> operator%=(Mutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator-(Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return DataSize<TUnit>{ -ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator+(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator-(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator*(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) * rhs };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator*(Int lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return DataSize<TUnit>{ lhs * ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator/(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) / rhs };
    }

    template <typename TUnit>
    constexpr Int operator/(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator%(Immutable<DataSize<TUnit>> lhs, Int rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) % rhs };
    }

    template <typename TUnit>
    constexpr DataSize<TUnit> operator%(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return DataSize<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

    // Comparison.
    // ===========

     template <typename TUnit, typename UUnit>
     constexpr Ordering operator<=>(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<UUnit>> rhs) noexcept
     {
         using TUnit = Templates::CommonRatio<TUnit, UUnit>;
 
         auto common_lhs = FromDataSize<DataSize<TUnit>>(lhs);
         auto common_rhs = FromDataSize<DataSize<TUnit>>(rhs);
 
         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <typename TUnit, typename UUnit>
     constexpr Bool operator==(Immutable<DataSize<TUnit>> lhs, Immutable<DataSize<UUnit>> rhs) noexcept
     {
         using TUnit = Templates::CommonRatio<TUnit, UUnit>;

         auto common_lhs = FromDataSize<DataSize<TUnit>>(lhs);
         auto common_rhs = FromDataSize<DataSize<TUnit>>(rhs);

         return ToInt(common_lhs) == ToInt(common_rhs);
     }

    // Conversion.
    // ===========

    template <typename TUnit>
    constexpr Int ToInt(Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    template <typename TDataSize>
    constexpr TDataSize ToDataSize(Int rhs) noexcept
    {
        return TDataSize{ rhs };
    }

    template <typename TUnit>
    constexpr Bytes ToBytes(Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return FromDataSize<Bytes>(rhs);
    }

    template <typename TDataSizeTo, typename TUnitFrom>
    constexpr TDataSizeTo FromDataSize(Immutable<DataSize<TUnitFrom>> rhs) noexcept
    {
        using TUnit = Templates::RatioDivide<TUnitFrom, typename TDataSizeTo::Unit>;

        auto count = (ToInt(rhs) * TUnit::kNumerator) / TUnit::kDenominator;

        return TDataSizeTo{ count };
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

    template <typename TUnit>
    constexpr Mutable<MutableBytePtr> operator+=(Mutable<MutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs + ToInt(ToDataSize(rhs));

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<MutableBytePtr> operator-=(Mutable<MutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <typename TUnit>
    constexpr MutableBytePtr operator+(MutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr MutableBytePtr operator-(MutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr Mutable<ImmutableBytePtr> operator+=(Mutable<ImmutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr Mutable<ImmutableBytePtr> operator-=(Mutable<ImmutableBytePtr> lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr ImmutableBytePtr operator+(ImmutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr ImmutableBytePtr operator-(ImmutableBytePtr lhs, Immutable<DataSize<TUnit>> rhs) noexcept
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


