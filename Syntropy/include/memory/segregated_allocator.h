
/// \file segregated_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators based on segregated free list algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"
#include "memory/block_allocator.h"

#include <array>
#include <vector>

namespace syntropy
{

    /// \brief High-performance allocator that uses segregated best-fit policy for small allocations up to 1KB size.
    /// The allocator is designed to minimize external fragmentation while keeping constant response time.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls. See StaticBlockAllocator.
    /// The segregated free list classes are distributed linearly. Each class is 8 bytes wide.
    /// The total amount of classes is the order of the allocator.
    ///
    /// Example for a 6th-order allocator
    /// Class 0 [1; 8]
    /// Class 1 [9; 16]
    /// Class 2 [17, 24]
    /// Class 3 [25, 32]
    /// Class 4 [33, 40]
    /// Class 5 [41, 48]
    /// 
    /// Based on "Building a low-fragmentation memory system for 64-bit games" by Aaron MacDougall - GDC16
    ///
    /// \author Raffaele D. Facendola - December 2016
    class TinySegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Allocation granularity, in bytes. Each allocated block is a multiple of this value.
        static const size_t kAllocationGranularity = 8;

        /// \brief Maximum allocation size, in bytes.
        static const size_t kMaximumAllocationSize = 1024;

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum amount of memory allocated by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to the next system memory page size.
        TinySegregatedFitAllocator(const HashedString& name, size_t capacity, size_t page_size);

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to the next system memory page size.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        TinySegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t page_size);


        /// \brief No copy constructor.
        TinySegregatedFitAllocator(const TinySegregatedFitAllocator&) = delete;

        /// \brief No assignment operator.
        TinySegregatedFitAllocator& operator=(const TinySegregatedFitAllocator&) = delete;

        /// \brief Default destructor.
        ~TinySegregatedFitAllocator() = default;

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Belongs(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        size_t GetCommitSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Number of segregated free lists.
        static const size_t kOrder = kMaximumAllocationSize / kAllocationGranularity;

        /// \brief Utility structure for free memory blocks within a page.
        struct Block
        {
            Block* next_;                           ///< \brief Address of the next free memory block within this page.
        };

        /// \brief Memory page header.
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

        StaticBlockAllocator allocator_;            ///< \brief Underlying block allocator for page allocations.

        std::array<Page*, kOrder> free_pages_;      ///< \brief Segregated lists of partially allocated pages. The n-th list handles memory blocks up to (1+n) * minimum_allocation_size bytes.

    };

    /// \brief High-performances, low-fragmentation allocator to handle allocation of medium-sized objects.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls.
    ///
    /// Based on: http://www.gii.upv.es/tlsf/files/jrts2008.pdf
    ///
    /// \author Raffaele D. Facendola - January 2017
    class TwoLevelSegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum capacity of the allocator, in bytes.
        /// \param second_level_index Number of classes for each first level index. The actual number of classes is 2^second_level_index.
        TwoLevelSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t second_level_index);

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param second_level_index Number of classes for each first level index. The actual number of classes is 2^second_level_index.
        TwoLevelSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t second_level_index);

        /// \brief Allocate a new memory block.
        /// \param size Size of the block to allocate, in bytes.
        /// \return Return a pointer to the allocated block.
        virtual void* Allocate(size_t size) override;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the block to allocate, in bytes.
        /// \param alignment Alignment of the block.
        /// \return Return a pointer to the allocated block.
        virtual void* Allocate(size_t size, size_t alignment) override;

        /// \brief Free an allocated memory block.
        /// \param block Block to free.
        virtual void Free(void* block) override;

        /// \brief Check whether a given memory block belongs to this allocator.
        /// \param block Block to check.
        ///\ return Returns true if block is allocated or can be allocated by the allocator, returns false otherwise.
        virtual bool Belongs(void* block) const override;

        /// \brief Get the largest amount of memory that can be allocated in a single allocation.
        /// \return Returns the size of the largest memory block that can be theoretically allocated on this allocator.
        virtual size_t GetMaxAllocationSize() const override;

    private:

        /// \brief Minimum size for each memory block.
        static const size_t kMinimumBlockSize = 32;

        /// \brief Header for an allocated block (either free or busy).
        struct BlockHeader
        {
            /// \brief Flag declaring that the block is being used (allocated).
            static const size_t kBusyBlockFlag = 0x2;

            /// \brief Flag declaring that the block has the highest address among all the allocated or free blocks.
            static const size_t kLastBlockFlag = 0x1;

            /// \brief Mask used
            static const size_t kSizeMask = kBusyBlockFlag | kLastBlockFlag;

            BlockHeader* previous_;             ///< \brief Pointer to the previous physical block.

            /// \brief Get the size of the block, in bytes. This size accounts for the size of the header, the payload and any padding.
            size_t GetSize() const;

            /// \brief Set the size of the block.
            /// \param size New size of the block. This size should account for the size of the header, the payload and any padding.
            void SetSize(size_t size);

            /// \brief Check whether this block is being used.
            /// \return Returns true if this block refers to an allocated block, returns false otherwise.
            bool IsBusy() const;

            /// \brief Mark this block as being in use or free.
            /// \param is_busy Whether the block should be considered busy after this call or not.
            void SetBusy(bool is_busy);

            /// \brief Check whether this block has the highest address among every other block inside the allocator.
            /// \return Returns true if this block is the last, returns false otherwise.
            bool IsLast() const;

            /// \brief mark this block as being the last or not.
            /// \param is_last Whether the block should be considered the last after this call or not.
            void SetLast(bool is_last);

            /// \brief Get a pointer to the payload.
            /// \return Returns a pointer to the first address of the payload.
            void* begin();

            /// \brief Get a pointer past the end of the payload.
            /// \return Returns a pointer to the first address after the payload.
            void* end();

        private:

            size_t size_;           ///< \brief Size of the block. The last two bits (Busy and Last) are used to store the block status.

        };

        /// \brief Extended header for a free block.
        struct FreeBlockHeader : BlockHeader
        {
            FreeBlockHeader* next_free_;        ///< \brief Next free block in the segregated list.

            FreeBlockHeader* previous_free_;    ///< \brief Previous free block in the segregated list.

            /// \brief Get a pointer to the payload.
            /// \return Returns a pointer to the first address of the payload.
            void* begin();

            /// \brief Get a pointer past the end of the payload.
            /// \return Returns a pointer to the first address after the payload.
            void* end();
        };

        /// \brief Get a pointer to the smallest free block that can fit an allocation of a given size.
        /// \param block_size Size of the block to fit.
        /// \return Returns a pointer to the smallest free block that can fit an allocation of size size.
        BlockHeader* GetFreeBlockBySize(size_t size);

        /// \brief Get the first free block in a particular segregated free list.
        /// \param index Index of the segregated free list where the block will be taken from.
        /// \return Returns a pointer to the first block in the specified segregated free list, if any. Returns nullptr if no block could be found.
        BlockHeader* PopBlock(size_t index);

        /// \brief Add a free block to the proper segregated free list.
        /// This method causes free blocks coalescing: if the provided block is adjacent to other free blocks, those blocks are merged and inserted inside the proper list.
        /// \param block Block to add. After this call the block is no longer considered 'busy'.
        void PushBlock(BlockHeader* block);

        /// \brief Split a block in two more blocks a stores the second inside the proper segregated free list. The second block is considered not busy.
        /// \param block Block to split.
        /// \param size Size of block after the split, in bytes. The remaining block after the split (if any) is stored as an additional free block.
        void SplitBlock(BlockHeader* block, size_t size);

        /// \param Remove a block from its current segregated free list.
        /// \param block Block to remove.
        void RemoveBlock(FreeBlockHeader* block);

        /// \brief Insert a block inside the proper segregated free list.
        /// \param block Block to add. After this call the block is no longer considered 'busy'.
        void InsertBlock(FreeBlockHeader* block);

        size_t GetFreeListIndexBySize(size_t size) const;

        LinearAllocator pool_;                              ///< \brief Memory pool used by this allocator.

        BlockHeader* last_block_;                           ///< \brief Pointer to the block currently on the head of the pool.

        size_t second_level_index_;                         ///< \brief Number of classes for each first-level class.
                                                            ///         First-level array divide free blocks in classes that are a power of two apart.
                                                            ///         Second-level array subdivide each class linearly.

        std::vector<FreeBlockHeader*> free_lists_;          ///< \brief Pointer to the free lists. Each list handles allocations for a particular class of sizes.
                                                            ///< \brief Segregated free lists are flattened.

    };

    /// \brief Low-fragmentation, low-waste allocator to handle allocation of large objects.
    /// The allocator is designed to minimize external fragmentation while keeping a low amount of wasted space. 
    /// Pages are allocated and deallocated on demand. Memory can be reserved at once and committed at a later time.
    /// The segregated free list classes are distributed exponentially: each class may handle allocation up to double the size of the previous class.
    /// The amount of classes handled by the allocator is said "order" of the allocator.
    ///
    /// Example for a 4th-order allocator with base_allocation_size of 4096 bytes.
    /// Class 0 [1; 4096]
    /// Class 1 [4097; 8192]
    /// Class 2 [8193, 16384]
    /// Class 3 [16385, 32768]
    ///
    /// 
    /// Based on "Building a low-fragmentation memory system for 64-bit games" by Aaron MacDougall - GDC16
    ///
    /// \author Raffaele D. Facendola - January 2017
    class ExponentialSegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Total capacity of the allocator. The capacity is split evenly among each allocation class.
        /// \param base_allocation_size Largest block the 1st-class allocator can handle. Rounded up to the next page size.
        /// \param order Number of classes in the allocator.
        ExponentialSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t base_allocation_size, size_t order);

        /// \brief No copy constructor.
        ExponentialSegregatedFitAllocator(const ExponentialSegregatedFitAllocator&) = delete;

        /// \brief No assignment operator.
        ExponentialSegregatedFitAllocator& operator=(const ExponentialSegregatedFitAllocator&) = delete;

        /// \brief Default destructor.
        ~ExponentialSegregatedFitAllocator() = default;

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Belongs(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

        /// \brief Reserve a new memory block.
        /// \param size Size of the memory block to reserve, in bytes.
        /// \return Returns a pointer to the reserved memory block.
        void* Reserve(size_t size);

        /// \brief Reserve a new aligned memory block.
        /// \param size Size of the memory block to reserve, in bytes.
        /// \param alignment Alignment of the reserved block. Must be a multiple of the minimum allocation size.
        /// \return Returns a pointer to the reserved memory block.
        void* Reserve(size_t size, size_t alignment);

        /// \brief Get the current allocation size, in bytes.
        /// \return Returns the total amount of allocations performed so far by this allocator, in bytes.
        size_t GetAllocationSize() const;

        /// \brief Get the amount of system memory committed by the allocator, in bytes.
        /// Note that the stack allocator allocates all the memory it needs upfront.
        /// \return Returns the amount of system memory committed by the allocator, in bytes.
        size_t GetCommitSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Maximum order for this allocator.
        static const size_t kMaxOrder = 16;

        /// \brief Get a reference to an allocator by block size.
        /// \param block_size Size of the block to allocate or reserve.
        /// \return Returns a reference to the smallest allocator that can handle the given allocation size.
        BlockAllocator& GetAllocatorBySize(size_t block_size);

        size_t base_allocation_size_;                           ///< \brief Allocation size for the first class.

        MemoryPool memory_pool_;                                ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;                              ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        size_t order_;                                          ///< \brief Number of classes in this allocator.

        std::vector<BlockAllocator> allocators_;                ///< \brief Segregated lists of partially allocated pages. The n-th list handles memory blocks up to (1+n) * minimum_allocation_size bytes.
    };

}
