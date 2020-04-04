
/// \file scoped_memory_resource.h
/// \brief This header is part of the syntropy memory management. It contains definitions for scope-based memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/allocators/memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* SCOPED MEMORY RESOURCE                                               */
    /************************************************************************/

    /// \brief Represents a RAII guard to change a memory resource in the current scope and restore the previous one upon destruction.
    /// Scoped memory resources can be nested. Overlapping scoped memory resources results in undefined behavior.
    /// \author Raffaele D. Facendola - April 2020.
    class ScopedMemoryResource
    {
    public:

        /// \brief Set a new memory resource for the current scope.
        ScopedMemoryResource(MemoryResource& memory_resource);

        /// \brief Restore the previous memory resource.
        ~ScopedMemoryResource();

        /// \brief No copy constructor.
        ScopedMemoryResource(const ScopedMemoryResource&) = delete;

        /// \brief No assignment operator.
        ScopedMemoryResource& operator=(const ScopedMemoryResource&) = delete;

    public:

        /// \brief Previous memory resource.
        MemoryResource* previous_memory_resource_ = nullptr;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ScopedMemoryResource.

    inline ScopedMemoryResource::ScopedMemoryResource(MemoryResource& memory_resource)
        : previous_memory_resource_(&MemoryResource::SetDefaultResource(memory_resource))
    {

    }

    inline ScopedMemoryResource::~ScopedMemoryResource()
    {
        MemoryResource::SetDefaultResource(*previous_memory_resource_);
    }

}
