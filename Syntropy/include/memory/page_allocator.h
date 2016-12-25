
/// \file page_allocator.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"

#include <vector>

namespace syntropy
{

    /// \brief Class used to allocate pages on a continuous memory range.
    /// The allocator reserves a range of virtual memory addresses and allocates pages of custom sizes when needed.
    /// Pages can be allocated or returned to the allocator and marked as free.
    /// \author Raffaele D. Facendola - December 2016
    class PageAllocator
    {
    public:

        /// \brief Create a new page allocator.
        /// \param capacity Size of the virtual memory reserved by the allocator in bytes.
        /// \param page_size Size of each allocated page. The size is rounded up the nearest multiple of the system page size.
        PageAllocator(size_t capacity, size_t page_size);

        /// \brief No copy-ctor.
        PageAllocator(const PageAllocator&) = delete;

        /// \brief No assignment operator.
        PageAllocator& operator=(const PageAllocator&) = delete;

        /// \brief Default destructor.
        ~PageAllocator();

        /// \brief Allocate a new page.
        /// \return Returns a pointer to the allocated page.
        void* AllocatePage();

        /// \brief Deallocate a page.
        /// \param page Page to deallocate. Must be a value returned by AllocatePage().
        void DeallocatePage(void* page);

        /// \brief Get the memory size allocated by this allocator.
        /// \return Returns the allocated memory size in bytes.
        size_t GetAllocationSize() const;

    private:

        Memory& memory_;                        ///< \brief Memory instance.

        std::vector<size_t> free_list_;         ///< \brief List of free pages. TODO: use a better structure

        size_t page_size_;                      ///< \brief Page size, in bytes.

        size_t head_;                           ///< \brief Head of the storage.

        int8_t* storage_;                       ///< \brief Base of the storage.

    };

}