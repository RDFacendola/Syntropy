#include "syntropy/memory/allocators/segregated_allocator.h"

#include <iterator>
#include <algorithm>
#include <new>

#include "syntropy/memory/virtual_memory.h"

#include "syntropy/diagnostics/foundation/assert.h"

#include "syntropy/math/math.h"

#include "syntropy/platform/builtin.h"

namespace syntropy 
{

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
    
    const Bytes TwoLevelSegregatedFitAllocator::kMinimumBlockSize(32);

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
        : Allocator(Move(other))
        , allocator_(Move(other.allocator_))
        , last_block_(Move(other.last_block_))
        , first_level_count_(other.first_level_count_)
        , second_level_count_(other.second_level_count_)
        , first_level_bitmap_(other.first_level_bitmap_)
        , second_level_bitmap_(Move(other.second_level_bitmap_))
        , free_lists_(Move(other.free_lists_))
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
        std::lock_guard<std::mutex> lock(mutex_);

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

    const MemoryRange& TwoLevelSegregatedFitAllocator::GetRange() const
    {
        return allocator_.GetRange();
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

        std::lock_guard<std::mutex> lock(mutex_);

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

    void TwoLevelSegregatedFitAllocator::GetFreeListIndex(Bytes size, size_t& first_level_index, size_t& second_level_index, bool roundup) const
    {
        if (roundup)
        {
            // Round up to the next class size.
            size = size + Bytes(size_t(1u) << (Math::FloorLog2(std::size_t(size)) - second_level_count_)) - 1_Bytes;
        }

        first_level_index = Math::FloorLog2(std::size_t(size));

        second_level_index = (std::size_t(size) ^ (std::size_t(1u) << first_level_index)) >> (first_level_index - second_level_count_);
    }

    size_t TwoLevelSegregatedFitAllocator::GetFreeListIndex(size_t first_level_index, size_t second_level_index) const
    {
        return first_level_index * (static_cast<size_t>(1u) << second_level_count_) + second_level_index;
    }

}
