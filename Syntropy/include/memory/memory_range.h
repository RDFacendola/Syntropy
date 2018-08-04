
/// \file memory_range.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for memory ranges.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstdint>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"

#include "diagnostics/assert.h"

#include "math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    /// \brief Represents a range of contiguous memory addresses.
    /// The range is of the form [base; top)
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create an empty memory range.
        constexpr MemoryRange() = default;

        /// \brief Default copy constructor.
        constexpr MemoryRange(const MemoryRange&) = default;

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param top One past the last address in the range.
        constexpr MemoryRange(MemoryAddress base, MemoryAddress top);

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param size Size of the range.
        constexpr MemoryRange(MemoryAddress base, Bytes size);

        /// \brief Default assignment operator.
        constexpr MemoryRange& operator=(const MemoryRange&) = default;

        /// \brief Check whether the range is non-empty.
        /// \return Returns true if the range is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        constexpr MemoryAddress operator[](Bytes offset) const;

        /// \brief Get the base address of the range.
        /// \return Returns the base address of the range.
        constexpr MemoryAddress GetBase() const noexcept;

        /// \brief Get the address past the end of this range,
        /// \return Returns the address past the end of this range.
        constexpr MemoryAddress GetTop() const noexcept;

        /// \brief Get the size of the range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        constexpr Bytes GetSize() const noexcept;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this memory range, returns false otherwise.
        constexpr bool Contains(const MemoryRange& memory_range) const noexcept;

        /// \brief Check whether an address falls within this memory range.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory range, returns false otherwise.
        constexpr bool Contains(MemoryAddress address) const noexcept;

    private:

        MemoryAddress base_;          ///< \brief First address in the memory range.

        MemoryAddress top_;           ///< \brief One past the last address in the memory range.

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr MemoryRange::MemoryRange(MemoryAddress base, MemoryAddress top)
        : base_(base)
        , top_(top)
    {
        SYNTROPY_ASSERT(base <= top);
    }

    constexpr MemoryRange::MemoryRange(MemoryAddress base, Bytes size)
        : MemoryRange(base, base + size)
    {

    }

    constexpr MemoryRange::operator bool() const noexcept
    {
        return top_ != base_;
    }

    constexpr MemoryAddress MemoryRange::operator[](Bytes offset) const
    {
        auto address = base_ + offset;

        SYNTROPY_ASSERT(Contains(address));

        return address;
    }

    constexpr MemoryAddress MemoryRange::GetBase() const noexcept
    {
        return base_;
    }

    constexpr MemoryAddress MemoryRange::GetTop() const noexcept
    {
        return top_;
    }

    constexpr Bytes MemoryRange::GetSize() const noexcept
    {
        return Bytes(std::size_t(top_ - base_));
    }

    constexpr bool MemoryRange::Contains(const MemoryRange& memory_range) const noexcept
    {
        return base_ <= memory_range.base_ && memory_range.top_ <= top_;
    }

    constexpr bool MemoryRange::Contains(MemoryAddress address) const noexcept
    {
        return base_ <= address && address < top_;
    }
}