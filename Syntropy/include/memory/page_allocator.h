
/// \file page_allocator.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"

#include <memory>

namespace syntropy
{

    class PageAllocator
    {
    public:

        PageAllocator(size_t pages, size_t page_size);

        ~PageAllocator();

        void Push(void* page);

        void* Pop();

        /// \brief Get the page size of the allocator.
        size_t GetPageSize() const;

        /// \brief Get the address of the page containing the provided address.
        void* GetPageAddress(void* address) const;

    private:

        struct Page {

            Page* next_;                            ///< \brief Next page.

        };

        /// \brief Pop an existing free page.
        void* PopFreePage();

        /// \brief Allocate a new unmapped page.
        void* AllocatePage();

        Memory& memory_;                            ///< \brief Used to handle virtual memory.

        size_t page_size_;                          ///< \brief Size of each memory page in bytes.

        int8_t* storage_;                           ///< \brief Pointer to the actual allocator storage.

        Page* free_list_;                           ///< \brief Head of the free page list. The list contains mapped pages only.

        Page* head_;                                ///< \brief Pointer to the first unmapped page. Lower pages are always mapped.
    };

    /// Static allocator something
    /// \author Raffaele D. Facendola
    class SegregatedAllocator
    {
    public:

        /// \brief Minimum allocation size in Bytes.
        /// Allocations lesser than this size are padded up to it.
        static const size_t kMinimumAllocationSize = 8;

        SegregatedAllocator(size_t capacity, size_t page_size, size_t maximum_allocation_size = 256);

        /// \brief Destructor.
        ~SegregatedAllocator();

        void* Allocate(size_t size, size_t alignment);

        void Deallocate(void* address);

    private:

        struct Block
        {
            Block* next_;

            Block* GetNextBlock(size_t block_size);
        };

        struct Page 
        {
            size_t allocations_;                    /// Performed allocation count.

            Block* free_block_;                     /// Next free block.

            Page* next_;                            /// Next free page.

            ///< \brief Get the first block the page.
            Block* GetFirstBlock(size_t block_size);

            ///< \brief Get the last block of the page.
            Block* GetLastBlock(size_t page_size, size_t block_size);
        };

        Page* AllocatePage(size_t block_size);

        PageAllocator free_list_;                   ///< \brief List of free memory pages.

        Page** allocators_;                         ///< \brief Array of allocators. The i-th allocator handles allocations of size less than kMinimumAllocationSize * (i+1) Bytes.

        size_t maximum_allocation_size_;            ///< \brief Maximum allocation size performed by the allocator.

    };

}