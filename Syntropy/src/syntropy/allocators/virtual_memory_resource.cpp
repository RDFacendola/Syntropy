#include "syntropy/allocators/virtual_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE :: FREE LIST                                 */
    /************************************************************************/

    struct VirtualMemoryResource::FreeList
    {
        /// \brief Next free list.
        FreeList* next_{ nullptr };

        /// \brief Next free block.
        MemoryAddress* free_block_{ nullptr };

        /// \brief First free block in the list.
        MemoryAddress first_block_{ nullptr };
    };

    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE                                              */
    /************************************************************************/

    MemoryRange VirtualMemoryResource::Allocate(Bytes size) noexcept
    {
        if (size <= page_size_)
        {
            if (auto block = Allocate())
            {
                VirtualMemory::Commit(block);           // Kernel call: commit the entire block.

                return { block.Begin(), block.Begin() + size };
            }
        }

        return {};
    }

    void VirtualMemoryResource::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(Owns(block));

        if (!free_ || (MemoryAddress{ *free_->free_block_ } >= MemoryAddress(free_) + page_size_))
        {
            // The current free list is full: the block is recycled as a new free list linked to the current one.

            auto next_free = free_;

            free_ = block.Begin().As<FreeList>();

            free_->next_ = next_free;
            free_->first_block_ = block.Begin();
            free_->free_block_ = (&free_->first_block_) + 1;
        }
        else
        {
            // Append to the existing free list.

            *(free_->free_block_) = block.Begin();

            ++(free_->free_block_);

            // Kernel call: decommit the entire block.

            auto virtual_block = MemoryRange(block.Begin(), block.Begin() + page_size_);

            VirtualMemory::Decommit(virtual_block);
        }
    }

    void VirtualMemoryResource::Swap(VirtualMemoryResource& rhs) noexcept
    {
        using std::swap;

        swap(virtual_memory_, rhs.virtual_memory_);
        swap(head_, rhs.head_);
        swap(page_size_, rhs.page_size_);
        swap(page_alignment_, rhs.page_alignment_);
        swap(free_, rhs.free_);
    }

    MemoryRange VirtualMemoryResource::Allocate()
    {
        // Attempt to recycle a free block from the current free list. The last block causes the list itself to be recycled.

        if (free_)
        {
            --(free_->free_block_);

            auto block = MemoryRange{ *free_->free_block_, page_size_ };

            if (block.Begin() == free_)
            {
                free_ = free_->next_;
            }

            return block;
        }

        // Allocate directly from the underlying virtual memory range.

        auto block = head_;

        auto head = block + page_size_;

        if (head <= virtual_memory_.End())
        {
            head_ = head;

            return { block, head_ };
        }

        return {};           // Out of memory!
    }
}