
/// \file stack_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources using system stack memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK MEMORY RESOURCE                                                */
    /************************************************************************/

    /// \brief Tier 0 memory resource used to allocate a single block of memory on system stack.
    /// \author Raffaele D. Facendola - August 2018
    template <std::size_t kSize, std::size_t kAlignment = alignof(void*)>
    class StackMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        StackMemoryResource() noexcept = default;

        /// \brief No copy constructor.
        StackMemoryResource(const StackMemoryResource&) = delete;

        /// \brief No move constructor.
        StackMemoryResource(StackMemoryResource&& rhs) = delete;

        /// \brief Default destructor.
        ~StackMemoryResource() = default;

        /// \brief No assignment operator.
        StackMemoryResource& operator=(VirtualMemoryResource rhs) = delete;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        /// \brief Stack storage.
        std::aligned_storage_t<kSize, kAlignment> storage_;

        /// \brief Whether the memory resource is free and can be used for allocation.
        bool is_free_{ true };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StackMemoryResource<kSize, kAlignment>.

    template <std::size_t kSize, std::size_t kAlignment>
    inline MemoryRange StackMemoryResource<kSize, kAlignment>::Allocate(Bytes size) noexcept
    {
        if (is_free_ && size <= Bytes(kSize))
        {
            is_free_ = false;

            return { &storage_, size };
        }

        return {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline MemoryRange StackMemoryResource<kSize, kAlignment>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (alignment <= Alignment(kAlignment))
        {
            return Allocate(size);
        }

        return {};
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline void StackMemoryResource<kSize, kAlignment>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(Owns(block));

        is_free_ = true;
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline void StackMemoryResource<kSize, kAlignment>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(alignment <= Alignment(kAlignment));

        Deallocate(block);
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline bool StackMemoryResource<kSize, kAlignment>::Owns(const MemoryRange& block) const noexcept
    {
        return ConstMemoryRange{ &storage_, Bytes{kSize} }.Contains(block);
    }

    template <std::size_t kSize, std::size_t kAlignment>
    inline Bytes StackMemoryResource<kSize, kAlignment>::GetMaxAllocationSize() const noexcept
    {
        return Bytes(kSize);
    }

}

