
/// \file passthrough_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources used to forward calls to other memory resources.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/allocators/null_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* PASSTHROUGH MEMORY RESOURCE                                          */
    /************************************************************************/

    /// \brief Memory resource that relays allocation requests to another memory resource.
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
        /// \param size Size of the memory block to allocate.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate. Expects an empty range.
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Expects an empty range.
        /// \param alignment Block alignment.
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        ///< \brief Underlying memory resource.
        TMemoryResource* memory_resource_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PassthroughMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    PassthroughMemoryResource<TMemoryResource>::PassthroughMemoryResource(TMemoryResource& memory_resource)
        : memory_resource_(std::addressof(memory_resource))
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange PassthroughMemoryResource<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        if (memory_resource_)
        {
            return memory_resource_->Allocate(size);
        }

        return NullMemoryResource::Allocate(size);
    }

    template <typename TMemoryResource>
    inline MemoryRange PassthroughMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (memory_resource_)
        {
            return memory_resource_->Allocate(size, alignment);
        }

        return NullMemoryResource::Allocate(size);
    }

    template <typename TMemoryResource>
    inline void PassthroughMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        if (memory_resource_)
        {
            memory_resource_->Deallocate(block);
        }
        else
        {
            NullMemoryResource::Deallocate(block);
        }
    }

    template <typename TMemoryResource>
    inline void PassthroughMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
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
    inline bool PassthroughMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_ ? memory_resource_->Owns(block) : NullMemoryResource::Owns(block);
    }

    template <typename TMemoryResource>
    inline Bytes PassthroughMemoryResource<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return memory_resource_ ? memory_resource_->GetMaxAllocationSize() : NullMemoryResource::GetMaxAllocationSize();
    }

}