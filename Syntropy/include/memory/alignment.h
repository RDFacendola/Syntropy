
/// \file alignment.h
/// \brief This header is part of the syntropy memory management system. It contains the definition of the Alignment type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <ostream>

#include "diagnostics/assert.h"
#include "memory/bytes.h"
#include "math/math.h"

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

        /// \brief Create a new alignment equal to 1.
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
        constexpr operator std::size_t() const noexcept;

        /// \brief Get the alignment value, in bytes.
        /// \return Returns the alignment value, in bytes.
        constexpr operator Bytes() const noexcept;

        /// \brief Get the alignment value.
        /// \return Returns the alignment value.
        constexpr operator std::align_val_t() const noexcept;

        /// \brief Right shift operator.
        /// \return Returns a reference to this element.
        constexpr Alignment& operator>>=(std::size_t rhs) noexcept;

        /// \brief Left shift operator.
        /// \return Returns a reference to this element.
        constexpr Alignment& operator<<=(std::size_t rhs) noexcept;

    private:

        std::size_t alignment_ = 1u;           ///< \brief Alignment, in bytes.

    };

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
    constexpr Alignment operator>>(const Alignment& lhs, std::size_t rhs) noexcept;

    /// \brief Left-shift an alignment.
    /// \return Returns an alignment which is equal to lhs left-shifted by rhs.
    constexpr Alignment operator<<(const Alignment& lhs, std::size_t rhs) noexcept;

    /// \brief Stream insertion for Alignment.
    std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr Alignment::Alignment(std::align_val_t alignment) noexcept
        : Alignment(Bytes(std::size_t(alignment)))
    {

    }

    constexpr Alignment::Alignment(Bytes alignment)
        : alignment_(std::size_t(alignment))
    {
        SYNTROPY_ASSERT(Math::IsPow2(alignment_));          // Support to power-of-two alignment only.
    }

    constexpr Alignment::operator std::size_t() const noexcept
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

    constexpr Alignment& Alignment::operator>>=(std::size_t rhs) noexcept
    {
        alignment_ >>= rhs;
        return *this;
    }

    constexpr Alignment& Alignment::operator<<=(std::size_t rhs) noexcept
    {
        alignment_ <<= rhs;
        return *this;
    }

    constexpr bool operator==(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return std::size_t(lhs) == std::size_t(rhs);
    }

    constexpr bool operator!=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return std::size_t(lhs) > std::size_t(rhs);
    }

    constexpr bool operator<(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return std::size_t(lhs) < std::size_t(rhs);
    }

    constexpr bool operator>=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return std::size_t(lhs) >= std::size_t(rhs);
    }

    constexpr bool operator<=(const Alignment& lhs, const Alignment& rhs) noexcept
    {
        return std::size_t(lhs) <= std::size_t(rhs);
    }

    constexpr Alignment operator>>(const Alignment& lhs, std::size_t rhs) noexcept
    {
        return Alignment(lhs) >>= rhs;
    }

    constexpr Alignment operator<<(const Alignment& lhs, std::size_t rhs) noexcept
    {
        return Alignment(lhs) <<= rhs;
    }

    inline std::ostream& operator<<(std::ostream& lhs, const Alignment& rhs)
    {
        return lhs << std::size_t(rhs);
    }
}

