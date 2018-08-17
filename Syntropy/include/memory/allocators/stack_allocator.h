
/// \file standard_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains an allocator used to allocate memory on the stack.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK ALLOCATOR                                                      */
    /************************************************************************/

    /// \brief Allocator used to allocate memory over a contiguous range of memory addresses on the stack.
    /// This allocator behaves identically as syntropy::LinearAllocator except it uses stack storage.
    /// \author Raffaele D. Facendola - August 2018
    template <std::size_t kSize, std::size_t kAlignment>
    class StackAllocator : public LinearAllocator
    {
    public:

        /// \brief Default constructor.
        StackAllocator() noexcept;

        /// \brief Default copy constructor.
        StackAllocator(const StackAllocator&) noexcept = default;

        /// \brief Default destructor.
        ~StackAllocator() noexcept = default;

        /// \brief Default assignment operator.
        StackAllocator& operator=(const StackAllocator&) noexcept = default;

    private:

        std::aligned_storage_t<kSize, kAlignment> storage_;         ///< \brief Storage.
    };

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{
    template <std::size_t kSize, std::size_t kAlignment>
    StackAllocator<kSize, kAlignment>::StackAllocator() noexcept
        : LinearAllocator({ MemoryAddress(&storage_), MemoryAddress(&storage_) + Bytes(kSize) })
    {

    }
}

