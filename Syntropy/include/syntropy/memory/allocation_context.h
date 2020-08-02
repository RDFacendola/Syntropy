
/// \file allocation_context.h
/// \brief This header is part of the Syntropy memory module. It contains definitions for scope-based allocation contexts.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/allocator.h"

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
    /* INLINE ALLOCATION CONTEXT                                            */
    /************************************************************************/

    /// \brief Represents a RAII guard to change the allocator in the current scope and restore the previous one upon destruction.
    /// Inline contexts wrap their own allocator: when the allocator goes out of scope accessing the allocated blocks results in undefined behavior.
    /// Memory contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TAllocator>
    class InlineAllocationContext : public AllocationContext
    {
    public:

        /// \brief Set a new allocator for the current scope.
        template <typename... TArguments>
        InlineAllocationContext(TArguments&&... arguments) noexcept;

        /// \brief Restore the previous allocator.
        ~InlineAllocationContext() = default;

        /// \brief No copy constructor.
        InlineAllocationContext(const InlineAllocationContext&) = delete;

        /// \brief No assignment operator.
        InlineAllocationContext& operator=(const InlineAllocationContext&) = delete;

    public:

        /// \brief Underlying allocator.
        AllocatorT<TAllocator> allocator_;

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

    // InlineAllocationContext<TAllocator>.
    // ====================================

    template <typename TAllocator>
    template <typename... TArguments>
    inline InlineAllocationContext<TAllocator>::InlineAllocationContext(TArguments&&... arguments) noexcept
        : AllocationContext(&allocator_)
        , allocator_(std::forward<TArguments>(arguments)...)
    {

    }

}
