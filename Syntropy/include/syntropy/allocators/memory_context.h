
/// \file memory_context.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for scope-based memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/type_traits.h"

#include "syntropy/allocators/memory_resource.h"
#include "syntropy/allocators/passthrough_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY CONTEXT                                                       */
    /************************************************************************/

    /// \brief Represents a RAII guard to change a memory resource in the current scope and restore the previous one upon destruction.
    /// Memory contexts can be nested but overlapping results in undefined behavior.
    ///
    /// #TODO Syntropy memory resources are not polymorphic, hence the wrapper is necessary most of the times. However if TMemoryResource happens to be polymorphic,
    ///       there's not need for the wrapper and the memory resource can be forwarded as-is and save some performance.
    ///
    /// \author Raffaele D. Facendola - April 2020.
    template <typename TMemoryResource>
    class MemoryContextT
    {
    public:

        /// \brief Set a new memory resource for the current scope.
        template <typename UMemoryResource>
        MemoryContextT(UMemoryResource& memory_resource);

        /// \brief Restore the previous memory resource.
        ~MemoryContextT();

        /// \brief No copy constructor.
        MemoryContextT(const MemoryContextT&) = delete;

        /// \brief No assignment operator.
        MemoryContextT& operator=(const MemoryContextT&) = delete;

    public:

        /// \brief Wrapper to a non-polymorphic memory resource type.
        MemoryResourceT<PassthroughMemoryResource<TMemoryResource>> memory_resource_wrapper_;

        /// \brief Previous memory resource.
        MemoryResource* previous_memory_resource_ = nullptr;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new memory context deducing the type from the arguments.
    template <typename TMemoryResource>
    MemoryContextT<TMemoryResource> MakeMemoryContext(TMemoryResource& memory_resource);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryContext<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename UMemoryResource>
    inline MemoryContextT<TMemoryResource>::MemoryContextT(UMemoryResource& memory_resource)
        : memory_resource_wrapper_(memory_resource)
        , previous_memory_resource_(&SetDefaultMemoryResource(memory_resource_wrapper_))
    {

    }

    template <typename TMemoryResource>
    inline MemoryContextT<TMemoryResource>::~MemoryContextT()
    {
        SetDefaultMemoryResource(*previous_memory_resource_);
    }

    // Non-member functions.

    template <typename TMemoryResource>
    inline MemoryContextT<TMemoryResource> MakeMemoryContext(TMemoryResource& memory_resource)
    {
        return MemoryContextT<TMemoryResource>(memory_resource);
    }

}
