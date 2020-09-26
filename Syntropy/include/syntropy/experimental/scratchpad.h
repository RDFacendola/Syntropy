
/// \file bytes.h
/// \brief This header is part of the Syntropy memory module. It contains definitions of memory unit types and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/memory/memory_types.h"

namespace Syntropy
{
    /************************************************************************/
    /* BYTES                                                                */
    /************************************************************************/

    /// \brief Represents a bytes amount.
    /// \author Raffaele D. Facendola - September 2020.
    template <typename TUnit = Templates::Rational<1, 1>>
    class BytesT
    {
    public:

        /// \brief Zero memory amount.
        BytesT() noexcept = default;

        /// \brief Default copy constructor.
        BytesT(const BytesT&) noexcept = default;

        /// \brief Create an amount of bytes, in TUnits.
        explicit BytesT(Int count) noexcept;

        /// \brief Get the amount of bytes, in TUnits.
        explicit operator Int() const noexcept;

        /// \brief Default copy assignment operator.
        BytesT& operator=(const BytesT&) noexcept = default;

        /// \brief Trivial destructor.
        ~BytesT() noexcept = default;

    private:

        /// \brief Actual bytes amount, relative to TUnit.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* UNITS                                                                */
    /************************************************************************/

    /// \brief Type alias for an amount of memory, in bits.
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
    using KibiBytes = BytesT<Templates::Rational<1024, 1>>;

    /// \brief Type alias for an amount of memory, in MiBytes.
    using MebiBytes = BytesT<Templates::Rational<1024 * 1024, 1>>;

    /// \brief Type alias for an amount of memory, in GiBytes.
    using GibiBytes = BytesT<Templates::Rational<1024 * 1024 * 1024, 1>>;

