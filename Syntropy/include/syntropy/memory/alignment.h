
/// \file alignment.h
/// \brief This header is part of Syntropy memory module. It contains the definition of the Alignment type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "syntropy/syntropy.h"
#include "syntropy/math/bits.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* ALIGNMENT                                                            */
    /************************************************************************/

    /// \brief Represents an alignment that is a power of 2.
    /// This type is meant to be a strongly-typed replacement for std::align_val_t.
    /// \author Raffaele D. Facendola - August 2018
    class Alignment
    {
    public:

        /// \brief Create a new alignment equal to the default system alignment.
        constexpr Alignment() noexcept = default;

        /// \brief Create a new alignment.
        constexpr Alignment(std::align_val_t alignment) noexcept;

        /// \brief Create a new alignment.
        constexpr explicit Alignment(Bytes alignment);

        /// \brief Default copy-constructor.
        constexpr Alignment(const Alignment&) = default;

        /// \brief Default assignment operator.
        Alignment& operator=(const Alignment&) = default;

        /// \brief Get the alignment value, in bytes.
        /// \return Returns the alignment value, in bytes.
        constexpr std::int64_t operator*() const noexcept;

        /// \brief Get the alignment value, in bytes.
        /// \return Returns the alignment value, in bytes.
        constexpr operator Bytes() const noexcept;

        /// \brief Get the alignment value.
        /// \return Returns the alignment value.
        constexpr operator std::align_val_t() const noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Alignment& operator>>=(std::int64_t rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Alignment& operator<<=(std::int64_t rhs) noexcept;

    private:

        ///< \brief Alignment, in bytes.
        std::int64_t alignment_ = 1u;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Equality comparison for Alignment.
    /// \return Returns true if lhs and rhs refer to the same alignment, returns false otherwise.
    constexpr bool operator==(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Inequality comparison for Alignment.
    /// \return Returns true if lhs and rhs refer to different alignments, returns false otherwise.
    constexpr bool operator!=(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Greater-than comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Less-than comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Greater-or-equal comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Less-or-equal comparison for Alignment.
    /// \return Returns true if lhs refers to an alignment that is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const Alignment& lhs, const Alignment& rhs) noexcept;

    /// \brief Right-shift an alignment.
    /// \return Returns an alignment which is equal to lhs right-shifted by rhs.
    constexpr Alignment operator>>(const Alignment& lhs, std::int64_t rhs) noexcept;

    /// \brief Left-shift an alignment.
    /// \return Returns an alignment which is equal to lhs left-shifted by rhs.
    constexpr Alignment operator<<(const Alignment& lhs, std::int64_t rhs) noexcept;

    /// \brief Stream insertion for Alignment.
    std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs);

    /// \brief Get the alignment of rhs.
    template <typename TType>
    constexpr Alignment AlignmentOf(const TType& rhs);

    /// \brief Get the alignment of TType.
    template <typename TType>
    constexpr Alignment AlignmentOf();

    /// \brief Get an alignment which is at least as strict (as large) as that of every scalar type.
    constexpr Alignment MaxAlignmentOf();

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Alignment.

    constexpr Alignment::Alignment(std::align_val_t alignment) noexcept
        : alignment_(std::int64_t(alignment))
    {

    }

    constexpr Alignment::Alignment(Bytes alignment)
        : alignment_(*alignment)
    {
        // Alignment are expected to be power-of-two.
        SYNTROPY_ASSERT(Math::IsPow2(alignment_));
    }

    constexpr std::int64_t Alignment::operator*() const noexcept
    {
        return alignment_;
    }

    constexpr Alignment::operator Bytes() const noexcept
    {
        return Bytes(alignment_);
    }

    constexpr Alignment::operator std::align_val_t() const noexcept
    {
        return std::align_val_t(alignment_);
    }

    constexpr Alignment& Alignment::operator>>=(std::int64_t rhs) noexcept
    {
        alignment_ >>= rhs;
        return *this;
    }

    constexpr Alignment& Alignment::operator<<=(std::int64_t rhs) noexcept
    {
        alignment_ <<= rhs;
        return *this;
    }

    // Non-member functions.

    constexpr bool operator==(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return *lhs == *rhs;
    }

    constexpr bool operator!=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return *lhs > *rhs;
    }

    constexpr bool operator<(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return *lhs < *rhs;
    }

    constexpr bool operator>=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return *lhs >= *rhs;
    }

    constexpr bool operator<=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return *lhs <= *rhs;
    }

    constexpr Alignment operator>>(const Alignment& lhs, std::int64_t rhs) noexcept
    {
        return Alignment(lhs) >>= rhs;
    }

    constexpr Alignment operator<<(const Alignment& lhs, std::int64_t rhs) noexcept
    {
        return Alignment(lhs) <<= rhs;
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs)
    {
        return lhs << *rhs;
    }

    template <typename TType>
    constexpr Alignment AlignmentOf(const TType& rhs)
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    constexpr Alignment AlignmentOf()
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    constexpr Alignment MaxAlignmentOf()
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }
}

