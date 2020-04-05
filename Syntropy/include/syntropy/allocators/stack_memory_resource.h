
/// \file stack_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources allocating memory on the stack.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK MEMORY RESOURCE                                                */
    /************************************************************************/

    /// \brief Sequential memory resource used to allocate memory over a contiguous range of memory addresses on the stack.
    /// \author Raffaele D. Facendola - August 2018
    template <std::size_t kSize, std::size_t kAlignment = alignof(void*)>
    class StackMemoryResource : public SequentialMemoryResource
    {
    public:

        /// \brief Default constructor.
        StackMemoryResource() noexcept;

        /// \brief Default copy constructor.
        StackMemoryResource(const StackMemoryResource&) noexcept = default;

        /// \brief Default destructor.
        ~StackMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        StackMemoryResource& operator=(const StackMemoryResource&) noexcept = default;

    private:

        /// \brief Stack storage.
        std::aligned_storage_t<kSize, kAlignment> storage_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StackMemoryResourc<kSize, kAlignment>.

    template <std::size_t kSize, std::size_t kAlignment>
    StackMemoryResource<kSize, kAlignment>::StackMemoryResource() noexcept
        : SequentialMemoryResource({ MemoryAddress(&storage_), MemoryAddress(&storage_) + Bytes(kSize) })
    {

    }

}

