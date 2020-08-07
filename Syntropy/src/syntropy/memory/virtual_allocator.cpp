#include "syntropy/memory/virtual_allocator.h"

#include "syntropy/memory/memory.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE :: FREE LIST                                 */
    /************************************************************************/

    /// \brief Type of a linked list used to track free pages.
    /// \author Raffaele D. Facendola - August 2018
    struct VirtualAllocator::FreeList
    {
        /// \brief Next free node.
        Pointer<FreeList> next_{ nullptr };

        /// \brief Available memory in the node.
        RWSpan<RWBytePtr> storage_;

        /// \brief Unallocated space in the node.
        RWSpan<RWBytePtr> unallocated_;
    };

    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE                                              */
    /************************************************************************/

    RWByteSpan VirtualAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= page_size_) && (alignment <= page_alignment_))
        {
            if (auto block = Reserve())
            {
                Memory::Commit(block);                      // Kernel call: commit the entire block.

                return Front(block, ToInt(size));           // Limit to the requested amount.
            }
        }

        return {};
    }

    RWByteSpan VirtualAllocator::Reserve(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= page_size_) && (alignment <= page_alignment_))
        {
            if (auto block = Reserve())
            {
                return Front(block, ToInt(size));           // Limit to the requested amount.
            }
        }

        return {};
    }

    void VirtualAllocator::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        SYNTROPY_ASSERT(Owns(block));
        SYNTROPY_ASSERT(alignment <= page_alignment_);

        static constexpr auto kHeaderSize = Memory::SizeOf<FreeList>();
        static constexpr auto kEntrySize = Memory::SizeOf<RWBytePtr>();

        auto storage = RWByteSpan{ Begin(block), ToInt(page_size_) };               // Grow to page capacity.

        if (!free_ || IsEmpty(free_->unallocated_))
        {
            // The free list is full: the block is recycled as a new free node.

            auto next_free = free_;

            free_ = FromTypeless<FreeList>(storage.GetData());                      // Link to the next free node.

            storage = PopFront(storage, ToInt(kHeaderSize));                        // Reduce the storage to account for the header size.

            storage = Memory::Align(storage, ToAlignment(kEntrySize));              // Align the span to an integer number of free entries.

            //

            free_->next_ = next_free;

            free_->storage_ = ToRWSpan<RWBytePtr>(storage);                         // Recycle remaining storage for new entries.

            free_->unallocated_ = free_->storage_;
        }
        else
        {
            // Append to the existing free list node.

            Front(free_->unallocated_) = Begin(storage);

            free_->unallocated_ = PopFront(free_->unallocated_);

            Memory::Decommit(storage);                                              // Kernel call: decommit the entire block.
        }
    }

    void VirtualAllocator::Swap(VirtualAllocator& rhs) noexcept
    {
        using std::swap;

        swap(virtual_storage_, rhs.virtual_storage_);
        swap(virtual_unallocated_, rhs.virtual_unallocated_);
        swap(page_size_, rhs.page_size_);
        swap(page_alignment_, rhs.page_alignment_);
        swap(free_, rhs.free_);
    }

    RWByteSpan VirtualAllocator::Reserve() noexcept
    {
        if (free_ && (free_->unallocated_ != free_->storage_))
        {
            auto entries = DifferenceBack(free_->storage_, free_->unallocated_);                // Take the first entry.

            auto block = RWByteSpan{ Front(entries), ToInt(page_size_) };

            free_->unallocated_ = Intersection(free_->storage_, PopFront(entries));             // Recompute the unallocated space.

            return block;
        }
        else if(free_)
        {
            auto block = RWByteSpan{ FromTypeless<Byte>(free_), ToInt(page_size_) };            // Recycle the node itself as a block.

            free_ = free_->next_;                                                               // Move to the next free node (if any).

            return block;
        }
        else if (Memory::Size(virtual_unallocated_) <= page_size_)                              // Allocate from the underlying virtual memory range.
        {
            auto block = Front(virtual_unallocated_, ToInt(page_size_));

            virtual_unallocated_ = PopFront(virtual_unallocated_, ToInt(page_size_));

            return block;
        }

        return {};           // Out of memory!
    }

}