    /// \brief Type alias for an amount of memory, in TiBytes.
    using TebiBytes = BytesT<Templates::Rational<1024 * 1024 * 1024 * 1024, 1>>;

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
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Sum a byte amount to an existing value.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator+=(BytesT<TUnit>& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Subtract a byte amount from an existing value.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator-=(BytesT<TUnit>& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Multiply a byte amount by a integer number.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator*=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount by an integer value and round the result towards zero.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator/=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Division remainder of a bytes amount by an integer value.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator%=(Bytes& lhs, Int rhs) noexcept;

    /// \brief Pre-increment a bytes amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator++(BytesT<TUnit>& rhs) noexcept;

    /// \brief Post-increment a bytes amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator++(BytesT<TUnit>& rhs, int) noexcept;

    /// \brief Pre-decrement a bytes amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit>& operator--(BytesT<TUnit>& rhs) noexcept;

    /// \brief Post-decrement a bytes amount by one.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator--(BytesT<TUnit>& rhs, int) noexcept;

    /// \brief Sum two bytes amount.
    /// \return Returns a memory amount which is the sum of lhs and rhs.
    template <typename T0Unit, typename T1Unit>
    constexpr Bytes operator+(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept;

    /// \brief Subtract two bytes amount.
    /// \return Returns a memory amount which is the difference of lhs and rhs.
    template <typename T0Unit, typename T1Unit>
    constexpr Bytes operator-(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs times rhs.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(BytesT<TUnit> lhs, Int rhs) noexcept;

    /// \brief Multiply a byte amount by a number.
    /// \return Returns a memory amount which is equal to rhs times lhs.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(Int lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Divide a byte amount by a number.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator/(BytesT<TUnit> lhs, Int rhs) noexcept;

    /// \brief Divide a byte amount by another byte amount.
    /// \return Returns a memory amount which is equal to lhs divided by rhs. The result is rounded towards zero.
    template <typename T0Unit, typename T1Unit>
    constexpr Int operator/(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by a number.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(BytesT<TUnit> lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a byte amount divided by another byte amount.
    /// \return Returns a memory amount which is equal to lhs modulus rhs.
    template <typename T0Unit, typename T1Unit>
    constexpr Int operator%(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr operator+(RWBytePtr lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr RWBytePtr operator-(RWBytePtr lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr& operator+=(BytePtr& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr& operator-=(BytePtr& lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr operator+(BytePtr lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given byte amount.
    template <typename TUnit>
    constexpr BytePtr operator-(BytePtr lhs, BytesT<TUnit> rhs) noexcept;

    /// \brief Stream insertion for BytesT<TUnit>.
    template <typename TUnit>
    std::ostream& operator<<(std::ostream& lhs, BytesT<TUnit> rhs);

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert an amount of bytes to integer.
    /// \remarks The returned value is expressed in TUnits!
    template <typename TUnit>
    constexpr Int ToInt(BytesT<TUnit> lhs) noexcept;

    /// \brief Convert an integer number to a bytes amount.
    /// \remarks The amount to convert is expressed in TUnits!
    template <typename TUnit = Templates::Rational<1, 1>>
    constexpr BytesT<TUnit> ToBytes(Int lhs) noexcept;

    /// \brief Convert a bytes amount to another bytes amount with different units.
    template <typename T0Unit, typename T1Unit = Templates::Rational<1, 1>>
    constexpr BytesT<T1Unit> ToBytes(BytesT<T0Unit> lhs) noexcept;

    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief Exposes memory unit literals.
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
    BytesT<TUnit>::BytesT(Int count) noexcept
        : count_(count)
    {

    }

    template <typename TUnit>
    BytesT<TUnit>::operator Int() const noexcept
    {
        return count_;
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

    // Non-member functions.
    // =====================

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator+=(BytesT<TUnit>& lhs, BytesT<TUnit> rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator-=(BytesT<TUnit>& lhs, BytesT<TUnit> rhs) noexcept
    {
        lhs = ToBytes<TUnit>(ToInt(lhs) - ToInt(rhs));

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator*=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = ToBytes<TUnit>(ToInt(lhs) * rhs);

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator/=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = ToBytes<TUnit>(ToInt(lhs) / rhs);

        return lhs;
    }

    template <typename TUnit>
    constexpr BytesT<TUnit>& operator%=(BytesT<TUnit>& lhs, Int rhs) noexcept
    {
        lhs = ToBytes<TUnit>(ToInt(lhs) % rhs);

        return lhs;
    }

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

    template <typename T0Unit, typename T1Unit>
    constexpr Bytes operator+(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept
    {
#error "Implement me"
        return ToBytes(ToInt(lhs) + ToInt(rhs));
    }

    template <typename T0Unit, typename T1Unit>
    constexpr Bytes operator-(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept
    {
#error "Implement me"
        return ToBytes(ToInt(lhs) - ToInt(rhs));
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(BytesT<TUnit> lhs, Int rhs) noexcept
    {
        return ToBytes<TUnit>(ToInt(lhs) * rhs);
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator*(Int lhs, BytesT<TUnit> rhs) noexcept
    {
        return ToBytes<TUnit>(lhs * ToInt(rhs));
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator/(BytesT<TUnit> lhs, Int rhs) noexcept
    {
        return ToBytes<TUnit>(ToInt(lhs) / rhs);
    }

    template <typename T0Unit, typename T1Unit>
    constexpr Int operator/(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept
    {
#error "Implement me"
        return ToInt(lhs) / ToInt(rhs);
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> operator%(BytesT<TUnit> lhs, Int rhs) noexcept
    {
        return ToBytes<TUnit>(ToInt(lhs) % rhs);
    }

    template <typename T0Unit, typename T1Unit>
    constexpr Int operator%(BytesT<T0Unit> lhs, BytesT<T1Unit> rhs) noexcept
    {
#error "Implement me"
        return ToInt(lhs) % ToInt(rhs);
    }

    constexpr RWBytePtr& operator+=(RWBytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    constexpr RWBytePtr& operator-=(RWBytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    constexpr RWBytePtr operator+(RWBytePtr lhs, Bytes rhs) noexcept
    {
        return lhs + ToInt(rhs);
    }

    constexpr RWBytePtr operator-(RWBytePtr lhs, Bytes rhs) noexcept
    {
        return lhs - ToInt(rhs);
    }

    constexpr BytePtr& operator+=(BytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs + rhs;

        return lhs;
    }

    constexpr BytePtr& operator-=(BytePtr& lhs, Bytes rhs) noexcept
    {
        lhs = lhs - rhs;

        return lhs;
    }

    constexpr BytePtr operator+(BytePtr lhs, Bytes rhs) noexcept
    {
        return lhs + ToInt(rhs);
    }

    constexpr BytePtr operator-(BytePtr lhs, Bytes rhs) noexcept
    {
        return lhs - ToInt(rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, Bytes rhs)
    {
        return lhs << ToInt(rhs);
    }

    // Type cast.

    template <typename TUnit>
    constexpr Int ToInt(BytesT<TUnit> lhs) noexcept
    {
        return static_cast<Int>(lhs);
    }

    template <typename TUnit>
    constexpr BytesT<TUnit> ToBytes(Int lhs) noexcept
    {
        return BytesT<TUnit>{ lhs };
    }

    template <typename T0Unit, typename T1Unit>
    constexpr BytesT<T1Unit> ToBytes(BytesT<T0Unit> lhs) noexcept
    {
#error "implement me"
    }

    // Literals.

    constexpr Bytes Literals::operator "" _Bytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs);
    }

    constexpr Bytes Literals::operator "" _KiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _MiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _GiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull);
    }

    constexpr Bytes Literals::operator "" _TiBytes(std::size_t lhs) noexcept
    {
        return Bytes(lhs * 0x400ull * 0x400ull * 0x400ull * 0x400ull);
    }

}

