
/// \file segregated_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators based on segregated free list algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <array>
#include <vector>
#include <mutex>

#include "memory/bytes.h"
#include "memory/memory_address.h"
#include "memory/virtual_memory_range.h"
#include "memory/allocators/linear_allocator.h"
#include "memory/allocators/allocator.h"

namespace syntropy
{
    struct BlockAllocator {};
    struct StaticBlockAllocator {};

    /// \brief High-performance, low-fragmentation allocator best suited to handle allocation of small objects.
    /// The allocator is designed to minimize external fragmentation while keeping constant response time.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls.
    /// The segregated free list classes are distributed linearly.
    ///
    /// Example for a 6th-order allocator whose allocation classes are 8 bytes wide:
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
    class LinearSegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Minimum allocation size, in bytes.
        static const Bytes kMinimumAllocationSize;
        
        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum amount of memory allocated by the allocator.
        /// \param class_size Size of each allocation class, in bytes.
        /// \param order Number of allocation classes handled by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to the next system memory page size.
        LinearSegregatedFitAllocator(const HashedString& name, Bytes capacity, Bytes class_size, size_t order, Bytes page_size);

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param memory_range Memory range used by the allocator. Determines the capacity of the allocator.
        /// \param class_size Size of each allocation class, in bytes.
        /// \param order Number of allocation classes handled by the allocator.
        /// \param page_size Size of each memory page, in bytes. This value is rounded up to the next system memory page size.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        LinearSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, Bytes class_size, size_t order, Bytes page_size);

        /// \brief No copy constructor.
        LinearSegregatedFitAllocator(const LinearSegregatedFitAllocator&) = delete;

        /// \brief Move constructor.
        LinearSegregatedFitAllocator(LinearSegregatedFitAllocator&& other);

        /// \brief No assignment operator.
        LinearSegregatedFitAllocator& operator=(const LinearSegregatedFitAllocator&) = delete;

        /// \brief Default destructor.
        ~LinearSegregatedFitAllocator() = default;

        virtual void* Allocate(Bytes size) override;

        virtual void* Allocate(Bytes size, Alignment alignment) override;

        virtual void Free(void* block) override;

        virtual bool Owns(void* block) const override;

        virtual Bytes GetMaxAllocationSize() const override;

        /// \brief Get the order of this allocator.
        /// \return Returns the number of allocation classes handled by the allocator.
        size_t GetOrder() const;

        /// \brief Get the size of each page.
        /// \return Returns the size of each page, in bytes.
        Bytes GetPageSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Utility structure for free memory blocks within a page.
        struct Block
        {
            Block* next_;                           ///< \brief Address of the next free memory block within this page.
        };

        /// \brief Memory page header.
        struct Page
        {

            Page(Bytes block_size, Bytes page_size);

            Page* next_;                            ///< \brief Address of the next page in the allocator.

            Page* previous_;                        ///< \brief Address of the previous page in the allocator.

            Bytes block_size_;                      ///< \brief Size of each memory block in this page in bytes.

            size_t allocated_blocks_;               ///< \brief Amount of allocated blocks in this page.

            Block* free_;                           ///< \brief Address of the first free block in this page. If the page is full this pointer is nullptr.

            /// \brief Allocate a new block.
            /// \return Returns the address of a free block.
            void* AllocateBlock();

            /// \brief Free a block.
            /// \param block Address of the free block.
            void FreeBlock(void* block);

            /// \brief Check whether the page is full and cannot fill any other allocations.
            /// \return Returns true if the page is full, returns false otherwise.
            bool IsFull() const;

            /// \brief Check whether the page has no currently allocated block.
            /// \return Returns true if the page is empty, returns false otherwise.
            bool IsEmpty() const;

            /// \brief Get a pointer to the first block in the page.
            /// \return Returns a pointer to the first block in the page.
            Block* GetFirstBlock();

            /// \brief Get a pointer to the last block in the page.
            /// \return Returns a pointer to the last block in the page.
            Block* GetLastBlock(Bytes page_size);

        };

