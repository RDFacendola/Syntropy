
/// \file alignment.h
///
/// \brief This header is part of Syntropy memory module.
///        It contains alignment-related definitions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/memory/foundation/byte.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/address.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Represents an alignment value, in Bytes.
    enum class Alignment : Enum64 {};

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the alignment of rhs.
    template <typename TType>
    [[nodiscard]] constexpr Alignment
    AlignmentOf(Immutable<TType> rhs) noexcept;

    /// \brief Get the alignment of TType.
    template <typename TType>
    [[nodiscard]] constexpr Alignment
    AlignmentOf() noexcept;

    /// \brief Get an alignment which is at least as large as that of every
    ///        scalar type.
    [[nodiscard]] constexpr Alignment
    MaxAlignment() noexcept;

    /// \brief Check whether a pointer is aligned to a given boundary.
    [[nodiscard]] Bool
    IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer forwards until it gets aligned to a
    ///        specified value.
    [[nodiscard]] BytePtr
    Align(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer forwards until it gets aligned to a
    ///        specified value.
    [[nodiscard]] RWBytePtr
    Align(RWBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer backwards until it gets aligned to
    ///        a specified value.
    [[nodiscard]] BytePtr
    AlignDown(BytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer backwards until it gets aligned to
    ///        a specified value.
    [[nodiscard]] RWBytePtr
    AlignDown(RWBytePtr pointer, Alignment alignment) noexcept;

    /************************************************************************/
    /* BITWISE OPERATIONS                                                   */
    /************************************************************************/

    /// \brief Shift an alignment value right.
    [[nodiscard]] constexpr Mutable<Alignment>
    operator>>=(Mutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    [[nodiscard]] constexpr Mutable<Alignment>
    operator<<=(Mutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value right.
    [[nodiscard]] constexpr Alignment
    operator>>(Immutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    [[nodiscard]] constexpr Alignment
    operator<<(Immutable<Alignment> lhs, Int rhs) noexcept;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert an alignment value to integer.
    [[nodiscard]] constexpr Int
    ToInt(Alignment lhs) noexcept;

    /// \brief Convert an alignment value to a bytes value.
    [[nodiscard]] constexpr Bytes
    ToBytes(Alignment lhs) noexcept;

    /// \brief Convert an integer number to an alignment value.
    ///
    /// If lhs is not a power of 2, the result of this method is undefined.
    [[nodiscard]] constexpr Alignment
    ToAlignment(Int lhs) noexcept;

    /// \brief Convert a byte amount to an alignment value.
    ///
    /// If lhs is not a power of 2, the result of this method is undefined.
    [[nodiscard]] constexpr Alignment
    ToAlignment(Bytes lhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory::Literals
{
    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief User-defined literal used to convert a number to Alignment type.
    [[nodiscard]] constexpr
    Alignment operator "" _Alignment(std::size_t lhs) noexcept;
}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    [[nodiscard]] constexpr Alignment
    AlignmentOf(Immutable<TType> rhs) noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    [[nodiscard]] constexpr Alignment
    AlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    [[nodiscard]] constexpr Alignment
    MaxAlignment() noexcept
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }

    [[nodiscard]] inline Bool
    IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        return !(ToAddress(pointer) & mask);
    }

    [[nodiscard]] inline BytePtr
    Align(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned = (ToAddress(pointer) + mask) & ~mask;

        return FromAddress<Byte>(aligned);
    }

    [[nodiscard]] inline RWBytePtr
    Align(RWBytePtr pointer, Alignment alignment) noexcept
    {
        using Syntropy::ToReadOnly;
        using Syntropy::ToReadWrite;

        return ToReadWrite(Align(ToReadOnly(pointer), alignment));
    }

    [[nodiscard]] inline BytePtr
    AlignDown(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned_pointer = ToAddress(pointer) & ~mask;

        return FromAddress<Byte>(aligned_pointer);
    }

    [[nodiscard]] inline RWBytePtr
    AlignDown(RWBytePtr pointer, Alignment alignment) noexcept
    {
        using Syntropy::ToReadOnly;
        using Syntropy::ToReadWrite;

        return ToReadWrite(AlignDown(ToReadOnly(pointer), alignment));
    }

    // Bitwise operations.
    // ===================

    [[nodiscard]] constexpr Mutable<Alignment>
    operator>>=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    [[nodiscard]] constexpr Mutable<Alignment>
    operator<<=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    [[nodiscard]] constexpr Alignment
    operator>>(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    [[nodiscard]] constexpr Alignment
    operator<<(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    // Conversion.
    // ===========

    [[nodiscard]] constexpr Int
    ToInt(Alignment lhs) noexcept
    {
        return static_cast<Int>(lhs);
    }

    [[nodiscard]] constexpr Bytes
    ToBytes(Alignment lhs) noexcept
    {
        return ToBytes(ToInt(lhs));
    }

    [[nodiscard]] constexpr Alignment
    ToAlignment(Int lhs) noexcept
    {
        return Alignment(lhs);
    }

    [[nodiscard]] constexpr Alignment
    ToAlignment(Bytes lhs) noexcept
    {
        return ToAlignment(ToInt(lhs));
    }

    // Literals.
    // =========

    [[nodiscard]] constexpr Alignment
    Literals::operator "" _Alignment(std::size_t lhs) noexcept
    {
        return Alignment(lhs);
    }
}
