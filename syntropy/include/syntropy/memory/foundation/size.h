
/// \file size.h
///
/// \brief This header is part of the Syntropy memory module.
///        It contains definitions of memory unit types and related functions.
///
/// \author Raffaele D. Facendola - Aug 2018

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/memory/foundation/byte.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* SIZE                                                                 */
    /************************************************************************/

    /// \brief Represents a data size amount.
    /// \tparam TUnit Units used to express the data amount.
    /// \author Raffaele D. Facendola - September 2020.
    template <Templates::RatioType TUnit>
    class Size
    {
    public:

        /// \brief Unit type the amount is expressed in.
        using Unit = TUnit;

        /// \brief Zero memory amount.
        constexpr Size() noexcept = default;

        /// \brief Copy constructor with ratio conversion.
        template <Templates::RatioType UUnit>
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
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator++(Mutable<Size<TUnit>> rhs) noexcept;

    /// \brief Post-increment a data size amount rhs by one unit.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator++(Mutable<Size<TUnit>> rhs, int) noexcept;

    /// \brief Pre-decrement a data size amount by one unit.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator--(Mutable<Size<TUnit>> rhs) noexcept;

    /// \brief Post-decrement a data size amount by one unit.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator--(Mutable<Size<TUnit>> rhs, int) noexcept;

    /// \brief Add a data size amount rhs to lhs and return a reference to
    ///        the latter.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator+=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs to lhs and return a reference
    ///        to the latter.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator-=(Mutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount lhs by rhs and return a reference
    ///        to the former.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator*=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by rhs and return a reference to
    ///        the former.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator/=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the division remainder of a data size amount lhs and an
    ///        integral number rhs, updating lhs with the result and returning
    ///        a reference to it.
    template <Templates::RatioType TUnit>
    constexpr Mutable<Size<TUnit>>
    operator%=(Mutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Negate a data size amount.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Add two data size amounts together.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator+(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Subtract a data size amount rhs from lhs.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator-(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Multiply a data size amount by a number.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator*(Int lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Divide a data size amount by a number, rounding the result
    ///        towards zero.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator/(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Divide a data size amount lhs by another data size amounts rhs,
    ///        rounding the result towards zero.
    template <Templates::RatioType TUnit>
    constexpr Int
    operator/(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by a number.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Int rhs) noexcept;

    /// \brief Get the remainder of a data size amount divided by
    ///        another amount.
    template <Templates::RatioType TUnit>
    constexpr Size<TUnit>
    operator%(Immutable<Size<TUnit>> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    /// \brief Compare two data size amounts.
    template <Templates::RatioType TUnit, Templates::RatioType UUnit>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<Size<TUnit>> lhs,
                Immutable<Size<UUnit>> rhs) noexcept;

    /// \brief Check whether two data size amounts refer to the same quantity.
    template <Templates::RatioType TUnit, Templates::RatioType UUnit>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<Size<TUnit>> lhs,
               Immutable<Size<UUnit>> rhs) noexcept;

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

    /// \brief Convert an integer number to a data size amount
    ///        (in TSize::Unit).
    template <typename TSize,
              Templates::RatioType TUnitTo = typename TSize::Unit>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Int rhs) noexcept;

    /// \brief Convert a data size amount to integer (in TUnits).
    template <Templates::RatioType TUnit>
    [[nodiscard]] constexpr Int
    ToInt(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Convert a data size amount in any unit to bytes units.
    [[nodiscard]] constexpr Bytes
    ToBytes(Int rhs) noexcept;

    /// \brief Convert a data size amount in any unit to bytes units.
    template <Templates::RatioType TUnit>
    [[nodiscard]] constexpr Bytes
    ToBytes(Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Convert a data size amount to another amount with different
    ///        units, rounding the result towards zero.
    template <typename TSize,
              Templates::RatioType TUnitFrom,
              Templates::RatioType TUnitTo = typename TSize::Unit>
    requires Templates::IsSame<TSize, Size<TUnitTo>>
    [[nodiscard]] constexpr TSize
    ToSize(Immutable<Size<TUnitFrom>> rhs) noexcept;

    /************************************************************************/
    /* BASIC                                                                */
    /************************************************************************/

    /// \brief Get the size of rhs, in Bytes.
    template <typename TType>
    [[nodiscard]] constexpr Bytes
    SizeOf(Immutable<TType> rhs) noexcept;

    /// \brief Get the size of TType, in Bytes.
    template <typename TType>
    [[nodiscard]] constexpr Bytes
    SizeOf() noexcept;

    /// \brief True if all types have the same size.
    template <typename TType, typename... TTypes>
    inline constexpr
    Bool IsSameSize
        = ((Memory::SizeOf<TType>() == Memory::SizeOf<TTypes>()) && ...);

    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator+=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr Mutable<RWBytePtr>
    operator-=(Mutable<RWBytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr RWBytePtr
    operator+(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr RWBytePtr
    operator-(RWBytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator+=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr Mutable<BytePtr>
    operator-=(Mutable<BytePtr> lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer forward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr BytePtr
    operator+(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

    /// \brief Move a byte pointer backward by a given data size amount.
    template <Templates::RatioType TUnit>
    constexpr BytePtr
    operator-(BytePtr lhs, Immutable<Size<TUnit>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory::Literals
{
    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief User-defined literal used to convert a number from Bytes
    ///        to Bytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr Bytes
    operator "" _Bytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in KiloBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr KiloBytes
    operator "" _KBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in MegaBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr MegaBytes
    operator "" _MBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in GigaBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr GigaBytes
    operator "" _GBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in TeraBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr TeraBytes
    operator "" _TBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in KibiBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr KibiBytes
    operator "" _KiBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in MebiBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr MebiBytes
    operator "" _MiBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in GibiBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr GibiBytes
    operator "" _GiBytes(IntLiteral lhs) noexcept;

    /// \brief User-defined literal used to declare a data size amount
    ///        in TebiBytes.
    /// \param number Number to convert.
    [[nodiscard]] constexpr TebiBytes
    operator "" _TiBytes(IntLiteral lhs) noexcept;
}

// ===========================================================================

#include "details/size.inl"

// ===========================================================================
