
/// \file allocation.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "memory.h"

namespace syntropy
{

    /// \brief Class used to allocate pages on a continuous memory range.
    /// The allocator reserves a range of virtual memory addresses and allocates pages of custom sizes.
    /// Pages can be allocated or returned to the allocator and marked as free.
    /// \author Raffaele D. Facendola - December 2016
    class PageAllocator
    {
    public:

        ///// \brief Create a new page allocator.
        ///// \param range_size Size of the virtual memory reserved by the allocator. The size is rounded up to the nearest multiple of the system allocation granularity.
        ///// \param page_size Size of each allocated page. The size is rounded up the nearest multiple of the system page size.
        //PageAllocator(size_t range_size, size_t page_size);

        ///// \brief No copy-ctor.
        //PageAllocator(const PageAllocator&) = delete;

        ///// \brief No assignment operator.
        //PageAllocator& operator=(const PageAllocator&) = delete;

        ///// \brief Default destructor.
        //~PageAllocator();

        ///// \brief Allocate a memory page.
        ///// \return Returns a memory block within the reserved memory range. If the allocation could not performed, the returned memory block is not valid.
        //MemoryBlock AllocPage();

        ///// \brief Free an allocated page.
        ///// \param page Page to deallocate.
        //void FreePage(const MemoryBlock& page);

    private:

        MemoryAddressPool free_addresses_;          ///< \brief Pool of free addresses.

    };

}