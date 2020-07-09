
/// \file alignment.h
/// \brief This header is part of Syntropy memory module. It contains the definition of the Alignment type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/core/types.h"
#include "syntropy/math/bits.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Represents an alignment value.
    enum class Alignment : Int {};

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
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

    /// \brief Get the alignment of rhs.
    template <typename TType>
    constexpr Alignment AlignmentOf(const TType& rhs) noexcept;

    /// \brief Get the alignment of TType.
    template <typename TType>
    constexpr Alignment AlignmentOf() noexcept;

    /// \brief Get an alignment which is at least as strict (as large) as that of every scalar type.
    constexpr Alignment MaxAlignmentOf() noexcept;

    /// \brief Shift an alignment value right.
    constexpr Alignment& operator>>=(Alignment& lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr Alignment& operator<<=(Alignment& lhs, Int rhs) noexcept;

    /// \return Shift an alignment value right.
    constexpr Alignment operator>>(const Alignment& lhs, Int rhs) noexcept;

    /// \brief Shift an alignment value left.
    constexpr Alignment operator<<(const Alignment& lhs, Int rhs) noexcept;

    /// \brief Stream insertion for Alignment.
    std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs);

    /************************************************************************/
    /* LITERALS                                                             */
    /************************************************************************/

    /// \brief Exposes alignment unit literals.
    namespace literals
    {
        /// \brief User-defined literal used to convert a number to Alignment type.
        constexpr Alignment operator "" _Alignment(std::size_t lhs) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

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

    template <typename TType>
    constexpr Alignment AlignmentOf(const TType& rhs) noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    constexpr Alignment AlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    constexpr Alignment MaxAlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }

    constexpr Alignment& operator>>=(Alignment& lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    constexpr Alignment& operator<<=(Alignment& lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    constexpr Alignment operator>>(const Alignment& lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    constexpr Alignment operator<<(const Alignment& lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs)
    {
        return lhs << ToInt(rhs);
    }

    // Literals.
    
    constexpr Alignment literals::operator "" _Alignment(std::size_t lhs) noexcept
    {
        return Alignment(lhs);
    }

}

