
/// \file memory_range.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for memory ranges.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <cstdint>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_address.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    /// \brief Represents a range of contiguous memory addresses.
    /// The range is of the form [begin; end)
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create an empty memory range.
        constexpr MemoryRange() = default;

        /// \brief Default copy constructor.
        constexpr MemoryRange(const MemoryRange&) = default;

        /// \brief Create a memory range.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        constexpr MemoryRange(MemoryAddress begin, MemoryAddress end);

        /// \brief Default assignment operator.
        constexpr MemoryRange& operator=(const MemoryRange&) = default;

        /// \brief Check whether the range is non-empty.
        /// \return Returns true if the range is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        constexpr MemoryAddress operator[](Bytes offset) const;

        /// \brief Advance the memory range forward.
        /// \param rhs Number of bytes to move the range forward to.
        /// \return Returns a reference to this element.
        constexpr MemoryRange& operator+=(Bytes rhs) noexcept;

        /// \brief Move the memory range backwards.
        /// \param rhs Number of bytes to move the range backwards to.
        /// \return Returns a reference to this element.
        constexpr MemoryRange& operator-=(Bytes rhs) noexcept;

        /// \brief Get the base address of the range.
        /// \return Returns the base address of the range.
        constexpr MemoryAddress Begin() const noexcept;

        /// \brief Get the address past the end of this range,
        /// \return Returns the address past the end of this range.
        constexpr MemoryAddress End() const noexcept;

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

        MemoryAddress begin_;       ///< \brief First address in the memory range.

        MemoryAddress end_;         ///< \brief One past the last address in the memory range.

    };

    /// \brief Equality comparison for MemoryRange.
    /// \return Returns true if lhs and rhs refer to the same memory range, returns false otherwise.
    constexpr bool operator==(const MemoryRange& lhs, const MemoryRange& rhs) noexcept;

    /// \brief Inequality comparison for MemoryRange.
    /// \return Returns true if lhs and rhs refer to different memory ranges, returns false otherwise.
    constexpr bool operator!=(const MemoryRange& lhs, const MemoryRange& rhs) noexcept;

    /// \brief Move a memory range forward.
    /// \return Returns a memory range which is equal to lhs moved forward by rhs bytes.
    constexpr MemoryRange operator+(const MemoryRange& lhs, const Bytes& rhs) noexcept;

    /// \brief Move a memory range backward.
    /// \return Returns a memory range which is equal to lhs moved backwards by rhs bytes.
    constexpr MemoryRange operator-(const MemoryRange& lhs, const Bytes& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr MemoryRange::MemoryRange(MemoryAddress begin, MemoryAddress end)
        : begin_(begin)
        , end_(end)
    {
        SYNTROPY_ASSERT(begin <= end);
    }

    constexpr MemoryRange::operator bool() const noexcept
    {
        return end_ != begin_;
    }

    constexpr MemoryAddress MemoryRange::operator[](Bytes offset) const
    {
        auto address = begin_ + offset;

        SYNTROPY_ASSERT(Contains(address));

        return address;
    }

    constexpr MemoryRange& MemoryRange::operator+=(Bytes rhs) noexcept
    {
        begin_ += rhs;
        end_ += rhs;
        return *this;
    }

    constexpr MemoryRange& MemoryRange::operator-=(Bytes rhs) noexcept
    {
        SYNTROPY_ASSERT(uintptr_t(begin_) >= std::size_t(rhs));

        begin_ -= rhs;
        end_ -= rhs;
        return *this;
    }

    constexpr MemoryAddress MemoryRange::Begin() const noexcept
    {
        return begin_;
    }

    constexpr MemoryAddress MemoryRange::End() const noexcept
    {
        return end_;
    }

    constexpr Bytes MemoryRange::GetSize() const noexcept
    {
        return Bytes(std::size_t(end_ - begin_));
    }

    constexpr bool MemoryRange::Contains(const MemoryRange& memory_range) const noexcept
    {
        return begin_ <= memory_range.begin_ && memory_range.end_ <= end_;
    }

    constexpr bool MemoryRange::Contains(MemoryAddress address) const noexcept
    {
        return begin_ <= address && address < end_;
    }

    constexpr bool operator==(const MemoryRange& lhs, const MemoryRange& rhs) noexcept
    {
        return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
    }

    constexpr bool operator!=(const MemoryRange& lhs, const MemoryRange& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr MemoryRange operator+(const MemoryRange& lhs, const Bytes& rhs) noexcept
    {
        return MemoryRange(lhs) += rhs;
    }

    constexpr MemoryRange operator-(const MemoryRange& lhs, const Bytes& rhs) noexcept
    {
        return MemoryRange(lhs) -= rhs;
    }

}