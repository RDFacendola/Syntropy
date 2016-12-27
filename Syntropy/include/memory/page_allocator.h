
/// \file page_allocator.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"

#include <vector>

namespace syntropy
{

    /// Used to store and recycle free page indexes.
    /// \author Raffaele D. Facendola - December 2016
    class PagePool
    {
    public:

        /// \brief Create a new page pool.
        /// \param max_pages Maximum number of pages that can be stored inside the pool.
        PagePool(size_t max_pages);

        /// \brief No copy constructor.
        PagePool(const PagePool&) = delete;

        /// \brief No assignment operator.
        PagePool& operator=(const PagePool&) = delete;

        /// \brief Destructor.
        ~PagePool();

        /// \brief Acquire a free page index.
        /// \param page_index Index of the free page to acquire.
        void Push(size_t page_index);

        /// \brief Get a free page index.
        /// If no index could be recycled, a new index is generated.
        /// \return Returns a free page index.
        size_t Pop();

        /// \brief Get the number of non-free page indexes.
        /// \return Returns the number of page indexes that were popped but not yet reacquired.
        size_t GetAllocatedPages() const;

    private:

        Memory& memory_;                        ///< \brief Memory instance.

        size_t* storage_;                       ///< \brief Storage for free page indexes.

        size_t size_;                           ///< \brief Free page indexes count.

        size_t capacity_;                       ///< \brief Storage capacity. Represents the actual amount of elements that have been allocated on the storage.

        size_t head_;                           ///< \brief Highest page index generated so far.
    };

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

        PagePool page_pool_;                    ///< \brief Pool of free page indexes.

        size_t page_size_;                      ///< \brief Page size, in bytes.

        int8_t* storage_;                       ///< \brief Base of the storage.

    };

}