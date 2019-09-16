
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
    template <bool is_const>
    class MemoryRangeT
    {
    public:

        /// \brief Create an empty memory range.
        constexpr MemoryRangeT() = default;

        /// \brief Create a memory range.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        constexpr MemoryRangeT(MemoryAddressT<is_const> begin, MemoryAddressT<is_const> end);

        /// \brief Default copy-constructor.
        template <bool is_rhs_const>
        constexpr MemoryRangeT(const MemoryRangeT<is_rhs_const>& rhs);

        /// \brief Default assignment operator.
        constexpr MemoryRangeT& operator=(const MemoryRangeT&) = default;

        /// \brief Check whether the range is non-empty.
        /// \return Returns true if the range is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        constexpr MemoryAddressT<is_const> operator[](Bytes offset) const;

        /// \brief Advance the memory range forward.
        /// \param rhs Number of bytes to move the range forward to.
        /// \return Returns a reference to this element.
        constexpr MemoryRangeT& operator+=(Bytes rhs) noexcept;

        /// \brief Move the memory range backwards.
        /// \param rhs Number of bytes to move the range backwards to.
        /// \return Returns a reference to this element.
        constexpr MemoryRangeT& operator-=(Bytes rhs) noexcept;

        /// \brief Get the base address of the range.
        /// \return Returns the base address of the range.
        constexpr MemoryAddressT<is_const> Begin() const noexcept;

        /// \brief Get the address past the end of this range,
        /// \return Returns the address past the end of this range.
        constexpr MemoryAddressT<is_const> End() const noexcept;

        /// \brief Get the size of the range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        constexpr Bytes GetSize() const noexcept;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this memory range, returns false otherwise.
        constexpr bool Contains(const MemoryRangeT<false>& memory_range) const noexcept;

        /// \brief Check whether an address falls within this memory range.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory range, returns false otherwise.
        constexpr bool Contains(ConstMemoryAddress address) const noexcept;

    private:

        MemoryAddressT<is_const> begin_;       ///< \brief First address in the memory range.

        MemoryAddressT<is_const> end_;         ///< \brief One past the last address in the memory range.

    };

    /// \brief Type alias for non-const memory ranges.
    using MemoryRange = MemoryRangeT<false>;

    /// \brief Type alias for const memory ranges.
    using ConstMemoryRange = MemoryRangeT<true>;

    /// \brief Equality comparison for MemoryRange.
    /// \return Returns true if lhs and rhs refer to the same memory range, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator==(const MemoryRangeT<is_lhs_const>& lhs, const MemoryRangeT<is_rhs_const>& rhs) noexcept;

    /// \brief Inequality comparison for MemoryRange.
    /// \return Returns true if lhs and rhs refer to different memory ranges, returns false otherwise.
    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator!=(const MemoryRangeT<is_lhs_const>& lhs, const MemoryRangeT<is_rhs_const>& rhs) noexcept;

    /// \brief Move a memory range forward.
    /// \return Returns a memory range which is equal to lhs moved forward by rhs bytes.
    template <bool is_const>
    constexpr MemoryRangeT<is_const> operator+(const MemoryRangeT<is_const>& lhs, const Bytes& rhs) noexcept;

    /// \brief Move a memory range backward.
    /// \return Returns a memory range which is equal to lhs moved backwards by rhs bytes.
    template <bool is_const>
    constexpr MemoryRangeT<is_const> operator-(const MemoryRangeT<is_const>& lhs, const Bytes& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <bool is_const>
    constexpr MemoryRangeT<is_const>::MemoryRangeT(MemoryAddressT<is_const> begin, MemoryAddressT<is_const> end)
        : begin_(begin)
        , end_(end)
    {
        SYNTROPY_ASSERT(begin <= end);
    }

    template <bool is_const>
    template <bool is_rhs_const>
    constexpr MemoryRangeT<is_const>::MemoryRangeT(const MemoryRangeT<is_rhs_const>& rhs)
        : begin_(rhs.Begin())
        , end_(rhs.End())
    {

    }

    template <bool is_const>
    constexpr MemoryRangeT<is_const>::operator bool() const noexcept
    {
        return end_ != begin_;
    }

    template <bool is_const>
    constexpr MemoryAddressT<is_const> MemoryRangeT<is_const>::operator[](Bytes offset) const
    {
        auto address = begin_ + offset;

        SYNTROPY_ASSERT(Contains(address));

        return address;
    }

    template <bool is_const>
    constexpr MemoryRangeT<is_const>& MemoryRangeT<is_const>::operator+=(Bytes rhs) noexcept
    {
        begin_ += rhs;
        end_ += rhs;
        return *this;
    }

    template <bool is_const>
    constexpr MemoryRangeT<is_const>& MemoryRangeT<is_const>::operator-=(Bytes rhs) noexcept
    {
        SYNTROPY_ASSERT(uintptr_t(begin_) >= std::size_t(rhs));

        begin_ -= rhs;
        end_ -= rhs;
        return *this;
    }

    template <bool is_const>
    constexpr MemoryAddressT<is_const> MemoryRangeT<is_const>::Begin() const noexcept
    {
        return begin_;
    }

    template <bool is_const>
    constexpr MemoryAddressT<is_const> MemoryRangeT<is_const>::End() const noexcept
    {
        return end_;
    }

    template <bool is_const>
    constexpr Bytes MemoryRangeT<is_const>::GetSize() const noexcept
    {
        return Bytes(std::size_t(end_ - begin_));
    }

    template <bool is_const>
    constexpr bool MemoryRangeT<is_const>::Contains(const MemoryRangeT<false>& memory_range) const noexcept
    {
        return begin_ <= memory_range.Begin() && memory_range.End() <= end_;
    }

    template <bool is_const>
    constexpr bool MemoryRangeT<is_const>::Contains(ConstMemoryAddress address) const noexcept
    {
        return begin_ <= address && address < end_;
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator==(const MemoryRangeT<is_lhs_const>& lhs, const MemoryRangeT<is_rhs_const>& rhs) noexcept
    {
        return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
    }

    template <bool is_lhs_const, bool is_rhs_const>
    constexpr bool operator!=(const MemoryRangeT<is_lhs_const>& lhs, const MemoryRangeT<is_rhs_const>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <bool is_const>
    constexpr MemoryRangeT<is_const> operator+(const MemoryRangeT<is_const>& lhs, const Bytes& rhs) noexcept
    {
        return MemoryRangeT(lhs) += rhs;
    }

    template <bool is_const>
    constexpr MemoryRangeT<is_const> operator-(const MemoryRangeT<is_const>& lhs, const Bytes& rhs) noexcept
    {
        return MemoryRangeT(lhs) -= rhs;
    }

}