        /// \brief Get the index of the smallest free list that can contain an allocation of a given size.
        /// \param size Size of the allocation.
        size_t GetListIndexBySize(Bytes size) const;

        /// \brief Allocate a new page.
        /// \param block_size Size of the blocks in the page, in bytes.
        /// \return Returns the address to the allocated page.
        Page* AllocatePage(Bytes block_size);

        /// \brief Discard the first page in a free list and move forward.
        /// \param list_index Index of the free list whose head should be discarded.
        void DiscardPage(size_t list_index);

        /// \brief Free a page.
        /// The page can be recycled by other allocators when needed
        /// \param page Page to free.
        void FreePage(Page* page);

        /// \brief Restore a discarded page.
        /// The page is added to its allocator list and can accept new allocations.
        /// \param page Page to restore.
        void RestorePage(Page* page);

        /// \brief Check whether the preconditions for this allocator are met.
        void CheckPreconditions() const;

        StaticBlockAllocator allocator_;            ///< \brief Underlying block allocator for page allocations.

        std::vector<Page*> free_lists_;             ///< \brief Segregated lists of partially allocated pages. The n-th list handles memory blocks up to (1+n) * minimum_allocation_size bytes.

        Bytes class_size_;                          ///< \brief Size of each allocation class, in bytes.

