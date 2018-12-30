
/// \file page_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators backed by virtual memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <algorithm>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_buffer.h"
#include "syntropy/memory/virtual_memory_page.h"

#include "syntropy/memory/allocators/page_allocator_policy.h"

#include "syntropy/memory/allocators/linear_allocator.h"
#include "syntropy/memory/allocators/pool_allocator.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* PAGE ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Allocator used to allocate memory blocks using system virtual memory.
    /// Allocation sizes are rounded up and aligned to page boundaries.
    /// Memory pages are committed and decommitted automatically.
    /// \tparam TPolicy Policy to be used to free and recycle previous memory pages.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TPolicy = FastPageAllocatorPolicy>
    class PageAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param capacity Maximum amount of memory that can be allocated by this allocator.
        /// \param page_size Size of each memory page.
        PageAllocator(Bytes capacity, Bytes page_size) noexcept;

        /// \brief No copy constructor.
        PageAllocator(const PageAllocator&) = delete;

        /// \brief Move constructor.
        PageAllocator(PageAllocator&& rhs) noexcept;

        /// \brief Default destructor.
        ~PageAllocator() = default;

        /// \brief Unified assignment operator.
        PageAllocator& operator=(PageAllocator rhs) noexcept;

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

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(PageAllocator& rhs) noexcept;

    private:

        VirtualMemoryBuffer memory_buffer_;                                                         ///< \brief Virtual memory buffer reserved by this allocator.

        PoolAllocator<LinearAllocator, typename TPolicy::TPoolAllocatorPolicy> allocator_;          ///< \brief Underlying pool allocator used to handle memory pages.

        TPolicy policy_;                                                                            ///< \brief Policy functor used to commit\decommit memory pages.
    };

}

/// \brief Swaps two syntropy::PageAllocator<> instances.
template <typename TPolicy>
void swap(syntropy::PageAllocator<TPolicy>& lhs, syntropy::PageAllocator<TPolicy>& rhs) noexcept;

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TPolicy>
    inline PageAllocator<TPolicy>::PageAllocator(Bytes capacity, Bytes page_size) noexcept
        : memory_buffer_(capacity)
        , allocator_(Math::Ceil(page_size, VirtualMemory::GetPageSize()), VirtualMemory::GetPageAlignment(), memory_buffer_)
    {

    }

    template <typename TPolicy>
    inline PageAllocator<TPolicy>::PageAllocator(PageAllocator&& rhs) noexcept
        : memory_buffer_(std::move(rhs.memory_buffer_))
        , allocator_(std::move(rhs.allocator_))
        , policy_(std::move(rhs.policy_))
    {

    }

    template <typename TPolicy>
    inline PageAllocator<TPolicy>& PageAllocator<TPolicy>::operator=(PageAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TPolicy>
    inline MemoryRange PageAllocator<TPolicy>::Allocate(Bytes size) noexcept
    {
        if (auto block = allocator_.Allocate(size))
        {
            policy_.Commit(block, GetMaxAllocationSize());

            return block;
        }

        return {};
    }

    template <typename TPolicy>
    inline MemoryRange PageAllocator<TPolicy>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            policy_.Commit(block, GetMaxAllocationSize());

            return block;
        }

        return {};
    }

    template <typename TPolicy>
    inline void PageAllocator<TPolicy>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(allocator_.Owns(block));

        policy_.Decommit(block, GetMaxAllocationSize());

        allocator_.Deallocate(block);
    }

    template <typename TPolicy>
    inline void PageAllocator<TPolicy>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(alignment <= VirtualMemory::GetPageAlignment());

        Deallocate(block);
    }

    template <typename TPolicy>
    inline bool PageAllocator<TPolicy>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TPolicy>
    inline Bytes PageAllocator<TPolicy>::GetMaxAllocationSize() const noexcept
    {
        return allocator_.GetMaxAllocationSize();
    }

    template <typename TPolicy>
    inline void PageAllocator<TPolicy>::Swap(PageAllocator& rhs) noexcept
    {
        using std::swap;

        swap(memory_buffer_, rhs.memory_buffer_);
        swap(allocator_, rhs.allocator_);
        swap(policy_, rhs.policy_);
    }

}

template <typename TPolicy>
void swap(syntropy::PageAllocator<TPolicy>& lhs, syntropy::PageAllocator<TPolicy>& rhs) noexcept
{
    lhs.Swap(rhs);
}
