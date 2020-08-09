#include "syntropy/allocators/virtual_allocator.h"

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
        /// \brief Self free-list node span.
        RWByteSpan self_;

        /// \brief Payload range.
        RWSpan<RWByteSpan> payload_;

        /// \brief Free pages.
        RWSpan<RWByteSpan> free_pages_;

        /// \brief Next free list node.
        Pointer<FreeList> next_{ nullptr };
    };

    /************************************************************************/
    /* VIRTUAL MEMORY RESOURCE                                              */
    /************************************************************************/

    RWByteSpan VirtualAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= page_size_) && (alignment <= VirtualMemory::PageAlignment()))
        {
            if (auto page = Reserve())
            {
                VirtualMemory::Commit(page);                     // Kernel call: commit the page(s).

                return Front(page, ToInt(size));
            }
        }

        return {};
    }

    RWByteSpan VirtualAllocator::Reserve(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= page_size_) && (alignment <= VirtualMemory::PageAlignment()))
        {
            if (auto page = Reserve())
            {
                return Front(page, ToInt(size));
            }
        }

        return {};
    }

    void VirtualAllocator::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        SYNTROPY_ASSERT(Owns(block));
        SYNTROPY_ASSERT(alignment <= VirtualMemory::PageAlignment());

        auto page = RWByteSpan{ Begin(block), ToInt(page_size_) };      // Blocks are allocated at page boundary.

        if (free_list_ && (free_list_->free_pages_ != free_list_->payload_))
        {
            // (a) Link to free pages in the current free list node.

            VirtualMemory::Decommit(page);                       // Kernel call: decommit the page(s).

            // #TODO FIFO Policy.

            auto available = DifferenceBack(free_list_->payload_, free_list_->free_pages_);

            new (Begin(available)) RWByteSpan{ page };

            free_list_->free_pages_ = DifferenceFront(free_list_->payload_, PopFront(available));
        }
        else
        {
            // (b) Recycle the deallocated block as a new free list node.

            auto next = free_list_;

            free_list_ = new (block.GetData()) FreeList{};

            // #TODO Boilerplate!

            auto payload = PopFront(page, ToInt(Memory::SizeOf<FreeList>()));
            
            payload = Front(payload, Math::Floor(Count(payload), ToInt(Memory::SizeOf<RWByteSpan>())));     // #TODO Floor (reduce from the back), Ceil (reduce from the front).

            free_list_->self_ = page;
            free_list_->payload_ = ToRWSpan<RWByteSpan>(payload);
            free_list_->free_pages_ = {};
            free_list_->next_ = next;
        }

    }

    void VirtualAllocator::Swap(VirtualAllocator& rhs) noexcept
    {
        using std::swap;

        swap(virtual_span_, rhs.virtual_span_);
        swap(unallocated_span_, rhs.unallocated_span_);
        swap(page_size_, rhs.page_size_);
        swap(free_list_, rhs.free_list_);
    }

    RWByteSpan VirtualAllocator::Reserve() noexcept
    {
        // Recycle a free block.

        if (free_list_ && (free_list_->free_pages_))
        {
            // #TODO FIFO Policy.

            auto block = Back(free_list_->free_pages_);

            free_list_->free_pages_ = PopBack(free_list_->free_pages_);

            return block;
        }

        // Recycle itself as a free block.

        if(free_list_)
        {
            auto block = free_list_->self_;

            free_list_ = free_list_->next_;

            return block;
        }
        
        // Allocate from the end.

        if (page_size_ <= Memory::Size(unallocated_span_))
        {
            auto [page, unallocated_span] = SliceFront(unallocated_span_, ToInt(page_size_));

            unallocated_span_ = unallocated_span;

            return page;
        }

        // Out of memory!

        return {};
    }

}