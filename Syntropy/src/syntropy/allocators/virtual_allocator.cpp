#include "syntropy/allocators/virtual_allocator.h"

#include "syntropy/memory/memory.h"
#include "syntropy/memory/new.h"

namespace Syntropy
{
    /************************************************************************/
    /* VIRTUAL ALLOCATOR :: FREE PAGE INDEX                                 */
    /************************************************************************/

    /// \brief A chunk in the index used to track free unmapped pages.
    /// \author Raffaele D. Facendola - August 2018
    struct VirtualAllocator::FreePageIndex
    {
        /// \brief Next index.
        Pointer<FreePageIndex> next_{ nullptr };

        /// \brief Memory span covering the free page index chunk.
        RWByteSpan self_;

        /// \brief Memory span enclosing the payload.
        RWByteSpan payload_;

        /// \brief Free entries in the index.
        RWSpan<RWByteSpan> free_entries_;

        /// \brief Free pages.
        RWSpan<RWByteSpan> free_pages_;
    };

    /************************************************************************/
    /* VIRTUAL ALLOCATOR                                                    */
    /************************************************************************/

    RWByteSpan VirtualAllocator::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if ((size <= page_size_) && (alignment <= VirtualMemory::PageAlignment()))
        {
            if (auto page = Reserve())
            {
                VirtualMemory::Commit(page);            // Kernel call.

                return Memory::Front(page, size);
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
                return Memory::Front(page, size);
            }
        }

        return {};
    }

    void VirtualAllocator::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        SYNTROPY_ASSERT(Owns(block));

        auto page = RWByteSpan{ Begin(block), ToInt(page_size_) };

        if (free_page_index_ && free_page_index_->free_entries_)
        {
            // Store the page reference inside the current free page index.

            auto [entry, free_entries] = SliceFront(free_page_index_->free_entries_, 1);

            free_page_index_->free_entries_ = free_entries;
            free_page_index_->free_pages_ = Union(free_page_index_->free_pages_, entry);

            Front(entry) = page;

            VirtualMemory::Decommit(page);      // Kernel call.
        }
        else
        {
            // Recycle the page itself as a new free page index.

            auto free_page_index = new (&page) FreePageIndex{};

            free_page_index->next_ = free_page_index_;
            free_page_index->self_ = page;
            free_page_index->payload_ = Memory::PopFront<FreePageIndex>(page);
            free_page_index->free_entries_ = ToRWSpan<RWByteSpan>(Memory::AlignAs<RWByteSpan>(free_page_index->payload_));

            free_page_index_ = free_page_index;
        }
    }

    RWByteSpan VirtualAllocator::Reserve() noexcept
    {
        // Recycle a free page in the current free page index.

        if (free_page_index_ && (free_page_index_->free_pages_))
        {
            auto [entry, free_pages] = SliceBack(free_page_index_->free_pages_, 1);

            free_page_index_->free_pages_ = free_pages;
            free_page_index_->free_entries_ = Union(free_page_index_->free_entries_, entry);

            return Front(entry);
        }

        // Recycle the free page index itself as a page.

        if(free_page_index_)
        {
            auto block = free_page_index_->self_;

            free_page_index_ = free_page_index_->next_;

            return block;
        }
        
        // Allocate on the underlying allocator.

        return allocator_.Reserve(page_size_, ToAlignment(page_size_));
    }

}