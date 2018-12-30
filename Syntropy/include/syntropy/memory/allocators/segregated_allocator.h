
/// \file segregated_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators based on segregated free list algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <array>
#include <vector>
#include <mutex>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/virtual_memory_range.h"
#include "syntropy/memory/allocators/linear_allocator.h"
#include "syntropy/memory/allocators/allocator.h"

namespace syntropy
{
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
