
/// \file virtual_memory_buffer.h
/// \brief This header is part of the Syntropy memory module. It contains classes and functionalities for automatic virtual memory management.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY BUFFER                                                */
    /************************************************************************/

    /// \brief Represents a raw buffer mapped to a virtual memory which is reserved during construction and released upon destruction via RAII paradigm.
    /// Buffer ownership is exclusive and can only be moved to other virtual memory buffers.
    /// The buffer is aligned to system virtual memory-page boundary.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryBuffer
    {
    public:

        /// \brief Create a new empty virtual memory buffer.
        VirtualMemoryBuffer() noexcept = default;

        /// \brief Reserve a virtual memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \remarks The buffer starts uncommitted.
        VirtualMemoryBuffer(Bytes size) noexcept;

        /// \brief Take ownership of the provided virtual memory span.
        /// If the provided span wasn't allocated from system virtual memory, the result of this method is undefined.
        explicit VirtualMemoryBuffer(const RWByteSpan& byte_span) noexcept;

        /// \brief No copy constructor.
        VirtualMemoryBuffer(const VirtualMemoryBuffer&) = delete;

        /// \brief Move constructor.
        /// Assign the virtual memory buffer of another instance to this one.
        /// \param rhs Virtual memory buffer to move.
        VirtualMemoryBuffer(VirtualMemoryBuffer&& rhs) noexcept;

        /// \brief Release reserved virtual memory.
        ~VirtualMemoryBuffer() noexcept;

        /// \brief Unified assignment operator.
        VirtualMemoryBuffer& operator=(VirtualMemoryBuffer rhs) noexcept;

        /// \brief Access the underlying byte span.
        ByteSpan GetData() const noexcept;

        /// \brief Access the underlying byte span.
        RWByteSpan GetData() noexcept;

        /// \brief Swap the content of this buffer with another one.
        void Swap(VirtualMemoryBuffer& rhs) noexcept;

    private:

        /// \brief Underlying memory buffer.
        RWByteSpan buffer_;

    };

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes memory-related definitions.
    namespace Memory
    {
        /// \brief Get the memory footprint of a virtual memory buffer.
        Bytes Size(const VirtualMemoryBuffer& buffer) noexcept;

        /// \brief Commit a virtual memory buffer.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        Bool Commit(VirtualMemoryBuffer& buffer) noexcept;

        /// \brief Decommit a virtual memory buffer.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        Bool Decommit(VirtualMemoryBuffer& buffer) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryBuffer.
    // ====================

    inline VirtualMemoryBuffer::VirtualMemoryBuffer(Bytes size) noexcept
        : buffer_(Memory::Reserve(size))
    {

    }

    inline VirtualMemoryBuffer::VirtualMemoryBuffer(const RWByteSpan& byte_span) noexcept
        : buffer_(byte_span)
    {

    }

    inline VirtualMemoryBuffer::VirtualMemoryBuffer(VirtualMemoryBuffer&& rhs) noexcept
        : buffer_(rhs.buffer_)
    {
        rhs.buffer_ = {};
    }

    inline VirtualMemoryBuffer::~VirtualMemoryBuffer() noexcept
    {
        Memory::Release(buffer_);
    }

    inline VirtualMemoryBuffer& VirtualMemoryBuffer::operator=(VirtualMemoryBuffer rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualMemoryBuffer::Swap(VirtualMemoryBuffer& rhs) noexcept
    {
        std::swap(buffer_, rhs.buffer_);
    }

    inline ByteSpan VirtualMemoryBuffer::GetData() const noexcept
    {
        return buffer_;
    }

    inline RWByteSpan VirtualMemoryBuffer::GetData() noexcept
    {
        return buffer_;
    }

    // Memory.
    // =======

    inline Bytes Memory::Size(const VirtualMemoryBuffer& span) noexcept
    {
        return Memory::Size(span.GetData());
    }

    inline Bool Memory::Commit(VirtualMemoryBuffer& buffer) noexcept
    {
        return Memory::Commit(buffer.GetData());
    }

    inline Bool Memory::Decommit(VirtualMemoryBuffer& buffer) noexcept
    {
        return Memory::Commit(buffer.GetData());
    }

}

