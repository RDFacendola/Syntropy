
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
        VirtualMemoryRange() = default;

        /// \brief Default copy constructor.
        VirtualMemoryRange(const VirtualMemoryRange&) = default;

        /// \brief Create a virtual memory range from a range of virtual memory pages.
        /// \param begin First page in the range.
        /// \param top One past the last page in the range.
        VirtualMemoryRange(const VirtualMemoryPage& begin, const VirtualMemoryPage& end);

        /// \brief Create a virtual memory range from a memory range.
        /// \param memory_range Memory range, must represent a full range of virtual memory pages.
        VirtualMemoryRange(const MemoryRange& memory_range);

        /// \brief Default assignment operator.
        VirtualMemoryRange& operator=(const VirtualMemoryRange&) = default;

        /// \brief Check whether the range is non-empty.
        /// \return Returns true if the range is non-empty, returns false otherwise.
        operator bool() const noexcept;

        /// \brief Get the memory range this virtual memory range refers to.
        /// \return Returns the memory range this virtual memory range refers to.
        operator MemoryRange() const noexcept;

        /// \brief Access a page in the range by index.
        /// \return Returns the page-index-th page after the first one in this range.
        const VirtualMemoryPage& operator[](std::int64_t page_index) const;

        /// \brief Get the first memory page in the range.
        /// \return Returns the first memory page in the range.
        const VirtualMemoryPage& Begin() const noexcept;

        /// \brief Get the one past the last memory page in the range.
        /// \return Returns one past the last memory page in the range.
        const VirtualMemoryPage& End() const noexcept;

        /// \brief Get the number of pages in this range.
        /// \return Returns the total number of pages in this range.
        std::size_t GetSize() const noexcept;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this virtual memory range, returns false otherwise.
        bool Contains(const MemoryRange& memory_range) const noexcept;

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

    /// \brief Equality comparison for VirtualMemoryRange.
    /// \return Returns true if lhs and rhs refer to the same virtual memory range, returns false otherwise.
    bool operator==(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept;

    /// \brief Inequality comparison for VirtualMemoryRange.
    /// \return Returns true if lhs and rhs refer to different virtual memory ranges, returns false otherwise.
    bool operator!=(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryRange.

    inline VirtualMemoryRange::VirtualMemoryRange(const VirtualMemoryPage& begin, const VirtualMemoryPage& end)
        : begin_(begin)
        , end_(end)
    {
        SYNTROPY_ASSERT(begin <= end);
    }

    inline VirtualMemoryRange::VirtualMemoryRange(const MemoryRange& memory_range)
        : VirtualMemoryRange(memory_range.Begin(), memory_range.End())
    {

    }

    inline VirtualMemoryRange::operator bool() const noexcept
    {
        return end_ != begin_;
    }

    inline VirtualMemoryRange::operator MemoryRange() const noexcept
    {
        return MemoryRange(begin_.Begin(), end_.Begin());
    }

    inline const VirtualMemoryPage& VirtualMemoryRange::operator[](std::int64_t page_index) const
    {
        auto page = begin_ + page_index;

        SYNTROPY_ASSERT(Contains(page));

        return page;
    }

    inline const VirtualMemoryPage& VirtualMemoryRange::Begin() const noexcept
    {
        return begin_;
    }

    inline const VirtualMemoryPage& VirtualMemoryRange::End() const noexcept
    {
        return end_;
    }

    inline std::size_t VirtualMemoryRange::GetSize() const noexcept
    {
        return end_ - begin_;
    }

    inline bool VirtualMemoryRange::Contains(const MemoryRange& memory_range) const noexcept
    {
        return begin_.Begin() <= memory_range.Begin() && memory_range.End() <= end_.Begin();
    }

    inline bool VirtualMemoryRange::Commit() const
    {
        return VirtualMemory::Commit(*this);
    }

    inline bool VirtualMemoryRange::Decommit() const
    {
        return VirtualMemory::Decommit(*this);
    }

    inline bool operator==(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept
    {
        return (lhs.Begin() == rhs.Begin()) && (lhs.End() == rhs.End());
    }

    inline bool operator!=(const VirtualMemoryRange& lhs, const VirtualMemoryRange& rhs) noexcept
    {
        return !(lhs == rhs);
    }

}
