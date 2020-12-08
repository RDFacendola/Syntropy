
/// \file alignment.h
/// \brief This header is part of Syntropy memory module. It contains alignment-related definitions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/memory/byte.h"
#include "syntropy/memory/data_size.h"
#include "syntropy/memory/address.h"

// ===========================================================================

namespace Syntropy
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
    constexpr Alignment AlignmentOf(Immutable<TType> rhs) noexcept;

    /// \brief Get the alignment of TType.
    template <typename TType>
    constexpr Alignment AlignmentOf() noexcept;

    /// \brief Get an alignment which is at least as large as that of every scalar type.
    constexpr Alignment MaxAlignment() noexcept;

    /// \brief Check whether a pointer is aligned to a given boundary.
    Bool IsAlignedTo(ImmutableBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer forwards until it gets aligned to a specified value.
    ImmutableBytePtr Align(ImmutableBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer forwards until it gets aligned to a specified value.
    MutableBytePtr Align(MutableBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer backwards until it gets aligned to a specified value.
    ImmutableBytePtr AlignDown(ImmutableBytePtr pointer, Alignment alignment) noexcept;

    /// \brief Move a byte pointer backwards until it gets aligned to a specified value.
    MutableBytePtr AlignDown(MutableBytePtr pointer, Alignment alignment) noexcept;

    /************************************************************************/
    /* BITWISE OPERATIONS                                                   */
    /************************************************************************/

    /// \brief Shift an alignment value right.
    constexpr Mutable<Alignment> operator>>=(Mutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr Mutable<Alignment> operator<<=(Mutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value right.
    constexpr Alignment operator>>(Immutable<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr Alignment operator<<(Immutable<Alignment> lhs, Int rhs) noexcept;

    /************************************************************************/
    /* CONVERSION                                                           */
    /************************************************************************/

    /// \brief Convert an alignment value to integer.
    constexpr Int ToInt(Alignment lhs) noexcept;

    /// \brief Convert an alignment value to a bytes value.
    constexpr Bytes ToBytes(Alignment lhs) noexcept;

    /// \brief Convert an integer number to an alignment value.
    /// If lhs is not a power of 2, the result of this method is undefined.
    constexpr Alignment ToAlignment(Int lhs) noexcept;

    /// \brief Convert a byte amount to an alignment value.
    /// If lhs is not a power of 2, the result of this method is undefined.
    constexpr Alignment ToAlignment(Bytes lhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Literals
{
    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief User-defined literal used to convert a number to Alignment type.
    constexpr Alignment operator "" _Alignment(std::size_t lhs) noexcept;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    constexpr Alignment AlignmentOf(Immutable<TType> rhs) noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    constexpr Alignment AlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    constexpr Alignment MaxAlignment() noexcept
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }

    inline Bool IsAlignedTo(ImmutableBytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        return !(ToAddress(pointer) & mask);
    }

    inline ImmutableBytePtr Align(ImmutableBytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned = (ToAddress(pointer) + mask) & ~mask;

        return FromAddress(aligned);
    }

    inline MutableBytePtr Align(MutableBytePtr pointer, Alignment alignment) noexcept
    {
        auto aligned_pointer = Align(ToImmutable(pointer), alignment);

        return ToMutable(aligned_pointer);
    }

    inline ImmutableBytePtr AlignDown(ImmutableBytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned_pointer = ToAddress(pointer) & ~mask;

        return FromAddress(aligned_pointer);
    }

    inline MutableBytePtr AlignDown(MutableBytePtr pointer, Alignment alignment) noexcept
    {
        auto aligned_pointer = AlignDown(ToImmutable(pointer), alignment);
        
        return ToMutable(aligned_pointer);
    }

    // Bitwise operations.
    // ===================

    constexpr Mutable<Alignment> operator>>=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    constexpr Mutable<Alignment> operator<<=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    constexpr Alignment operator>>(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    constexpr Alignment operator<<(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    // Conversion.
    // ===========

    constexpr Int ToInt(Alignment lhs) noexcept
    {
        return static_cast<Int>(lhs);
    }

    constexpr Bytes ToBytes(Alignment lhs) noexcept
    {
        return ToBytes(ToInt(lhs));
    }

    constexpr Alignment ToAlignment(Int lhs) noexcept
    {
        return Alignment(lhs);
    }

    constexpr Alignment ToAlignment(Bytes lhs) noexcept
    {
        return ToAlignment(ToInt(lhs));
    }

    // Literals.
    // =========

    constexpr Alignment Literals::operator "" _Alignment(std::size_t lhs) noexcept
    {
        return Alignment(lhs);
    }
}

