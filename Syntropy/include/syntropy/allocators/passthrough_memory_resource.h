
/// \file passthrough_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources used to forward calls to other memory resources.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/core/types.h"
#include "syntropy/allocators/null_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* PASSTHROUGH MEMORY RESOURCE <TMEMORY RESOURCE>                       */
    /************************************************************************/

    /// \brief Tier Omega memory resource that forwards requests to an underlying memory resource.
    /// If no memory resource is bound, this memory resource behaves like a syntropy::NullMemoryResource.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TMemoryResource>
    class PassthroughMemoryResource : private NullMemoryResource
    {
    public:

        /// \brief Default constructor.
        PassthroughMemoryResource() noexcept = default;

        /// \brief Create a new pass-through memory resource referencing another memory resource.
        /// \param memory_Memory resource the allocation requests are relayed to.
        PassthroughMemoryResource(TMemoryResource& memory_resource);

        /// \brief Default copy constructor.
        PassthroughMemoryResource(const PassthroughMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        PassthroughMemoryResource(PassthroughMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~PassthroughMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        PassthroughMemoryResource& operator=(const PassthroughMemoryResource&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory resource is assigned, relay this function call to that, otherwise this memory resource behaves as the null memory resource.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the memory resource owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

    private:

        ///< \brief Underlying memory resource.
        Pointer<TMemoryResource> memory_resource_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PassthroughMemoryResource<TMemoryResource>.
    // ===========================================

    template <typename TMemoryResource>
    PassthroughMemoryResource<TMemoryResource>::PassthroughMemoryResource(TMemoryResource& memory_resource)
        : memory_resource_(std::addressof(memory_resource))
    {

    }

    template <typename TMemoryResource>
    inline RWByteSpan PassthroughMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (memory_resource_)
        {
            return memory_resource_->Allocate(size, alignment);
        }

        return NullMemoryResource::Allocate(size);
    }

    template <typename TMemoryResource>
    inline void PassthroughMemoryResource<TMemoryResource>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        if (memory_resource_)
        {
            memory_resource_->Deallocate(block, alignment);
        }
        else
        {
            NullMemoryResource::Deallocate(block, alignment);
        }
    }

    template <typename TMemoryResource>
    inline Bool PassthroughMemoryResource<TMemoryResource>::Owns(const ByteSpan& block) const noexcept
    {
        return memory_resource_ ? memory_resource_->Owns(block) : NullMemoryResource::Owns(block);
    }

}
