
/// \file tlsf_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources based on two-level segregated-fit allocator.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <tuple>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

#include "syntropy/containers/vector.h"

#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* TLSF MEMORY RESOURCE <TMEMORY RESOURCE>                              */
    /************************************************************************/

    /// \brief Tier 2, high-performances, low-fragmentation memory resource to handle allocation of any size.
    /// The allocator allocates pages on demand but uses a no-deallocation policy to avoid kernel calls.
    ///
    /// Based on: http://www.gii.upv.es/tlsf/files/jrts2008.pdf
    ///
    /// \author Raffaele D. Facendola - January 2017
    template <typename TMemoryResource>
    class TLSFMemoryResource
    {
    public:

        /// \brief Create a new allocator.
        /// \param first_levels Number of first-dimension levels. Each first-level splits free-blocks ranges exponentially.
        /// \param second_level_bits Number of bits needed to encode second-dimension levels. Each second-level splits first-level ranges linearly.
        /// \param arguments Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        TLSFMemoryResource(std::int64_t first_levels, std::int64_t second_level_bits, TArguments&&... arguments);

        /// \brief No copy constructor.
        TLSFMemoryResource(const TLSFMemoryResource&) = delete;

        /// \brief Move constructor.
        TLSFMemoryResource(TLSFMemoryResource&& other);

        /// \brief No assignment operator.
        TLSFMemoryResource& operator=(const TLSFMemoryResource&) = delete;

        /// \brief Destructor.
        ~TLSFMemoryResource();

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        struct FreeBlock
        {

        };

        /// \brief Map a block size to its first-level and second-level indexes.
        std::tuple<std::int64_t, std::int64_t> MappingInsert(Bytes size) const;

        /// \brief Maps a block size to the first-level and second-level to start the search of a free block from. The block size is rounded up to the next list size.
        std::tuple<Bytes, std::int64_t, std::int64_t> MappingSearch(Bytes size) const;

        /// \brief Find a free block starting from a given pair of first-level and second-level index.
        MemoryRange SearchSuitableBlock(std::int64_t first_level_index, std::int64_t second_level_index) const;

        /// \brief Underlying memory resource.
        TMemoryResource memory_resource_;

        /// \brief Number of first-classes ranges. Each range i is [2^i; 2^(i+1)).
        std::int64_t first_levels_{ 16 };

        /// \brief Number of bits needed to represent second-classes ranges. Each range splits a first-level range in equal widths.
        std::int64_t second_level_bits_{ 3 };

        /// \brief Bitmap used to identify non-empty, first-level, free lists.
        std::int64_t first_bitmap_{ 0 };

        /// \brief Bitmap used to identify non-empty, second-level, free lists. One bitmap per each first-level range.
        Vector<std::int64_t> second_bitmaps_;

        /// \brief Linked list of free blocks for each first-level second-level index pair.
        Vector<FreeBlock*> free_lists_;









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

        /// \brief Get the index of a free list inside the flat free list array given its first-level and second-level indexes.
        /// \param first_level_index First level index.
        /// \param second_level_index Second level index.
        /// \return Returns the index of the free list associated with the given first-level and second-level index.
        size_t GetFreeListIndex(size_t first_level_index, size_t second_level_index) const;


        BlockHeader* last_block_;                           ///< \brief Pointer to the block currently on the head of the pool.

        size_t first_level_bitmap_;                         ///< \brief Bitmap used to determine which first-level class contains at least one free block.

        std::vector<size_t> second_level_bitmap_;           ///< \brief Bitmap used to determine which second-level class contains at least one free block. Each element refers to a first-level class.

        std::vector<FreeBlockHeader*> free_lists_;          ///< \brief Pointer to the free lists. Flattened to a mono-dimensional array.

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: BLOCK HEADER                   */
    /************************************************************************/

    Bytes TwoLevelSegregatedFitAllocator::BlockHeader::GetSize() const
    {
        return size_ & Bytes(~kSizeMask);
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetSize(Bytes size)
    {
        SYNTROPY_ASSERT((size & Bytes(kSizeMask)) == 0_Bytes)               // The new size must not interfere with the status bits at the end!.

            size_ = size | (size_ & Bytes(kSizeMask));                          // Preserve the status of the two flags at the end.
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsBusy() const
    {
        return (size_ & Bytes(kBusyBlockFlag)) != 0_Bytes;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetBusy(bool is_busy)
    {
        if (is_busy)
        {
            size_ = size_ | Bytes(kBusyBlockFlag);
        }
        else
        {
            size_ = size_ & Bytes(kBusyBlockFlag);
        }
    }

    bool TwoLevelSegregatedFitAllocator::BlockHeader::IsLast() const
    {
        return (size_ & Bytes(kLastBlockFlag)) != 0_Bytes;
    }

    void TwoLevelSegregatedFitAllocator::BlockHeader::SetLast(bool is_last)
    {
        if (is_last)
        {
            size_ = size_ | Bytes(kLastBlockFlag);
        }
        else
        {
            size_ = size_ & Bytes(~kLastBlockFlag);
        }
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::begin()
    {
        return MemoryAddress(this) + Bytes(sizeof(BlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::BlockHeader::end()
    {
        return MemoryAddress(this) + GetSize();
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR :: FREE BLOCK HEADER              */
    /************************************************************************/

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::begin()
    {
        return MemoryAddress(this) + Bytes(sizeof(FreeBlockHeader));
    }

    void* TwoLevelSegregatedFitAllocator::FreeBlockHeader::end()
    {
        return MemoryAddress(this) + GetSize();
    }

    /************************************************************************/
    /* TWO LEVEL SEGREGATED FIT ALLOCATOR                                   */
    /************************************************************************/

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, Bytes capacity, size_t second_level_index)
        : Allocator(name)
        , allocator_(capacity, Alignment(VirtualMemory::GetPageSize()))
    {
        Initialize(second_level_index);
    }

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(const HashedString& name, const MemoryRange& memory_range, size_t second_level_index)
        : Allocator(name)
        , allocator_(memory_range, Alignment(VirtualMemory::GetPageSize()))
    {
        Initialize(second_level_index);
    }

    TwoLevelSegregatedFitAllocator::TwoLevelSegregatedFitAllocator(TwoLevelSegregatedFitAllocator&& other)
        : Allocator(std::move(other))
        , allocator_(std::move(other.allocator_))
        , last_block_(std::move(other.last_block_))
        , first_level_count_(other.first_level_count_)
        , second_level_count_(other.second_level_count_)
        , first_level_bitmap_(other.first_level_bitmap_)
        , second_level_bitmap_(std::move(other.second_level_bitmap_))
        , free_lists_(std::move(other.free_lists_))
    {

    }

    void* TwoLevelSegregatedFitAllocator::Allocate(Bytes size)
    {
        // Structure of the block:
        // || HEADER | BASE_POINTER | ... BLOCK ... ||

        auto block = GetFreeBlockBySize(size + Bytes(sizeof(uintptr_t)));           // Reserve enough space for the block and the base pointer.

        *reinterpret_cast<BlockHeader**>(block->begin()) = block;                   // The base pointer points to the header.

        return MemoryAddress(block->begin()) + Bytes(sizeof(uintptr_t));
    }

    void* TwoLevelSegregatedFitAllocator::Allocate(Bytes size, Alignment alignment)
    {
        // Structure of the block:
        // || HEADER | PADDING | BASE_POINTER | ... ALIGNED BLOCK ... ||

        auto block = GetFreeBlockBySize(size + alignment - 1_Bytes + Bytes(sizeof(uintptr_t)));                         // Reserve enough space for the block, the base pointer and the eventual padding.

        auto aligned_begin = MemoryAddress(block->begin() + Bytes(sizeof(uintptr_t))).GetAligned(alignment);            // First address of the requested aligned block.

        *(aligned_begin - Bytes(sizeof(uintptr_t))).As<BlockHeader*>() = block;                                         // The base pointer points to the header.

        return aligned_begin;
    }

    void TwoLevelSegregatedFitAllocator::Free(void* block)
    {
        // The base pointer is guaranteed to be adjacent to the allocated block.

        auto base_pointer = (MemoryAddress(block) - Bytes(sizeof(uintptr_t))).As<BlockHeader*>();

        PushBlock(*base_pointer);
    }

    bool TwoLevelSegregatedFitAllocator::Owns(void* block) const
    {
        return GetRange().Contains(block);
    }

    Bytes TwoLevelSegregatedFitAllocator::GetMaxAllocationSize() const
    {
        return GetRange().GetSize();
    }

    void TwoLevelSegregatedFitAllocator::Initialize(size_t second_level_count)
    {
        last_block_ = nullptr;

        first_level_count_ = Math::FloorLog2(std::size_t(allocator_.GetRange().GetSize())) + 1u;
        second_level_count_ = second_level_count;

        // Ensure that the bitmaps can store at least one bit per first or second class.
        SYNTROPY_ASSERT(sizeof(first_level_bitmap_) * 8u >= first_level_count_);
        SYNTROPY_ASSERT(sizeof(decltype(second_level_bitmap_)::value_type) * 8u >= (static_cast<size_t>(1u) << second_level_count_));

        free_lists_.resize(first_level_count_ * (static_cast<size_t>(1u) << second_level_count_));

        // Free lists start empty
        first_level_bitmap_ = 0;
        second_level_bitmap_.resize(first_level_count_);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::GetFreeBlockBySize(Bytes size)
    {
        SYNTROPY_PRECONDITION(size > 0_Bytes);

        size += Bytes(sizeof(BlockHeader));                                         // Reserve space for the header.
        size = std::max(size, kMinimumBlockSize);                                   // The size must be at least as big as the minimum size allowed.
        size = Math::Ceil(size, Bytes(BlockHeader::kSizeMask) + 1_Bytes);           // The size must not interfere with the status bits of the block.

        size_t first_level_index;
        size_t second_level_index;

        // Start searching from the next class (the current free list may have blocks that are smaller than the requested size)
        GetFreeListIndex(size, first_level_index, second_level_index, true);

        auto index = GetFreeListIndex(first_level_index, second_level_index);

        if (index < free_lists_.size() && !free_lists_[index])
        {
            // Search a free list with a block which is larger than the requested size.

            auto second_level_bitmap = second_level_bitmap_[first_level_index] & ~((static_cast<uint64_t>(1u) << second_level_index) - 1u);         // Mask out smaller second-level classes.
            auto first_level_bitmap = first_level_bitmap_ & ~((static_cast<uint64_t>(2u) << first_level_index) - 1u);                               // Mask out the current and smaller first-level classes.

            if (second_level_bitmap != 0)
            {
                // Search in the current first-level (there are larger second-level lists that are not empty).
                second_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(second_level_bitmap));

                index = GetFreeListIndex(first_level_index, second_level_index);
            }
            else if (first_level_bitmap != 0)
            {
                // Search in a larger first-level class.
                first_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(first_level_bitmap));
                second_level_index = platform::BuiltIn::GetLeastSignificantBit(static_cast<uint64_t>(second_level_bitmap_[first_level_index]));

                index = GetFreeListIndex(first_level_index, second_level_index);
            }
            else
            {
                // No free list found.
                index = free_lists_.size();
            }
        }

        BlockHeader* block;

        if (index < free_lists_.size())
        {
            SYNTROPY_ASSERT(!!free_lists_[index]);

            // Pop a free block from the given segregated free list.
            block = PopBlock(index);

            // Split the block (if needed).
            SplitBlock(block, size);
        }
        else
        {
            // Get a new block from the pool
            block = AllocateBlock(size);
        }

        SYNTROPY_ASSERT(block->GetSize() >= size);

        return block;
    }

    void TwoLevelSegregatedFitAllocator::SetBitmap(size_t first_level_index, size_t second_level_index)
    {
        first_level_bitmap_ |= (static_cast<size_t>(1u) << first_level_index);
        second_level_bitmap_[first_level_index] |= (static_cast<size_t>(1u) << second_level_index);
    }

    void TwoLevelSegregatedFitAllocator::ResetBitmap(size_t first_level_index, size_t second_level_index)
    {
        second_level_bitmap_[first_level_index] &= ~(static_cast<size_t>(1u) << second_level_index);

        // If all the second-level classes in the current first-level class are empty, the first-level is marked as 'empty' as well.
        if (second_level_bitmap_[first_level_index] == 0)
        {
            first_level_bitmap_ &= ~(static_cast<size_t>(1u) << first_level_index);
        }
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::PopBlock(size_t index)
    {
        auto block = free_lists_[index];

        SYNTROPY_ASSERT(block != nullptr);
        SYNTROPY_ASSERT(!block->IsBusy());

        // Promote the next free block as head of the list
        auto next_free = block->next_free_;

        free_lists_[index] = next_free;

        if (next_free)
        {
            next_free->previous_free_ = nullptr;
        }

        // Fix-up the bitmaps
        if (!free_lists_[index])
        {
            auto first_level_index = index / (static_cast<size_t>(1u) << second_level_count_);
            auto second_level_index = index % (static_cast<size_t>(1u) << second_level_count_);

            ResetBitmap(first_level_index, second_level_index);
        }

        // Mark the popped block as "busy"
        block->SetBusy(true);

        return reinterpret_cast<BlockHeader*>(block);
    }

    void TwoLevelSegregatedFitAllocator::PushBlock(BlockHeader* block)
    {
        auto merged_block = reinterpret_cast<FreeBlockHeader*>(block);

        auto previous_block = reinterpret_cast<FreeBlockHeader*>(block->previous_);
        auto next_block = reinterpret_cast<FreeBlockHeader*>(block->end());

        // Merge the block with the previous physical block

        if (previous_block && !previous_block->IsBusy())
        {
            RemoveBlock(previous_block);                                                // Remove the previous block from its segregated list.

            previous_block->SetSize(previous_block->GetSize() + block->GetSize());      // Grow the block size.
            previous_block->SetLast(block->IsLast());                                   // Merging with the 'last' block yields another 'last' block.

            merged_block = previous_block;                                              // The previous block and the original one are now merged
        }

        // Merge the block with the next physical block

        if (!merged_block->IsLast() && !next_block->IsBusy())
        {
            RemoveBlock(next_block);                                                    // Remove the next block from its segregated list.

            merged_block->SetSize(merged_block->GetSize() + next_block->GetSize());     // Grow the block size.
            merged_block->SetLast(next_block->IsLast());                                // Merging with the 'last' block yields another 'last' block.
        }

        if (merged_block->IsLast())
        {
            last_block_ = merged_block;                                                 // Update the pointer to last block (The previous last block was merged into the new block).
        }
        else
        {
            next_block = reinterpret_cast<FreeBlockHeader*>(merged_block->end());
            next_block->previous_ = merged_block;                                       // The pointer to the previous physical block of the next block is no longer valid since that block may have been merged.
        }

        // Ensures that the block cannot be merged any further

        SYNTROPY_ASSERT(!merged_block->previous_ || merged_block->previous_->IsBusy());
        SYNTROPY_ASSERT(merged_block->IsLast() || reinterpret_cast<BlockHeader*>(merged_block->end())->IsBusy());

        // Insert the merged block in the proper free list

        merged_block->SetBusy(false);                                                   // The block is no longer busy

        InsertBlock(merged_block);
    }

    TwoLevelSegregatedFitAllocator::BlockHeader* TwoLevelSegregatedFitAllocator::AllocateBlock(Bytes size)
    {
        auto block = reinterpret_cast<BlockHeader*>(allocator_.Allocate(size));

        block->SetSize(size);
        block->SetBusy(true);
        block->SetLast(true);

        block->previous_ = last_block_;

        if (last_block_)
        {
            last_block_->SetLast(false);            // The last block is no longer the last
        }

        last_block_ = block;

        return block;
    }

    void TwoLevelSegregatedFitAllocator::SplitBlock(BlockHeader* block, Bytes size)
    {
        SYNTROPY_ASSERT(block->IsBusy());                               // Make sure the original block is busy, otherwise PushBlock will merge again block and remaining_block!

        if (block->GetSize() >= kMinimumBlockSize + size)               // Do not split if the remaining block size would fall below the minimum size allowed.
        {
            auto remaining_block = (MemoryAddress(block) + size).As<BlockHeader>();

            // Setup the new block
            remaining_block->previous_ = block;                         // Previous physical block
            remaining_block->SetBusy(false);                            // This block is free
            remaining_block->SetLast(block->IsLast());                  // This block is the last only if the original one was the last
            remaining_block->SetSize(block->GetSize() - size);          // Remaining size

            // Update the block info
            block->SetSize(size);                                       // Shrink the size
            block->SetLast(false);                                      // It can't be the last block anymore

            PushBlock(remaining_block);                                 // Store the remaining block for further allocations
        }
    }

    void TwoLevelSegregatedFitAllocator::RemoveBlock(FreeBlockHeader* block)
    {
        // Fix-up the double linked list

        if (block->next_free_)
        {
            block->next_free_->previous_free_ = block->previous_free_;
        }

        if (block->previous_free_)
        {
            block->previous_free_->next_free_ = block->next_free_;
        }
        else
        {
            // The block has no previous block: fix the head of the free list
            size_t first_level_index;
            size_t second_level_index;

            GetFreeListIndex(block->GetSize(), first_level_index, second_level_index);

            auto index = GetFreeListIndex(first_level_index, second_level_index);

            free_lists_[index] = block->next_free_;

            if (!free_lists_[index])
            {
                ResetBitmap(first_level_index, second_level_index);
            }
        }
    }

    void TwoLevelSegregatedFitAllocator::InsertBlock(FreeBlockHeader* block)
    {
        // Push on the head of the proper segregated list
        size_t first_level_index;
        size_t second_level_index;

        GetFreeListIndex(block->GetSize(), first_level_index, second_level_index);

        SetBitmap(first_level_index, second_level_index);

        auto index = GetFreeListIndex(first_level_index, second_level_index);

        block->previous_free_ = nullptr;
        block->next_free_ = free_lists_[index];

        if (block->next_free_)
        {
            block->next_free_->previous_free_ = block;
        }

        free_lists_[index] = block;
    }

    size_t TwoLevelSegregatedFitAllocator::GetFreeListIndex(size_t first_level_index, size_t second_level_index) const
    {
        return first_level_index * (static_cast<size_t>(1u) << second_level_count_) + second_level_index;
    }

}
