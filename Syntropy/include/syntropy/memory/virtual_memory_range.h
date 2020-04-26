
/// \file virtual_memory_range.h
/// \brief This header is part of the Syntropy memory module. It contains classes and functionalities for automatic virtual memory management.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RANGE                                                 */
    /************************************************************************/

    /// \brief Represents a virtual memory range that is reserved during construction and released upon destruction via RAII paradigm.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryRange
    {
    public:

        /// \brief Create a new empty virtual memory buffer.
        VirtualMemoryRange() = default;

        /// \brief Create a new virtual memory buffer.
        /// \param size Size of the buffer, in bytes. Must be a multiple of the system virtual page size.
        VirtualMemoryRange(Bytes size);

        /// \brief Take ownership of the provided memory range.
        explicit VirtualMemoryRange(const MemoryRange& range);

        /// \brief No copy constructor.
        VirtualMemoryRange(const VirtualMemoryRange&) = delete;

        /// \brief Move constructor.
        /// Assign the virtual memory buffer of another instance to this one.
        /// \param rhs Virtual memory buffer to move.
        VirtualMemoryRange(VirtualMemoryRange&& rhs);

        /// \brief Release reserved virtual memory.
        ~VirtualMemoryRange();

        /// \brief Unified assignment operator.
        VirtualMemoryRange& operator=(VirtualMemoryRange rhs) noexcept;

        /// \brief Get the first address in the range.
        MemoryAddress Begin() const;

        /// \brief Get one past the last address in the range.
        MemoryAddress End() const;

        /// \brief Check whether a memory range is contained entirely inside this buffer.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this virtual memory buffer, returns false otherwise.
        bool Contains(const MemoryRange& memory_range) const noexcept;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const noexcept;

        /// \brief Swap the content of this buffer with another one.
        void Swap(VirtualMemoryRange& rhs) noexcept;

        /// \brief Get the underlying memory range.
        operator MemoryRange() const noexcept;

        /// \brief Access the underlying memory range
        MemoryRange GetRange() const noexcept;

    private:

        /// \brief Underlying memory range.
        MemoryRange range_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryRange.

    inline VirtualMemoryRange::VirtualMemoryRange(Bytes size)
        : range_(VirtualMemory::Reserve(size))
    {

    }

    inline VirtualMemoryRange::VirtualMemoryRange(const MemoryRange& range)
        : range_(range)
    {

    }

    inline VirtualMemoryRange::VirtualMemoryRange(VirtualMemoryRange&& rhs)
        : range_(rhs.range_)
    {
        rhs.range_ = {};
    }

    inline VirtualMemoryRange::~VirtualMemoryRange()
    {
        VirtualMemory::Release(range_);
    }

    inline VirtualMemoryRange& VirtualMemoryRange::operator=(VirtualMemoryRange rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline MemoryAddress VirtualMemoryRange::Begin() const
    {
        return range_.Begin();
    }

    inline MemoryAddress VirtualMemoryRange::End() const
    {
        return range_.End();
    }

    inline bool VirtualMemoryRange::Contains(const MemoryRange& memory_range) const noexcept
    {
        return range_.Contains(memory_range);
    }

    inline Bytes VirtualMemoryRange::GetSize() const noexcept
    {
        return range_.GetSize();
    }

    inline void VirtualMemoryRange::Swap(VirtualMemoryRange& rhs) noexcept
    {
        std::swap(range_, rhs.range_);
    }

    inline VirtualMemoryRange::operator MemoryRange() const noexcept
    {
        return  range_;
    }

    inline MemoryRange VirtualMemoryRange::GetRange() const noexcept
    {
        return range_;
    }
}

