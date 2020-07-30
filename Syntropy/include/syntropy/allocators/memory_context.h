
/// \file memory_context.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for scope-based memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY CONTEXT                                                       */
    /************************************************************************/

    /// \brief Represents a RAII guard to change a memory resource in the current scope and restore the previous one upon destruction.
    /// Memory contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - April 2020.
    class MemoryContext
    {
    public:

        /// \brief Set a new memory resource for the current scope.
        MemoryContext(Allocator& memory_resource) noexcept;

        /// \brief Restore the previous memory resource.
        ~MemoryContext() noexcept;

        /// \brief No copy constructor.
        MemoryContext(const MemoryContext&) = delete;

        /// \brief No assignment operator.
        MemoryContext& operator=(const MemoryContext&) = delete;

    public:

        /// \brief Previous memory resource.
        Pointer<Allocator> previous_memory_resource_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryContext.
    // ==============

    inline MemoryContext::MemoryContext(Allocator& memory_resource) noexcept
        : previous_memory_resource_(&SetDefaultMemoryResource(memory_resource))
    {

    }

    inline MemoryContext::~MemoryContext() noexcept
    {
        SetDefaultMemoryResource(*previous_memory_resource_);
    }

}