        std::mutex mutex_;                          ///< \brief Used for thread-safety purposes.
    };

    /// \brief Low-fragmentation, low-waste allocator best suited to handle allocation of large objects.
    /// The allocator is designed to minimize external fragmentation while keeping a low amount of wasted space. 
    /// Pages are allocated and deallocated on demand. Memory can be reserved and committed at a later time.
    /// The segregated free list classes are distributed exponentially: each class may handle allocation up to double the size of the previous class.
    ///
    /// Example for a 4th-order allocator with class_size equal to 4096 bytes.
    /// Class 0 [1; 4096]
    /// Class 1 [4097; 8192]
    /// Class 2 [8193, 16384]
    /// Class 3 [16385, 32768]
    ///
    /// Based on "Building a low-fragmentation memory system for 64-bit games" by Aaron MacDougall - GDC16
    ///
    /// \author Raffaele D. Facendola - January 2017
    class ExponentialSegregatedFitAllocator : public Allocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum amount of memory allocated by the allocator.
        /// \param class_size Size of the first allocation class, in bytes. Rounded up to memory page size.
        /// \param order Number of allocation classes handled by the allocator.
        ExponentialSegregatedFitAllocator(const HashedString& name, Bytes capacity, Bytes class_size, size_t order);

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param memory_range Memory range used by the allocator. Determines the capacity of the allocator.
        /// \param class_size Size of the first allocation class, in bytes. Rounded up to memory page size.
        /// \param order Number of allocation classes handled by the allocator.
        /// \remarks The allocator doesn't take ownership of the memory range provided as input.
        ExponentialSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, Bytes class_size, size_t order);

        /// \brief No copy constructor.
        ExponentialSegregatedFitAllocator(const ExponentialSegregatedFitAllocator&) = delete;

        /// \brief Move constructor.
        ExponentialSegregatedFitAllocator(ExponentialSegregatedFitAllocator&& other);

        /// \brief No assignment operator.
        ExponentialSegregatedFitAllocator& operator=(const ExponentialSegregatedFitAllocator&) = delete;

        /// \brief Default destructor.
        ~ExponentialSegregatedFitAllocator() = default;

        virtual void* Allocate(Bytes size) override;

        virtual void* Allocate(Bytes size, Alignment alignment) override;

        virtual void Free(void* block) override;

        virtual bool Owns(void* block) const override;

        virtual Bytes GetMaxAllocationSize() const override;

        /// \brief Reserve a memory block.
        /// A reserved block must be committed before it can be accessed.
        /// \param size Size of the memory block to reserve, in bytes.
        /// \return Returns a pointer to the reserved memory block. 
        /// \remarks The block is guaranteed not to share any memory page with any other allocation.
        void* Reserve(Bytes size);

        /// \brief Reserve an aligned memory block.
        /// A reserved block must be committed before it can be accessed.
        /// \param size Size of the memory block to reserve, in bytes.
        /// \param alignment Alignment of the reserved block.
        /// \return Returns a pointer to the reserved memory block.
        /// \remarks The block is guaranteed not to share any memory page with any other allocation.
        void* Reserve(Bytes size, Alignment alignment);

        /// \brief Get the order of this allocator.
        /// \return Returns the number of allocation classes handled by the allocator.
        std::size_t GetOrder() const;

        /// \brief Get the size of the smallest allocation class.
        /// \brief Returns the size of the smallest allocation class, in bytes.
        Bytes GetClassSize() const;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Initialize the allocators for each allocation class.
        /// \param order Number of allocators to initialize.
        /// \param class_size Size of the first allocation class in bytes.
        void InitializeAllocators(std::size_t order, Bytes class_size);

        /// \brief Get a reference to an allocator by block size.
        /// \param block_size Size of the block to allocate or reserve.
        /// \return Returns a reference to the smallest allocator that can handle the given allocation size.
        BlockAllocator& GetAllocatorBySize(Bytes block_size);

        /// \brief Get the capacity of each allocator.
        /// \return Returns the capacity of each allocator, in bytes.
        Bytes GetAllocatorCapacity() const;

        VirtualMemoryRange memory_pool_;                    ///< \brief Virtual memory range owned by this allocator. Empty if the allocator owns no virtual memory.

        MemoryRange memory_range_;                          ///< \brief Memory range managed by the allocator. May refer to memory_pool_ or to a range owned by someone else.

        std::vector<BlockAllocator> allocators_;            ///< \brief Segregated lists of partially allocated pages. The n-th list handles memory blocks up to (1+n) * minimum_allocation_size bytes.

        std::mutex mutex_;                                  ///< \brief Used for thread-safety purposes.
    };

    /// \brief High-performances, low-fragmentation allocator to handle allocation of any size.
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
        TwoLevelSegregatedFitAllocator(const HashedString& name, Bytes capacity, size_t second_level_index);

        /// \brief Create a new allocator.
        /// \param name Name of the allocator.
        /// \param memory_range Memory range used by the allocator.
        /// \param second_level_index Number of classes for each first level index. The actual number of classes is 2^second_level_index.
        TwoLevelSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t second_level_index);

        /// \brief No copy constructor.
        TwoLevelSegregatedFitAllocator(const TwoLevelSegregatedFitAllocator&) = delete;

        /// \brief Move constructor.
        TwoLevelSegregatedFitAllocator(TwoLevelSegregatedFitAllocator&& other);

        /// \brief No assignment operator.
        TwoLevelSegregatedFitAllocator& operator=(const TwoLevelSegregatedFitAllocator&) = delete;

        /// \brief Virtual destructor.
        virtual ~TwoLevelSegregatedFitAllocator() = default;

        virtual void* Allocate(Bytes size) override;

        virtual void* Allocate(Bytes size, Alignment alignment) override;

        virtual void Free(void* block) override;

        virtual bool Owns(void* block) const override;

        virtual Bytes GetMaxAllocationSize() const override;

        /// \brief Get the memory range managed by this allocator.
        /// \return Returns the memory range managed by this allocator.
        const MemoryRange& GetRange() const;

    private:

        /// \brief Minimum size for each memory block.
        static const Bytes kMinimumBlockSize;

        /// \brief Header for an allocated block (either free or busy).
        struct BlockHeader
        {
            /// \brief Flag declaring that the block is being used (allocated).
            static const size_t kBusyBlockFlag = 0x2;

            /// \brief Flag declaring that the block has the highest address among all the allocated or free blocks.
            static const size_t kLastBlockFlag = 0x1;

            /// \brief Used to mask away status flags from the block size.
            static const size_t kSizeMask = kBusyBlockFlag | kLastBlockFlag;

            BlockHeader* previous_;             ///< \brief Pointer to the previous physical block.

            /// \brief Get the size of the block, in bytes. This size accounts for the size of the header, the payload and any padding.
            Bytes GetSize() const;

            /// \brief Set the size of the block.
            /// \param size New size of the block. This size should account for the size of the header, the payload and any padding.
            void SetSize(Bytes size);

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

            Bytes size_;            ///< \brief Size of the block. The last two bits (Busy and Last) are used to store the block status.

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

        /// \brief Initialize the allocator.
        void Initialize(size_t second_level_count);

        /// \brief Get a pointer to the smallest free block that can fit an allocation of a given size.
        /// \param block_size Size of the block to fit.
        /// \return Returns a pointer to the smallest free block that can fit an allocation of size size.
        BlockHeader* GetFreeBlockBySize(Bytes size);

        /// \brief Mark the bit relative to a free list as "set".
        void SetBitmap(size_t first_level_index, size_t second_level_index);

        /// \brief Mark the bit relative to a free list as "not set".
        void ResetBitmap(size_t first_level_index, size_t second_level_index);

        /// \brief Get the first free block in a particular segregated free list.
        /// \param index Index of the segregated free list where the block will be taken from.
        /// \return Returns a pointer to the first block in the specified segregated free list, if any. Returns nullptr if no block could be found.
        BlockHeader* PopBlock(size_t index);

        /// \brief Add a free block to the proper segregated free list.
        /// This method causes free blocks coalescing: if the provided block is adjacent to other free blocks, those blocks are merged and inserted inside the proper list.
        /// \param block Block to add. After this call the block is no longer considered 'busy'.
        void PushBlock(BlockHeader* block);

        /// \brief Allocate a new block from the pool. This method doesn't recycle any existing free blocks.
        /// \param size Size of the block to allocate.
        /// \return Returns a pointer to the allocated block.
        BlockHeader* AllocateBlock(Bytes size);

        /// \brief Split a block in two more blocks a stores the second inside the proper segregated free list. The second block is considered not busy.
        /// \param block Block to split.
        /// \param size Size of block after the split, in bytes. The remaining block after the split (if any) is stored as an additional free block.
        void SplitBlock(BlockHeader* block, Bytes size);

        /// \param Remove a block from its current segregated free list.
        /// \param block Block to remove.
        void RemoveBlock(FreeBlockHeader* block);

        /// \brief Insert a block inside the proper segregated free list.
        /// \param block Block to add. After this call the block is no longer considered 'busy'.
        void InsertBlock(FreeBlockHeader* block);

        /// \brief Get the first and the second level index of a free list for a given allocation size.
        /// \param size Size of the block.
        /// \param first_level_index Index of the first-level class. Output.
        /// \param second_level_index Index of the second-level class. Output.
        /// \param roundup Whether to round up the size to the next class.
        void GetFreeListIndex(Bytes size, size_t& first_level_index, size_t& second_level_index, bool roundup = false) const;

        /// \brief Get the index of a free list inside the flat free list array given its first-level and second-level indexes.
        /// \param first_level_index First level index.
        /// \param second_level_index Second level index.
        /// \return Returns the index of the free list associated with the given first-level and second-level index.
        size_t GetFreeListIndex(size_t first_level_index, size_t second_level_index) const;
        
        LinearAllocator allocator_;                         ///< \brief Underlying allocator used by this one.

        BlockHeader* last_block_;                           ///< \brief Pointer to the block currently on the head of the pool.

        size_t first_level_count_;                          ///< \brief Number of first-level classes.

        size_t second_level_count_;                         ///< \brief Number of classes for each first-level class. Stored as log2(number_of_classes)

        size_t first_level_bitmap_;                         ///< \brief Bitmap used to determine which first-level class contains at least one free block.

        std::vector<size_t> second_level_bitmap_;           ///< \brief Bitmap used to determine which second-level class contains at least one free block. Each element refers to a first-level class.

        std::vector<FreeBlockHeader*> free_lists_;          ///< \brief Pointer to the free lists. Flattened to a mono-dimensional array.

        std::mutex mutex_;                                  ///< \brief Used for thread-safety purposes.
    };

}
