
/// \file memory_resource.h
/// \brief This header is part of the syntropy memory management. It contains definitions and interfaces for memory resource.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/allocators/heap_memory_resource.h"
#include "syntropy/allocators/polymorphic_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RESOURCE                                                      */
    /************************************************************************/

    /// \brief Represents an abstract interface to an unbounded set of classes encapsulating memory resources.
    /// This class mimics and replaces standard's std::memory_resource. Standard's global default memory resoures are deemed
    /// to be harmful since it may cause non-coherent allocation behavior in the same scope.
    /// \author Raffaele D. Facendola - April 2020.
    class MemoryResource
    {
    public:

        /// \brief Get a memory resource that uses the global operator new and operator delete to allocate memory.
        static PolymorphicMemoryResource& GetNewDeleteResource() noexcept;

        /// \brief Get the thread-local default memory resource.
        /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
        static PolymorphicMemoryResource& GetDefaultResource() noexcept;

        /// \brief Set the thread-local default memory resource.
        /// \return Returns the previous value of the local default memory resource.
        /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
        static PolymorphicMemoryResource& SetDefaultResource(PolymorphicMemoryResource& memory_resource) noexcept;

    private:

        /// \brief Thread-local default memory resource.
        static inline thread_local PolymorphicMemoryResource* default_memory_resource_ = &MemoryResource::GetNewDeleteResource();

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryResource.

    inline PolymorphicMemoryResource& MemoryResource::GetDefaultResource() noexcept
    {
        return *default_memory_resource_;
    }

    inline PolymorphicMemoryResource& MemoryResource::SetDefaultResource(PolymorphicMemoryResource& memory_resource) noexcept
    {
        auto& previous_memory_resource = GetDefaultResource();

        default_memory_resource_ = &memory_resource;

        return previous_memory_resource;
    }

    inline PolymorphicMemoryResource& MemoryResource::GetNewDeleteResource() noexcept
    {
        static auto new_delete_resource = PolymorphicMemoryResourceT<HeapMemoryResource>{};

        return new_delete_resource;
    }
}
