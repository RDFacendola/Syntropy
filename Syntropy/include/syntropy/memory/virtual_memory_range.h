
/// \file virtual_memory_range.h
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
    /* VIRTUAL MEMORY RANGE                                                 */
    /************************************************************************/

    /// \brief Represents a virtual memory range that is reserved during construction and released upon destruction via RAII paradigm.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryRange
    {
    public:

        /// \brief Create a new empty virtual memory buffer.
        VirtualMemoryRange() noexcept = default;

        /// \brief Create a new virtual memory buffer.
        /// \param size Size of the buffer, in bytes. Must be a multiple of the system virtual page size.
        VirtualMemoryRange(Bytes size) noexcept;

        /// \brief Take ownership of the provided byte span.
        explicit VirtualMemoryRange(const RWByteSpan& byte_span) noexcept;

        /// \brief No copy constructor.
        VirtualMemoryRange(const VirtualMemoryRange&) = delete;

        /// \brief Move constructor.
        /// Assign the virtual memory buffer of another instance to this one.
        /// \param rhs Virtual memory buffer to move.
        VirtualMemoryRange(VirtualMemoryRange&& rhs) noexcept;

        /// \brief Release reserved virtual memory.
        ~VirtualMemoryRange();

        /// \brief Unified assignment operator.
        VirtualMemoryRange& operator=(VirtualMemoryRange rhs) noexcept;

        /// \brief Swap the content of this buffer with another one.
        void Swap(VirtualMemoryRange& rhs) noexcept;

        /// \brief Access the underlying memory.
        RWByteSpan GetData() const noexcept;

    private:

        /// \brief Underlying memory range.
        RWByteSpan byte_span_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VirtualMemoryRange.

    inline VirtualMemoryRange::VirtualMemoryRange(Bytes size) noexcept
        : byte_span_(VirtualMemory::Reserve(size))
    {

    }

    inline VirtualMemoryRange::VirtualMemoryRange(const RWByteSpan& byte_span) noexcept
        : byte_span_(byte_span)
    {

    }

    inline VirtualMemoryRange::VirtualMemoryRange(VirtualMemoryRange&& rhs) noexcept
        : byte_span_(rhs.byte_span_)
    {
        rhs.byte_span_ = {};
    }

    inline VirtualMemoryRange::~VirtualMemoryRange()
    {
        VirtualMemory::Release(byte_span_);
    }

    inline VirtualMemoryRange& VirtualMemoryRange::operator=(VirtualMemoryRange rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    inline void VirtualMemoryRange::Swap(VirtualMemoryRange& rhs) noexcept
    {
        std::swap(byte_span_, rhs.byte_span_);
    }

    inline RWByteSpan VirtualMemoryRange::GetData() const noexcept
    {
        return byte_span_;
    }
}

