
/// \file segregated_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators based on segregated free list algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"
#include "memory/block_allocator.h"

namespace syntropy
{

    /// \brief High-performance allocator that uses segregated best-fit policy for allocations up to a certain size.
    /// The allocator is designed to minimize external fragmentation while keeping constant response time.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls. See MonotonicBlockAllocator for more.
    /// \author Raffaele D. Facendola - December 2016
    class SegregatedPoolAllocator
    {
    public:

        /// \brief Minimum allocation size in bytes.
        static const size_t kMinimumAllocationSize = 8;

        /// \brief Create a new allocator.
        /// \param capacity Maximum amount of memory allocated by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to system memory page size.
        /// \param maximum_allocation_size Maximum size for a single allocation that can be performed on this allocator.
        SegregatedPoolAllocator(size_t capacity, size_t page_size, size_t maximum_allocation_size = 256);

        /// \brief No copy constructor.
        SegregatedPoolAllocator(const SegregatedPoolAllocator&) = delete;

        /// \brief No assignment operator.
        SegregatedPoolAllocator& operator=(const SegregatedPoolAllocator&) = delete;

        /// \brief Default destructor.
        ~SegregatedPoolAllocator() = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \param alignment Alignment of the allocated block. Must be a multiple of the minimum allocation size.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free a memory block.
        /// \param address Address of the block to free.
        void Free(void* address);

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetSize() const;

        /// \brief Get the current effective memory footprint of the allocator on the system memory, in bytes.
        /// This value is always equal or greater than the allocated size.
        /// \return Returns the current effective memory footprint of the allocator on the system memory, in bytes.
        size_t GetEffectiveSize() const;

        /// \brief Get the maximum amount of memory that can be allocated by this allocator, in bytes.
        /// \return Returns the maximum amount of memory that can be allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        ///< \brief Utility structure for free memory blocks within a page.
        struct Block
        {
            Block* next_;                           ///< \brief Address of the next free memory block within this page.
        };

        ///< \brief Memory page header.
        struct Page
        {
            Page* next_;                            ///< \brief Address of the next page in the allocator.

            Page* previous_;                        ///< \brief Address of the previous page in the allocator.

            Block* free_;                           ///< \brief Address of the first free block in this page. If the page is full this pointer is nullptr.

            size_t block_size_;                     ///< \brief Size of each memory block in this page in bytes.

            size_t allocated_blocks_;               ///< \brief Amount of allocated blocks in this page.
        };

        /// \brief Allocate a new page.
        /// \param block_size Size of the blocks in the page, in bytes.
        /// \return Returns the address to the allocated page.
        Page* AllocatePage(size_t block_size);

        /// \brief Free a page.
        /// The page can be recycled by other allocators when needed
        /// \param page Page to free.
        void FreePage(Page* page);

        /// \brief Remove a page from its allocator list.
        /// \param page Page to discard. 
        void DiscardPage(Page*& page);

        /// \brief Restore a discarded page.
        /// The page is added to its allocator list and can accept new allocations.
        /// \param page Page to restore.
        void RestorePage(Page* page);

        /// \brief Get the total amount of blocks that can be allocated inside a page.
        /// \param page Page to work with.
        /// \param block_size Size of each block, in bytes.
        /// \param head Output. Contains a pointer to the first block after the page header.
        /// \return Returns the amount of blocks of size block_size that can be stored inside the page.
        size_t GetBlockCount(Page* page, size_t block_size, Block*& head) const;

        MonotonicBlockAllocator allocator_;         ///< \brief Underlying block allocator for page allocations.

        size_t maximum_block_size_;                 ///< \brief Maximum block size for this allocator.

        VectorAllocator<Page*> free_pages_;         ///< \brief Segregated lists of partially allocated pages. The n-th list handles memory blocks up to (1+n) * minimum_allocation_size bytes.

    };

    /// \brief Low-fragmentation, low-waste allocator to handle allocation of large objects.
    /// The allocator is designed to minimize external fragmentation while keeping a low amount of wasted space.
    /// The allocator allocates and deallocates pages on demand.
    /// \author Raffaele D. Facendola - January 2017
    class ClusteredPoolAllocator
    {
    public:

        ClusteredPoolAllocator(size_t capacity, size_t minimum_allocation_size, size_t order);

        /// \brief No copy constructor.
        ClusteredPoolAllocator(const ClusteredPoolAllocator&) = delete;

        /// \brief No assignment operator.
        ClusteredPoolAllocator& operator=(const ClusteredPoolAllocator&) = delete;

        /// \brief Default destructor.
        ~ClusteredPoolAllocator() = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size);

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \param alignment Alignment of the allocated block. Must be a multiple of the minimum allocation size.
        /// \return Returns a pointer to the allocated memory block.
        void* Allocate(size_t size, size_t alignment);

        /// \brief Free a memory block.
        /// \param address Address of the block to free.
        void Free(void* address);

        /// \brief Get the effective memory footprint of this allocator.
        /// \return Returns the amount of memory effectively allocated by this allocator, in bytes.
        size_t GetSize() const;

    private:

        BlockAllocator* allocators_;                ///< \brief Array of cluster allocators. One allocator per order.

        size_t order_;                              ///< \brief Order of the allocator. Maximum allocation size is base_allocation_size_ * 2 ^ order_.

        size_t base_allocation_size_;               ///< \brief Allocation size for the first-order allocator.

    };



}