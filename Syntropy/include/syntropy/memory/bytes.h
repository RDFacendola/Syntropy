
/// \file bytes.h
/// \brief This header is part of the Syntropy memory module. It contains definitions of memory unit types and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>
#include <compare>

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/rational.h"
#include "syntropy/language/traits/manipulation.h"
#include "syntropy/memory/memory_types.h"
#include "syntropy/memory/details/bytes_details.h"

namespace Syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a bytes amount.
    /// \tparam TUnit Type of units. Default Bytes.
    /// \author Raffaele D. Facendola - September 2020.
    template <typename TUnit = Templates::Rational<1, 1>>
    class BytesT
    {
    public:

        /// \brief Zero memory amount.
        constexpr BytesT() noexcept = default;

        /// \brief Copy constructor with unit conversion.
        template <typename UUnit>
        constexpr BytesT(const BytesT<UUnit>& rhs) noexcept;

        /// \brief Create an amount of bytes, in TUnits.
        constexpr explicit BytesT(Int count) noexcept;

        /// \brief Get the amount of bytes, in TUnits.
        constexpr explicit operator Int() const noexcept;

        /// \brief Default copy assignment operator.
        constexpr BytesT& operator=(const BytesT&) noexcept = default;

        /// \brief Trivial destructor.
        ~BytesT() noexcept = default;

    private:

        /// \brief Actual bytes amount, in TUnits.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Pre-increment a byte amount rhs by one.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator++(BytesT<TUnit>& rhs) noexcept;

    /// \brief Post-increment a byte amount rhs by one.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator++(BytesT<TUnit>& rhs, int) noexcept;

    /// \brief Pre-decrement a byte amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator--(BytesT<TUnit>& rhs) noexcept;

    /// \brief Post-decrement a byte amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator--(BytesT<TUnit>& rhs, int) noexcept;

    /// \brief Add a byte amount rhs to lhs and return a reference to the latter.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator+=(BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Subtract a byte amount rhs to lhs and return a reference to the latter. 
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator-=(BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Multiply a byte amount lhs by rhs and return a reference to the former.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator*=(BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount lhs by rhs and return a reference to the former.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator/=(BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Get the division remainder of a byte amount lhs and an integral number rhs, updating lhs with the result and returning a reference to it.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator%=(BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Negate a byte amount.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator-(const BytesT<TUnit>& rhs) noexcept;

    /// \brief Add two byte amounts together
    template <typename TUnit>
    constexpr BytesT<TUnit> operator+(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Subtract a byte amount rhs from lhs.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator-(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(const BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(Int lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Divide a byte amount by a number, rounding the result towards zero.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator/(const BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount lhs by another byte amounts rhs, rounding the result towards zero.
    template <typename TUnit>
    constexpr Int operator/(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by a number.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(const BytesT<TUnit>& lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by another amount.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept;

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Compare two bytes amounts.
    template <typename T0Unit, typename T1Unit>
    constexpr auto operator<=>(const BytesT<T0Unit>& lhs, const BytesT<T1Unit>& rhs) noexcept;

    /// \brief Check whether two bytes amounts refer to the same quantity.
    template <typename T0Unit, typename T1Unit>
    constexpr bool operator==(const BytesT<T0Unit>& lhs, const BytesT<T1Unit>& rhs) noexcept;

    /************************************************************************/
    /* UNITS                                                                */
    /************************************************************************/

    /// \brief Type alias for an amount of memory, in Bits.
    using Bits = BytesT<Templates::Rational<1, 8>>;

    /// \brief Type alias for an amount of memory, in Bytes.
    using Bytes = BytesT<Templates::Rational<1, 1>>;

    /// \brief Type alias for an amount of memory, in KByes.
    using KiloBytes = BytesT<Templates::Kilo>;

    /// \brief Type alias for an amount of memory, in MBytes.
    using MegaBytes = BytesT<Templates::Mega>;

    /// \brief Type alias for an amount of memory, in GBytes.
    using GigaBytes = BytesT<Templates::Giga>;

    /// \brief Type alias for an amount of memory, in TBytes.
    using TeraBytes = BytesT<Templates::Tera>;

    /// \brief Type alias for an amount of memory, in KiBytes.
    using KibiBytes = BytesT<Templates::Kibi>;

    /// \brief Type alias for an amount of memory, in MiBytes.
    using MebiBytes = BytesT<Templates::Mebi>;

    /// \brief Type alias for an amount of memory, in GiBytes.
    using GibiBytes = BytesT<Templates::Gibi>;

    /// \brief Type alias for an amount of memory, in TiBytes.
    using TebiBytes = BytesT<Templates::Tebi>;

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert an amount of bytes to integer (in TUnits).
    template <typename TUnit>
    constexpr Int ToInt(const BytesT<TUnit>& rhs) noexcept;

    /// \brief Convert an integer number to a bytes amount (in TUnits).
    template <typename TBytes = Bytes>
    constexpr TBytes ToBytes(Int rhs) noexcept;

    /// \brief Convert a bytes amount in any unit to base bytes units.
    template <typename TUnit>
    constexpr Bytes ToBytes(const BytesT<TUnit>& rhs) noexcept;

    /// \brief Convert a bytes amount to another amount with different units, rounding the result towards zero.
    template <typename TBytesTo, typename TUnitFrom>
    constexpr TBytesTo FromBytes(const BytesT<TUnitFrom>& rhs) noexcept;

    /************************************************************************/
    /* TEMPLATES                                                            */
    /************************************************************************/

    /// \brief Exposes template-related declarations.
    namespace Templates
    {
        /// \brief Constant equal to true if TBytes is a BytesT<T> type, equal to false otherwise.
        template <typename TRational>
        constexpr bool IsBytes = Details::IsBytes<TRational>;

        /// \brief Type equal to units of a BytesT<> type TBytes.
        template <typename TBytes>
        using ByteUnits = typename Details::ByteUnits<TBytes>::TType;
    }

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes bytes-related functions.
    namespace Memory
    {
        /// \brief Get the size of rhs, in bytes.
        template <typename TType>
        constexpr Bytes SizeOf(const TType& rhs) noexcept;

        /// \brief Get the size of TType, in bytes.
        template <typename TType>
        constexpr Bytes SizeOf() noexcept;
    }

    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr operator+(RWBytePtr lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr operator-(RWBytePtr lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr& operator+=(BytePtr& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr& operator-=(BytePtr& lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr operator+(BytePtr lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr operator-(BytePtr lhs, const BytesT<TUnit>& rhs) noexcept;

    /// \brief Stream insertion for BytesT<TUnit>.
    template <typename TUnit>
    std::ostream& operator<<(std::ostream& lhs, const BytesT<TUnit>& rhs);

    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief Memory unit literals.
    namespace Literals
    {
        /// \brief User-defined literal used to convert a number from Bytes to Bytes.
        /// \param number Number to convert.
        constexpr Bytes operator "" _Bytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in KiloBytes.
        /// \param number Number to convert.
        constexpr KiloBytes operator "" _KBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in MegaBytes.
        /// \param number Number to convert.
        constexpr MegaBytes operator "" _MBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in GigaBytes.
        /// \param number Number to convert.
        constexpr GigaBytes operator "" _GBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in TeraBytes.
        /// \param number Number to convert.
        constexpr TeraBytes operator "" _TBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in KibiBytes.
        /// \param number Number to convert.
        constexpr KibiBytes operator "" _KiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in MebiBytes.
        /// \param number Number to convert.
        constexpr MebiBytes operator "" _MiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in GibiBytes.
        /// \param number Number to convert.
        constexpr GibiBytes operator "" _GiBytes(std::size_t lhs) noexcept;

        /// \brief User-defined literal used to declare an amount of memory in TebiBytes.
        /// \param number Number to convert.
        constexpr TebiBytes operator "" _TiBytes(std::size_t lhs) noexcept;
    }



    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BytesT<TUnit>.
    // ===============

    template <typename TUnit>
    constexpr BytesT<TUnit>::BytesT(Int count) noexcept
        : count_(count)
    {

    }

    template <typename TUnit>
    template <typename UUnit>
    constexpr BytesT<TUnit>::BytesT(const BytesT<UUnit>& rhs) noexcept
        : count_(ToInt(FromBytes<BytesT<TUnit>>(rhs)))
    {

    }

    template <typename TUnit>
    constexpr BytesT<TUnit>::operator Int() const noexcept
    {
        return count_;
    }

    // Non-member functions.
    // =====================

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator++(BytesT<TUnit>& rhs) noexcept
    {
        rhs += ToBytes<TUnit>(1);

        return rhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator++(BytesT<TUnit>& rhs, int) noexcept
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator--(BytesT<TUnit>& rhs) noexcept
    {
        rhs -= ToBytes<TUnit>(1);

        return rhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator--(BytesT<TUnit>& rhs, int) noexcept
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator+=(BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator-=(BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator*=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = lhs * rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator/=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = lhs / rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator%=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = lhs % rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator-(const BytesT<TUnit>& rhs) noexcept
    {
        return BytesT<TUnit>{ -ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator+(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) + ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator-(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) - ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(const BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) * rhs };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(Int lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return BytesT<TUnit>{ lhs * ToInt(rhs) };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator/(const BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) / rhs };
    }

    template <typename TUnit>
    constexpr Int operator/(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return ToInt(lhs) / ToInt(rhs);
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(const BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) % rhs };
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(const BytesT<TUnit>& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return BytesT<TUnit>{ ToInt(lhs) % ToInt(rhs) };
    }

    // Comparison.
    // ===========

     template <typename T0Unit, typename T1Unit>
     constexpr auto operator<=>(const BytesT<T0Unit>& lhs, const BytesT<T1Unit>& rhs) noexcept
     {
         using TCommon = Templates::CommonRational<T0Unit, T1Unit>;
 
         auto common_lhs = FromBytes<BytesT<TCommon>>(lhs);
         auto common_rhs = FromBytes<BytesT<TCommon>>(rhs);
 
         return ToInt(common_lhs) <=> ToInt(common_rhs);
     }

     template <typename T0Unit, typename T1Unit>
     constexpr bool operator==(const BytesT<T0Unit>& lhs, const BytesT<T1Unit>& rhs) noexcept
     {
         using TCommon = Templates::CommonRational<T0Unit, T1Unit>;

         auto common_lhs = FromBytes<BytesT<TCommon>>(lhs);
         auto common_rhs = FromBytes<BytesT<TCommon>>(rhs);

         return ToInt(common_lhs) == ToInt(common_rhs);
     }

    // Type cast.
    // ==========

    template <typename TUnit>
    constexpr Int ToInt(const BytesT<TUnit>& rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    template <typename TBytes>
    constexpr TBytes ToBytes(Int rhs) noexcept
    {
        static_assert(Templates::IsBytes<TBytes>, "TBytes is not a ByteT<> type.");
        return TBytes{ rhs };
    }

    template <typename TUnit>
    constexpr Bytes ToBytes(const BytesT<TUnit>& rhs) noexcept
    {
        return FromBytes<Bytes>(rhs);
    }

    template <typename TBytesTo, typename TUnitFrom>
    constexpr TBytesTo FromBytes(const BytesT<TUnitFrom>& rhs) noexcept
    {
        static_assert(Templates::IsBytes<TBytesTo>, "TBytesTo is not a ByteT<> type.");

        using TRatio = Templates::RationalQuotient<TUnitFrom, Templates::ByteUnits<TBytesTo>>;

        auto count = (ToInt(rhs) * TRatio::kNumerator) / TRatio::kDenominator;

        return TBytesTo{ count };
    }

    // Memory.
    // =======

    template <typename TType>
    constexpr Bytes Memory::SizeOf(const TType& rhs) noexcept
    {
        return Bytes{ sizeof(rhs) };
    }

    template <typename TType>
    constexpr Bytes Memory::SizeOf() noexcept
    {
        return Bytes{ sizeof(TType) };
    }

    // Pointers.
    // =========

    template <typename TUnit>
    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs + ToInt(ToBytes(rhs));

        return lhs;
    }

    template <typename TUnit>
    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs - ToInt(ToBytes(rhs));

        return lhs;
    }

    template <typename TUnit>
    constexpr RWBytePtr operator+(RWBytePtr lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr RWBytePtr operator-(RWBytePtr lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr BytePtr& operator+=(BytePtr& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytePtr& operator-=(BytePtr& lhs, const BytesT<TUnit>& rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytePtr operator+(BytePtr lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return lhs + ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    constexpr BytePtr operator-(BytePtr lhs, const BytesT<TUnit>& rhs) noexcept
    {
        return lhs - ToInt(ToBytes(rhs));
    }

    template <typename TUnit>
    inline std::ostream& operator<<(std::ostream& lhs, const BytesT<TUnit>& rhs)
    {
        return lhs << ToInt(rhs);
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

