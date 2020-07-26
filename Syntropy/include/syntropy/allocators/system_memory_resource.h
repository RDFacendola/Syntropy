
/// \file system_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources using system heap memory.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace syntropy
{
    /************************************************************************/
    /* SYSTEM MEMORY RESOURCE                                               */
    /************************************************************************/

    /// \brief Tier 0 memory resource used to allocate memory on system heap via new\delete calls.
    /// \author Raffaele D. Facendola - February 2017
    class SystemMemoryResource
    {
    public:

        /// \brief Default constructor.
        SystemMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        SystemMemoryResource(const SystemMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        SystemMemoryResource(SystemMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~SystemMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        SystemMemoryResource& operator=(const SystemMemoryResource&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the memory resource owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SystemMemoryResource.
    // =====================

    inline RWByteSpan SystemMemoryResource::Allocate(Bytes size, Alignment alignment) noexcept
    {
        auto size_value = static_cast<std::size_t>(ToInt(size));
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        if (auto block = ::operator new(size_value, alignment_value, std::nothrow))
        {
            return { ToRWBytePtr(block), ToInt(size) };
        }

        return {};
    }

    inline void SystemMemoryResource::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        auto alignment_value = static_cast<std::align_val_t>(ToInt(alignment));

        ::operator delete(block.GetData(), alignment_value, std::nothrow);
    }

    inline Bool SystemMemoryResource::Owns(const ByteSpan& block) const noexcept
    {
        // This is not correct, however system memory resource is expected to be used as last resort 
        // to other memory resources or used as the single allocator for the application.

        return true;
    }

}


