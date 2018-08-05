
/// \file virtual_memory_range.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definitions for virtual memory ranges and automatic virtual memory management.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_range.h"
#include "memory/virtual_memory.h"

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RANGE                                                 */
    /************************************************************************/

    /// \brief Represents a range of virtual memory addresses.
    /// RAII: the range is reserved when the range is created and released upon destruction.
    /// Addresses in the range must be committed\decommitted manually.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryRange
    {
    public:

        /// \brief Create a new empty memory range.
        VirtualMemoryRange() = default;

        /// \brief Create a new virtual memory range.
        /// \param size Size of the range, in bytes.
        VirtualMemoryRange(Bytes size);

        /// \brief Create a new aligned virtual memory range.
        /// \param size Size of the range, in bytes.
        /// \param alignment Alignment of the range, in bytes.
        VirtualMemoryRange(Bytes size, Alignment alignment);

        /// \brief No copy ctor.
        VirtualMemoryRange(const VirtualMemoryRange&) = delete;

        /// \brief Move ctor.
        VirtualMemoryRange(VirtualMemoryRange&& other);

        /// \brief Default destructor.
        ~VirtualMemoryRange();

        /// \brief No assignment operator.
        VirtualMemoryRange& operator=(const VirtualMemoryRange&) = delete;

        /// \brief Get the underlying aligned memory range.
        operator const MemoryRange&() const noexcept;

    private:

        MemoryRange memory_range_;              ///< \brief Underlying memory range, accounts for any alignment.

        MemoryRange aligned_memory_range_;      ///< \brief Aligned memory range. This range is fully contained within memory_range_.

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline VirtualMemoryRange::VirtualMemoryRange(Bytes size)
        : memory_range_(VirtualMemory::Reserve(size))
        , aligned_memory_range_(memory_range_)
    {
        SYNTROPY_ASSERT(memory_range_);
    }

    inline VirtualMemoryRange::VirtualMemoryRange(Bytes size, Alignment alignment)
        : memory_range_(VirtualMemory::Reserve(size + alignment - 1_Bytes))
        , aligned_memory_range_(memory_range_.GetBase().GetAligned(alignment), size)
    {
        SYNTROPY_ASSERT(memory_range_);
        SYNTROPY_ASSERT(memory_range_.Contains(aligned_memory_range_));
    }

    inline VirtualMemoryRange::VirtualMemoryRange(VirtualMemoryRange&& other)
    {
        std::swap(memory_range_, other.memory_range_);
        std::swap(aligned_memory_range_, other.aligned_memory_range_);
    }

    inline VirtualMemoryRange::~VirtualMemoryRange()
    {
        if (memory_range_)
        {
            VirtualMemory::Release(memory_range_);
        }
    }

    inline VirtualMemoryRange::operator const MemoryRange&() const noexcept
    {
        return aligned_memory_range_;
    }

}