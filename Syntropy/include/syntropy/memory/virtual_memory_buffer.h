
/// \file virtual_memory_buffer.h
/// \brief This header is part of the syntropy memory management system. It contains classes and functionalities for automatic virtual memory management.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY BUFFER                                                */
    /************************************************************************/

    /// \brief Represents a virtual memory buffer that is reserved during construction and released upon destruction via RAII paradigm.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryBuffer
    {
    public:

        /// \brief Create a new empty virtual memory buffer.
        VirtualMemoryBuffer() = default;

        /// \brief Create a new virtual memory buffer.
        /// \param size Size of the buffer, in bytes. Must be a multiple of the system virtual page size.
        VirtualMemoryBuffer(Bytes size);

        /// \brief No copy constructor.
        VirtualMemoryBuffer(const VirtualMemoryBuffer&) = delete;

        /// \brief Move constructor.
        /// Assign the virtual memory buffer of another instance to this one.
        /// \param rhs Virtual memory buffer to move.
        VirtualMemoryBuffer(VirtualMemoryBuffer&& rhs);

        /// \brief Destructor.
        /// Releases any reserved memory.
        ~VirtualMemoryBuffer();

        /// \brief Unified assignment operator.
        VirtualMemoryBuffer& operator=(VirtualMemoryBuffer rhs) noexcept;

        /// \brief Get the underlying virtual memory range.
        operator const VirtualMemoryRange&() noexcept;

        /// \brief Get the underlying memory range.
        operator const MemoryRange&() const noexcept;

        /// \brief Check whether a memory range is contained entirely inside this buffer.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this virtual memory buffer, returns false otherwise.
        bool Contains(const MemoryRange& memory_range) const noexcept;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        Bytes GetSize() const noexcept;

        /// \brief Swap the content of this buffer with another one.
        void Swap(VirtualMemoryBuffer& rhs) noexcept;

    private:

        VirtualMemoryRange virtual_memory_range_;           ///< \brief Underlying virtual memory range.

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline VirtualMemoryBuffer::VirtualMemoryBuffer(Bytes size)
        : virtual_memory_range_(VirtualMemory::Reserve(size))
    {

    }

    inline VirtualMemoryBuffer::VirtualMemoryBuffer(VirtualMemoryBuffer&& rhs)
        : virtual_memory_range_(rhs.virtual_memory_range_)
    {
        rhs.virtual_memory_range_ = VirtualMemoryRange();
    }

    inline VirtualMemoryBuffer::~VirtualMemoryBuffer()
    {
        VirtualMemory::Release(virtual_memory_range_);
    }

    inline VirtualMemoryBuffer& VirtualMemoryBuffer::operator=(VirtualMemoryBuffer rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    inline VirtualMemoryBuffer::operator const VirtualMemoryRange&() noexcept
    {
        return virtual_memory_range_;
    }

    inline VirtualMemoryBuffer::operator const MemoryRange&() const noexcept
    {
        return virtual_memory_range_;
    }

    inline bool VirtualMemoryBuffer::Contains(const MemoryRange& memory_range) const noexcept
    {
        return virtual_memory_range_.Contains(memory_range);
    }

    inline Bytes VirtualMemoryBuffer::GetSize() const noexcept
    {
        return virtual_memory_range_.GetSize() * VirtualMemory::GetPageSize();
    }

    inline void VirtualMemoryBuffer::Swap(VirtualMemoryBuffer& rhs) noexcept
    {
        std::swap(virtual_memory_range_, rhs.virtual_memory_range_);
    }
}

