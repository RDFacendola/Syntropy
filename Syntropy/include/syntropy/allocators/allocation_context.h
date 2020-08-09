
/// \file allocation_context.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for scope-based allocation contexts.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/allocators/allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* ALLOCATION CONTEXT                                                   */
    /************************************************************************/

    /// \brief Represents a RAII guard to change the allocator in the current scope and restore the previous one upon destruction.
    /// Allocation contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - April 2020.
    class AllocationContext
    {
    public:

        /// \brief Set a new allocator for the current scope.
        AllocationContext(Allocator& allocator) noexcept;

        /// \brief Restore the previous allocator.
        ~AllocationContext() noexcept;

        /// \brief No copy constructor.
        AllocationContext(const AllocationContext&) = delete;

        /// \brief No assignment operator.
        AllocationContext& operator=(const AllocationContext&) = delete;

    public:

        /// \brief Previous allocator.
        Pointer<Allocator> previous_allocator_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AllocationContext.
    // ==================

    inline AllocationContext::AllocationContext(Allocator& allocator) noexcept
        : previous_allocator_(&Memory::SetAllocator(allocator))
    {

    }

    inline AllocationContext::~AllocationContext() noexcept
    {
        Memory::SetAllocator(*previous_allocator_);
    }

}
