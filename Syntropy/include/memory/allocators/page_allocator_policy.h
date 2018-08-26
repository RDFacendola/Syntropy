
/// \file page_allocator_policy.h
/// \brief This header is part of the syntropy memory management system. It contains different page allocator policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

#include "memory/virtual_memory.h"
#include "memory/virtual_memory_range.h"

#include "memory/allocators/pool_allocator_policy.h"

namespace syntropy
{
    /************************************************************************/
    /* FAST PAGE ALLOCATOR POLICY                                           */
    /************************************************************************/

    /// \brief Represents a syntropy::PageAllocator policy that is used to minimize the number of kernel calls at the cost of a higher memory consumption.
    /// This policy commits memory pages once: when a page is freed it remains committed, consuming some of the system memory but granting an immediate response when reallocating again.
    /// Due to the commit-once policy, pages are always committed entirely, even when the requested block is smaller (this is not a problem since most of the OSs will not commit the memory
    /// until the first access).
    /// This policy is best suited for smaller allocation sizes.
    /// \author Raffaele D. Facendola - August 2018
    struct FastPageAllocatorPolicy
    {
        /// \brief Policy of the underlying pool allocator: intrusive free-list can be used since memory pages are kept committed at any given time.
        using TPoolAllocatorPolicy = DefaultPoolAllocatorPolicy;

        /// \brief Commit a memory block.
        /// \param block Block to commit.
        /// \param page_size Size of the memory page.
        void Commit(const MemoryRange& block, Bytes page_size);

        /// \brief Decommit a memory block.
        /// \param block Block to decommit.
        /// \param page_size Size of the memory page.
        void Decommit(const MemoryRange& block, Bytes page_size);

    private:

        MemoryAddress head_;            ///< \brief Highest address that was ever committed. Used to quickly determine whether a block was recycled.
    };

    /************************************************************************/
    /* COMPACT PAGE ALLOCATOR POLICY                                        */
    /************************************************************************/

    /// \brief Represetns a syntropy::PageAllocator policy that is used to minimize the amount of memory used by an allocator at cost of a higher response time of the allocator.
    /// This policy causes memory pages to be decommitted upon deallocation automatically. Any allocation thus requires a kernel call to make the page available again.
    /// Due to the on-demand commit policy, the number of virtual memory pages is always the minimum amount that can satisfy the block size.
    /// This policy is best suited for larger allocation sizes.
    /// \author Raffaele D. Facendola - August 2018
    struct CompactPageAllocatorPolicy
    {
        /// \brief Policy of the underlying pool allocator: since memory pages are decommitted the allocator cannot intrusively store information inside the free blocks.
        using TPoolAllocatorPolicy = NonIntrusivePoolAllocatorPolicy;

        /// \brief Commit a memory block.
        /// \param block Block to commit.
        /// \param page_size Size of the memory page.
        void Commit(const MemoryRange& block, Bytes page_size);

        /// \brief Decommit a memory block.
        /// \param block Block to decommit.
        /// \param page_size Size of the memory page.
        void Decommit(const MemoryRange& block, Bytes page_size);
    };

}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline void FastPageAllocatorPolicy::Commit(const MemoryRange& block, Bytes page_size)
    {
        // Blocks are only committed the first time, subsequent calls to this method expect the block to be already committed.
        // The underlying linear allocator guarantees that new blocks have an increasingly higher memory address.

        if (block.Begin() >= head_)
        {
            auto page_alignment = VirtualMemory::GetPageAlignment();

            VirtualMemoryRange(block.Begin(), block.Begin() + page_size).Commit();                          // Kernel call.

            head_ = block.Begin() + page_size;
        }
    }

    inline void FastPageAllocatorPolicy::Decommit(const MemoryRange& /*block*/, Bytes /*page_size*/)
    {
        // The block is committed: leave it unchanged to avoid any subsequent kernel call when recycling it.
    }

    inline void CompactPageAllocatorPolicy::Commit(const MemoryRange& block, Bytes /*page_size*/)
    {
        // Blocks are always decommitted when freed: re-commit is always needed. Commit the minimum number of pages around the block.

        auto page_alignment = VirtualMemory::GetPageAlignment();

        VirtualMemoryRange(block.Begin(), block.End().GetAligned(page_alignment)).Commit();                 // Kernel call.
    }

    inline void CompactPageAllocatorPolicy::Decommit(const MemoryRange& block, Bytes /*page_size*/)
    {
        // Keep the allocations to a minimum: deallocate the minimum number of pages around the block.

        auto page_alignment = VirtualMemory::GetPageAlignment();

        VirtualMemoryRange(block.Begin(), block.End().GetAligned(page_alignment)).Decommit();               // Kernel call.
    }

}

