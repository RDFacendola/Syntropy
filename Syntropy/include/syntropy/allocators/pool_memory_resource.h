
/// \file pool_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains fixed-size memory resources.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <algorithm>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* POOL MEMORY RESOURCE                                                 */
    /************************************************************************/

    /// \brief Memory resource that allocates fixed-size blocks. Deallocated blocks are kept around and recycled when possible.
    /// \tparam TMemoryResource Type of the underlying memory resource.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource>
    class PoolMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param max_size Maximum size for each allocation.
        /// \param max_alignment Maximum alignment for each allocation.
        /// \param Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        PoolMemoryResource(Bytes max_size, Alignment max_alignment, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        PoolMemoryResource(const PoolMemoryResource&) = delete;

        /// \brief Move constructor.
        PoolMemoryResource(PoolMemoryResource&& rhs) noexcept;

        /// \brief Default destructor.
        ~PoolMemoryResource() = default;

        /// \brief Unified assignment operator.
        PoolMemoryResource& operator=(PoolMemoryResource rhs) noexcept;

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

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Swap this memory resource with the provided instance.
        void Swap(PoolMemoryResource& rhs) noexcept;

    private:

        /// \brief Represents a free block: the memory block itself is used to store a pointer to the next free block in the list.
        struct FreeBlock
        {
            ///< \brief Next free block in the pool.
            FreeBlock* next_{ nullptr };
        };

        ///< \brief Underlying memory resource used to manage the pooled memory. Deallocated blocks are sent to the free list and never deallocated by this memory resource.
        TMemoryResource memory_resource_;

        ///< \brief Maximum size for each allocated block.
        Bytes max_size_;

        ///< \brief Maximum alignment for each allocated block.
        Alignment max_alignment_;

        ///< \brief Next free block in the pool.
        FreeBlock* free_{ nullptr };

    };

    /// \brief Swaps two syntropy::PoolMemoryResource<> instances.
    template <typename TMemoryResource>
    void swap(syntropy::PoolMemoryResource<TMemoryResource>& lhs, syntropy::PoolMemoryResource<TMemoryResource>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PoolMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline PoolMemoryResource<TMemoryResource>::PoolMemoryResource(Bytes max_size, Alignment max_alignment, TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
        , max_size_(max_size)
        , max_alignment_(max_alignment)
    {

    }

    template <typename TMemoryResource>
    inline PoolMemoryResource<TMemoryResource>::PoolMemoryResource(PoolMemoryResource&& rhs) noexcept
        : memory_resource_(std::move(rhs.memory_resource_))
        , max_size_(std::move(rhs.max_size_))
        , max_alignment_(std::move(rhs.max_alignment_))
        , free_(rhs.free_)
    {

    }

    template <typename TMemoryResource>
    inline PoolMemoryResource<TMemoryResource>& PoolMemoryResource<TMemoryResource>::operator=(PoolMemoryResource rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TMemoryResource>
    inline MemoryRange PoolMemoryResource<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        if (size <= max_size_)
        {
            if (free_)                                                                      // Attempts to recycle a previously deallocated block.
            {
                auto block = MemoryAddress(free_);

                free_ = free_->next_;

                return { block, block + size };
            }
            else if(auto block = memory_resource_.Allocate(max_size_, max_alignment_))      // Allocate from the underlying memory resource.
            {
                return { block.Begin(), block.Begin() + size };
            }
        }

        return {};
    }

    template <typename TMemoryResource>
    inline MemoryRange PoolMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (alignment <= max_alignment_)
        {
            return Allocate(size);
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(memory_resource_.Owns(block));

        auto free = free_;

        free_ = block.Begin().As<FreeBlock>();      // Send the block to the free list, making it eligible for recycling.

        new (free_) FreeBlock();

        free_->next_ = free;
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(alignment <= max_alignment_);

        Deallocate(block);
    }

    template <typename TMemoryResource>
    inline bool PoolMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline Bytes PoolMemoryResource<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return max_size_;
    }

    template <typename TMemoryResource>
    inline void PoolMemoryResource<TMemoryResource>::Swap(PoolMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(memory_resource_, rhs.memory_resource_);
        swap(max_size_, rhs.max_size_);
        swap(max_alignment_, rhs.max_alignment_);
        swap(free_, rhs.free_);
    }

    template <typename TMemoryResource>
    void swap(syntropy::PoolMemoryResource<TMemoryResource>& lhs, syntropy::PoolMemoryResource<TMemoryResource>& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}
