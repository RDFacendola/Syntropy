
/// \file virtual_memory_range.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for virtual memory ranges.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/virtual_memory_page.h"
#include "syntropy/memory/virtual_memory.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RANGE                                                 */
    /************************************************************************/

    /// \brief Represents a range of contiguous virtual memory pages.
    /// The range is of the form [base; top)
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryRange
    {
    public:

        /// \brief Create an empty virtual memory range.
        constexpr VirtualMemoryRange() = default;

        /// \brief Default copy constructor.
        constexpr VirtualMemoryRange(const VirtualMemoryRange&) = default;

        /// \brief Create a virtual memory range from a range of virtual memory pages.
        /// \param begin First page in the range.
        /// \param top One past the last page in the range.
        constexpr VirtualMemoryRange(const VirtualMemoryPage& begin, const VirtualMemoryPage& end);

        /// \brief Create a virtual memory range from a memory range.
        /// \param memory_range Memory range, must represent a full range of virtual memory pages.
        constexpr VirtualMemoryRange(const MemoryRange& memory_range);

        /// \brief Default assignment operator.
        constexpr VirtualMemoryRange& operator=(const VirtualMemoryRange&) = default;

        /// \brief Check whether the range is non-empty.
        /// \return Returns true if the range is non-empty, returns false otherwise.
        constexpr operator bool() const noexcept;

        /// \brief Get the memory range this virtual memory range refers to.
        /// \return Returns the memory range this virtual memory range refers to.
        constexpr operator MemoryRange() const noexcept;

        /// \brief Access a page in the range.
        /// \param offset Offset with respect to the first page of the range.
        /// \return Returns the offset-th page after the first one in this range.
        constexpr const VirtualMemoryPage& operator[](std::size_t offset) const;

        /// \brief Advance the virtual memory range forward.
        /// \param rhs Number of pages to move the range forward to.
        /// \return Returns a reference to this element.
        constexpr VirtualMemoryRange& operator+=(std::size_t rhs) noexcept;

        /// \brief Move the virtual memory range backwards.
        /// \param rhs Number of pages to move the range backwards to.
        /// \return Returns a reference to this element.
        constexpr VirtualMemoryRange& operator-=(std::size_t rhs) noexcept;

        /// \brief Get the first memory page in the range.
        /// \return Returns the first memory page in the range.
        constexpr const VirtualMemoryPage& Begin() const noexcept;

        /// \brief Get the one past the last memory page in the range.
        /// \return Returns one past the last memory page in the range.
        constexpr const VirtualMemoryPage& End() const noexcept;

        /// \brief Get the number of pages in this range.
        /// \return Returns the total number of pages in this range.
        constexpr std::size_t GetSize() const noexcept;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this virtual memory range, returns false otherwise.
        constexpr bool Contains(const MemoryRange& memory_range) const noexcept;

        /// \brief Commit the virtual memory range making it accessible by the application.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        bool Commit() const;

        /// \brief Decommit the virtual memory range making it inaccessible by the application.
        /// \return Returns true if the memory could be decommitted, returns false otherwise.
        bool Decommit() const;

    private:

        VirtualMemoryPage begin_;           ///< \brief First virtual memory page in the range.

        VirtualMemoryPage end_;             ///< \brief One past the last memory page in the range.

    };

    /// \brief Equality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to the same virtual memory range, returns false otherwise.
    constexpr bool operator==(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Inequality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to different virtual memory ranges, returns false otherwise.
    constexpr bool operator!=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Move a virtual memory range forward.
    /// \return Returns a virtual memory range which is equal to lhs moved forward by rhs pages.
    constexpr VirtualMemoryPage operator+(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /// \brief Move a virtual memory range backward.
    /// \return Returns a virtual memory range which is equal to lhs moved backwards by rhs pages.
    constexpr VirtualMemoryPage operator-(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr VirtualMemoryRange::VirtualMemoryRange(const VirtualMemoryPage& begin, const VirtualMemoryPage& end)
        : begin_(begin)
        , end_(end)
    {
        SYNTROPY_ASSERT(begin <= end);
    }

    constexpr VirtualMemoryRange::VirtualMemoryRange(const MemoryRange& memory_range)
        : VirtualMemoryRange(memory_range.Begin(), memory_range.End())
    {

    }

    constexpr VirtualMemoryRange::operator bool() const noexcept
    {
        return end_ != begin_;
    }

    constexpr VirtualMemoryRange::operator MemoryRange() const noexcept
    {
        return MemoryRange(begin_.Begin(), end_.End());
    }

    constexpr const VirtualMemoryPage& VirtualMemoryRange::operator[](std::size_t offset) const
    {
        auto page = begin_ + offset;

        SYNTROPY_ASSERT(Contains(page));

        return page;
    }

    constexpr VirtualMemoryRange& VirtualMemoryRange::operator+=(std::size_t rhs) noexcept
    {
        begin_ += rhs;
        end_ += rhs;
        return *this;
    }

    constexpr VirtualMemoryRange& VirtualMemoryRange::operator-=(std::size_t rhs) noexcept
    {
        begin_ -= rhs;
        end_ -= rhs;
        return *this;
    }

    constexpr const VirtualMemoryPage& VirtualMemoryRange::Begin() const noexcept
    {
        return begin_;
    }

    constexpr const VirtualMemoryPage& VirtualMemoryRange::End() const noexcept
    {
        return end_;
    }

    constexpr std::size_t VirtualMemoryRange::GetSize() const noexcept
    {
        return end_ - begin_;
    }

    constexpr bool VirtualMemoryRange::Contains(const MemoryRange& memory_range) const noexcept
    {
        return begin_ <= memory_range.Begin() && memory_range.End() <= end_;
    }

    inline bool VirtualMemoryRange::Commit() const
    {
        return VirtualMemory::Commit(*this);
    }

    inline bool VirtualMemoryRange::Decommit() const
    {
        return VirtualMemory::Decommit(*this);
    }

    constexpr bool operator==(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept
    {
        return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
    }

    constexpr bool operator!=(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr VirtualMemoryRange operator+(const VirtualMemoryRange& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryRange(lhs) += rhs;
    }

    constexpr VirtualMemoryRange operator-(const VirtualMemoryRange& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryRange(lhs) -= rhs;
    }
}
