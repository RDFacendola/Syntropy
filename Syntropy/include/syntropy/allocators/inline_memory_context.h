
/// \file inline_memory_context.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for inline scope-based memory resources.
///
/// \author Raffaele D. Facendola - May 2020

#pragma once

#include "syntropy/allocators/memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* INLINE MEMORY CONTEXT                                                */
    /************************************************************************/

    /// \brief Represents a RAII guard to change a memory resource in the current scope and restore the previous one upon destruction.
    /// Inline memory contexts wrap their own memory resource: allocations performed on it are expected to be discarded when the guard goes out of scope.
    /// Memory contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TMemoryResource>
    class InlineMemoryContext
    {
    public:

        /// \brief Set a new memory resource for the current scope.
        template <typename... TArguments>
        InlineMemoryContext(TArguments&&... arguments) noexcept;

        /// \brief Restore the previous memory resource.
        ~InlineMemoryContext();

        /// \brief No copy constructor.
        InlineMemoryContext(const InlineMemoryContext&) = delete;

        /// \brief No assignment operator.
        InlineMemoryContext& operator=(const InlineMemoryContext&) = delete;

    public:

        /// \brief Underlying memory resource.
        MemoryResourceT<TMemoryResource> memory_resource_;

        /// \brief Previous memory resource.
        Pointer<MemoryResource> previous_memory_resource_ = nullptr;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // InlineMemoryContext<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline InlineMemoryContext<TMemoryResource>::InlineMemoryContext(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
        , previous_memory_resource_(&SetDefaultMemoryResource(memory_resource_))
    {

    }

    template <typename TMemoryResource>
    inline InlineMemoryContext<TMemoryResource>::~InlineMemoryContext()
    {
        SetDefaultMemoryResource(*previous_memory_resource_);
    }

}
