
/// \file virtual_buffer.h
/// \brief This header is part of the Syntropy virtual memory module. It contains classes and functionalities for automatic virtual memory management.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/virtual_memory/virtual_memory.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL BUFFER                                                       */
    /************************************************************************/

    /// \brief Represents a raw buffer mapped to a virtual memory which is reserved during construction and released upon destruction via RAII paradigm.
    /// Buffer ownership is exclusive and can only be moved to other virtual memory buffers.
    /// The buffer is aligned to system virtual memory-page boundary.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualBuffer
    {
    public:

        /// \brief Create a new empty virtual memory buffer.
        VirtualBuffer() noexcept = default;

        /// \brief Reserve a virtual memory buffer.
        /// \param size Size of the buffer, in bytes.
        /// \remarks The buffer starts uncommitted.
        VirtualBuffer(Bytes size) noexcept;

        /// \brief Take ownership of the provided virtual memory span.
        /// If the provided span wasn't allocated from system virtual memory, the result of this method is undefined.
        explicit VirtualBuffer(const RWByteSpan& byte_span) noexcept;

        /// \brief No copy constructor.
        VirtualBuffer(const VirtualBuffer&) = delete;

        /// \brief Move constructor.
        /// Assign the virtual memory buffer of another instance to this one.
        /// \param rhs Virtual memory buffer to move.
        VirtualBuffer(VirtualBuffer&& rhs) noexcept;

        /// \brief Release reserved virtual memory.
        ~VirtualBuffer() noexcept;

        /// \brief Unified assignment operator.
        VirtualBuffer& operator=(VirtualBuffer rhs) noexcept;

        /// \brief Access the underlying byte span.
        ByteSpan GetData() const noexcept;

        /// \brief Access the underlying byte span.
        RWByteSpan GetData() noexcept;

        /// \brief Swap the content of this buffer with another one.
        void Swap(VirtualBuffer& rhs) noexcept;

    private:

        /// \brief Underlying memory buffer.
        RWByteSpan buffer_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Swap two VirtualBuffer instances.
    void Swap(VirtualBuffer& lhs, VirtualBuffer& rhs) noexcept;

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

        /// \brief Exposes memory-related definitions.
    namespace Memory
    {
        /// \brief Get the memory footprint of a virtual memory buffer.
        /// \return Returns the size of a virtual memory buffer, in bytes.
        Bytes Size(const VirtualBuffer& buffer) noexcept;
    }

    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    /// \brief Exposes virtual memory-related definitions.
    namespace VirtualMemory
    {
        /// \brief Commit a virtual memory buffer.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        Bool Commit(VirtualBuffer& buffer) noexcept;

        /// \brief Decommit a virtual memory buffer.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        Bool Decommit(VirtualBuffer& buffer) noexcept;
    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualBuffer.
    // ==============

    inline VirtualBuffer::VirtualBuffer(Bytes size) noexcept
        : buffer_(VirtualMemory::Reserve(size))
    {

    }

    inline VirtualBuffer::VirtualBuffer(const RWByteSpan& byte_span) noexcept
        : buffer_(byte_span)
    {

    }

    inline VirtualBuffer::VirtualBuffer(VirtualBuffer&& rhs) noexcept
        : buffer_(rhs.buffer_)
    {
        rhs.buffer_ = {};
    }

    inline VirtualBuffer::~VirtualBuffer() noexcept
    {
        VirtualMemory::Release(buffer_);
    }

    inline VirtualBuffer& VirtualBuffer::operator=(VirtualBuffer rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualBuffer::Swap(VirtualBuffer& rhs) noexcept
    {
        std::swap(buffer_, rhs.buffer_);
    }

    inline ByteSpan VirtualBuffer::GetData() const noexcept
    {
        return buffer_;
    }

    inline RWByteSpan VirtualBuffer::GetData() noexcept
    {
        return buffer_;
    }

    // Non-member functions.
    // =====================

    inline void Swap(VirtualBuffer& lhs, VirtualBuffer& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

    // Memory.
    // =======

    inline Bytes Memory::Size(const VirtualBuffer& span) noexcept
    {
        return Memory::Size(span.GetData());
    }

    // VirtualMemory.
    // ==============

    inline Bool VirtualMemory::Commit(VirtualBuffer& buffer) noexcept
    {
        return VirtualMemory::Commit(buffer.GetData());
    }

    inline Bool VirtualMemory::Decommit(VirtualBuffer& buffer) noexcept
    {
        return VirtualMemory::Commit(buffer.GetData());
    }

}

