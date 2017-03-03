
/// \file segregated_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators based on segregated free list algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"
#include "memory/block_allocator.h"

namespace syntropy
{

    /// \brief High-performance allocator that uses segregated best-fit policy for small allocations up to a certain size.
    /// The allocator is designed to minimize external fragmentation while keeping constant response time.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls. See MonotonicBlockAllocator.
    /// The segregated free list classes are distributed linearly in the range [1, maximum_allocation_size]. Each class is 8 bytes large.
    /// The amount of classes handled by the allocator is said "order" of the allocator.
    ///
    /// Example for a 6th-order allocator
    /// Class 0 [1; 8]
    /// Class 1 [9; 16]
    /// Class 2 [17, 24]
    /// Class 3 [25, 32]
    /// Class 4 [33, 40]
    /// Class 5 [41, 48]
    /// 
    /// \author Raffaele D. Facendola - December 2016
    class TinySegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Minimum allocation size in bytes.
        static const size_t kMinimumAllocationSize = 8;

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum amount of memory allocated by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to the next system memory page size.
        /// \param order Number of classes in the allocator.
        TinySegregatedFitAllocator(const HashedString& name, size_t capacity, size_t page_size, size_t order = 32);

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

    /// \brief High-performances, low-fragmentation allocator to handle allocation of medium-sized objects.
    /// The allocator is designed to minimize external fragmentation while keeping constant response time. 
    /// Pages are allocated and deallocated on demand. Memory can be reserved at once and committed at a later time.
    /// The segregated free list classes are distributed linearly: each class may handle allocation up to a multiple of the base allocation size.
    /// The amount of classes handled by the allocator is said "order" of the allocator.
    ///
    /// Example for a 4th-order allocator with base_allocation_size of 1024 bytes.
    /// Class 0 [1; 1024]
    /// Class 1 [1025; 2048]
    /// Class 2 [2049, 3072]
    /// Class 3 [3073, 4096]
    ///
    /// \author Raffaele D. Facendola - January 2017
    class TwoLevelSegregatedFitAllocator : public Allocator
    {
    public:

        TwoLevelSegregatedFitAllocator(const HashedString& name, size_t capacity, size_t second_level_index);

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Belongs(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

    private:

        /// \brief Header for an allocated block.
        struct BlockHeader
        {
            static const size_t kBusyBlockFlag = 0x2;

            static const size_t kLastBlockFlag = 0x1;

            static const size_t kSizeMask = kBusyBlockFlag | kLastBlockFlag;

            size_t size_;                       ///< \brief Size of the block. 
                                                /// The last two bit (F and T) are used to store the block status. F tells whether the block is busy or free, T tells whether the block is the last one in the pool.

            BlockHeader* previous_;             ///< \brief Pointer to the previous physical block.

            void* operator*();

            size_t GetSize() const;

            void SetSize(size_t size);

            bool IsBusy() const;

            void SetBusy(bool is_busy);

            bool IsLast() const;

            void SetLast(bool is_last);
        };

        /// \brief Extended header for a free block.
        struct FreeBlockHeader : BlockHeader
        {
            FreeBlockHeader* next_free_;        ///< \brief Next free block in the segregated list.

            FreeBlockHeader* previous_free_;    ///< \brief Previous free block in the segregated list.
        };

        /// \brief Get a pointer to a free block which can contain an allocation of a given size.
        /// \param block_size Size of the block.
        /// \return Returns a pointer to the smallest free block which can handle an allocation for the given size.
        BlockHeader* GetFreeBlockBySize(size_t size);

        BlockHeader* PopBlock(size_t index);

        void PushBlock(BlockHeader* block);

        void SplitBlock(BlockHeader* block, size_t size);

        // Remove a block from its current free list.
        void RemoveBlock(FreeBlockHeader* block);

        void InsertBlock(FreeBlockHeader* block);

        size_t GetFreeListIndexBySize(size_t size) const;

        SequentialAllocator pool_;                          ///< \brief Memory pool used by this allocator.

        BlockHeader* last_block_;                           ///< \brief Pointer to the block currently on the head of the pool.

        size_t second_level_index_;                         ///< \brief Number of classes for each first-level class.
                                                            ///         First-level array divide free blocks in classes that are a power of two apart.
                                                            ///         Second-level array subdivide each class linearly.

        VectorAllocator<FreeBlockHeader*> free_lists_;      ///< \brief Pointer to the free lists. Each list handles allocations for a particular class of sizes.
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

        /// \brief Get an upper bound for the memory being used by the allocator.
        /// \return Returns an upper bound for the effective memory footprint of the allocator on the system memory, in bytes.
        size_t GetUpperBoundSize() const;

        /// \brief Get the maximum amount of memory that can be allocated by this allocator, in bytes.
        /// \return Returns the maximum amount of memory that can be allocated by this allocator, in bytes.
        size_t GetCapacity() const;

    private:

        /// \brief Get a reference to an allocator by block size.
        /// \param block_size Size of the block to allocate or reserve.
        /// \return Returns a reference to the smallest allocator that can handle the given allocation size.
        BlockAllocator& GetAllocatorBySize(size_t block_size);

        VectorAllocator<BlockAllocator> allocators_;    ///< \brief Actual allocators. One for each class.

        size_t base_allocation_size_;                   ///< \brief Allocation size for the first class.

    };

}
