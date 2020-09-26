
/// \file alignment.h
/// \brief This header is part of Syntropy memory module. It contains alignment-related definitions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/language/foundation.h"
#include "syntropy/memory/byte.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/address.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/bits.h"

namespace Syntropy
{
    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Represents an alignment value.
    enum class Alignment : Int {};

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes alignment-related functions.
    namespace Memory
    {
        /// \brief Get the alignment of rhs.
        template <typename TType>
        constexpr Alignment AlignmentOf(const TType& rhs) noexcept;

        /// \brief Get the alignment of TType.
        template <typename TType>
        constexpr Alignment AlignmentOf() noexcept;

        /// \brief Get an alignment which is at least as large as that of every scalar type.
        constexpr Alignment MaxAlignment() noexcept;

        /// \brief Check whether a pointer is aligned to a given boundary.
        Bool IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept;

        /// \brief Move a byte pointer forwards until it gets aligned to a specified value.
        BytePtr Align(BytePtr pointer, Alignment alignment) noexcept;

        /// \brief Move a byte pointer forwards until it gets aligned to a specified value.
        RWBytePtr Align(RWBytePtr pointer, Alignment alignment) noexcept;

        /// \brief Move a byte pointer backwards until it gets aligned to a specified value.
        BytePtr AlignDown(BytePtr pointer, Alignment alignment) noexcept;

        /// \brief Move a byte pointer backwards until it gets aligned to a specified value.
        RWBytePtr AlignDown(RWBytePtr pointer, Alignment alignment) noexcept;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Shift an alignment value right.
    constexpr RWReference<Alignment> operator>>=(RWReference<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr RWReference<Alignment> operator<<=(RWReference<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value right.
    constexpr Alignment operator>>(Reference<Alignment> lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr Alignment operator<<(Reference<Alignment> lhs, Int rhs) noexcept;

    /************************************************************************/
    /* TYPE CAST                                                            */
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

    /************************************************************************/
    /* STREAM INSERTION                                                     */
    /************************************************************************/

    /// \brief Stream insertion for Alignment.
    std::ostream& operator<<(std::ostream& lhs, Reference<Alignment> rhs);

    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief Exposes alignment unit literals.
    namespace Literals
    {
        /// \brief User-defined literal used to convert a number to Alignment type.
        constexpr Alignment operator "" _Alignment(std::size_t lhs) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    template <typename TType>
    constexpr Alignment Memory::AlignmentOf(const TType& rhs) noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    constexpr Alignment Memory::AlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    constexpr Alignment Memory::MaxAlignment() noexcept
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }

    inline Bool Memory::IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        return (ToAddress(pointer) & mask) == Address{};
    }

    inline BytePtr Memory::Align(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned = (ToAddress(pointer) + mask) & ~mask;

        return FromAddress(aligned);
    }

    inline RWBytePtr Memory::Align(RWBytePtr pointer, Alignment alignment) noexcept
    {
        auto x = Align(ReadOnly(pointer), alignment);

        return ReadWrite(x);
    }

    inline BytePtr Memory::AlignDown(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned = ToAddress(pointer) & ~mask;

        return FromAddress(aligned);
    }

    inline RWBytePtr Memory::AlignDown(RWBytePtr pointer, Alignment alignment) noexcept
    {
        auto x = AlignDown(ReadOnly(pointer), alignment);
        
        return ReadWrite(x);
    }

    // Non-member functions.
    // =====================

    constexpr RWReference<Alignment> operator>>=(RWReference<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    constexpr RWReference<Alignment> operator<<=(RWReference<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    constexpr Alignment operator>>(Reference<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    constexpr Alignment operator<<(Reference<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, Reference<Alignment> rhs)
    {
        return lhs << ToInt(rhs);
    }

    // Type cast.
    // ==========

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
        SYNTROPY_ASSERT(Math::IsPow2(lhs));

        return Alignment(lhs);
    }

    constexpr Alignment ToAlignment(Bytes lhs) noexcept
    {
        return ToAlignment(ToInt(lhs));
    }

    // Literals.
    
    constexpr Alignment Literals::operator "" _Alignment(std::size_t lhs) noexcept
    {
        return Alignment(lhs);
    }

}

