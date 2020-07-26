
/// \file memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/syntropy.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/allocators/system_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    class MemoryResource;

    /// \brief Get a memory resource that uses the global operator new and operator delete to allocate memory.
    MemoryResource& GetSystemMemoryResource() noexcept;

    /// \brief Get the thread-local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    MemoryResource& GetDefaultMemoryResource() noexcept;

    /// \brief Set the thread-local default memory resource.
    /// \return Returns the previous value of the local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept;

    /************************************************************************/
    /* MEMORY RESOURCE                                                      */
    /************************************************************************/

    /// \brief Represents an abstract interface to an unbounded set of classes encapsulating memory resources.
    /// This class mimics and replaces standard's std::memory_resource. Standard's global default memory resoures are deemed
    /// to be harmful since it may cause non-coherent allocation behavior in the same scope.
    /// \author Raffaele D. Facendola - April 2020.
    class MemoryResource
    {
        friend MemoryResource& GetSystemMemoryResource() noexcept;
        friend MemoryResource& GetDefaultMemoryResource() noexcept;
        friend MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept;

    public:

        /// \brief Default virtual destructor.
        virtual ~MemoryResource() = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        virtual RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept = 0;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept = 0;

        /// \brief Check whether the memory resource owns a memory block.
        virtual Bool Owns(const ByteSpan& block) const noexcept = 0;

    private:

        /// \brief Get the active memory resource in the current scope.
        static Pointer<MemoryResource>& GetScopeMemoryResource() noexcept;

    };

    /************************************************************************/
    /* MEMORY RESOURCE T <TMEMORY RESOURCE>                                 */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying, type-erased, memory resource.
    /// \author Raffaele D. Facendola - April 2020
    template <typename TMemoryResource>
    class MemoryResourceT : public MemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        template <typename... TArguments>
        MemoryResourceT(TArguments&&... arguments) noexcept;

        /// \brief Default virtual destructor.
        virtual ~MemoryResourceT() = default;

        virtual RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept override;

        virtual Bool Owns(const ByteSpan& block) const noexcept override;

        /// \brief Get the underlying memory resource.
        TMemoryResource& GetMemoryResource() noexcept;

        /// \brief Get the underlying memory resource.
        const TMemoryResource& GetMemoryResource() const noexcept;

    private:

        /// \brief Underlying memory resource.
        TMemoryResource memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    inline MemoryResource& GetSystemMemoryResource() noexcept
    {
        static auto system_memory_resource = MemoryResourceT<SystemMemoryResource>{};

        return system_memory_resource;
    }

    inline MemoryResource& GetDefaultMemoryResource() noexcept
    {
        return *MemoryResource::GetScopeMemoryResource();
    }

    inline MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept
    {
        auto& previous_memory_resource = GetDefaultMemoryResource();

        MemoryResource::GetScopeMemoryResource() = &memory_resource;

        return previous_memory_resource;
    }

    // MemoryResource.
    // ===============

    inline Pointer<MemoryResource>& MemoryResource::GetScopeMemoryResource() noexcept
    {
        static thread_local Pointer<MemoryResource> default_memory_resource_ = &GetSystemMemoryResource();

        return default_memory_resource_;
    }

    // MemoryResourceT<TMemoryResource>.
    // =================================

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline MemoryResourceT<TMemoryResource>::MemoryResourceT(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline RWByteSpan MemoryResourceT<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return memory_resource_.Allocate(size, alignment);
    }

    template <typename TMemoryResource>
    inline void MemoryResourceT<TMemoryResource>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        memory_resource_.Deallocate(block, alignment);
    }

    template <typename TMemoryResource>
    inline Bool MemoryResourceT<TMemoryResource>::Owns(const ByteSpan& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline TMemoryResource& MemoryResourceT<TMemoryResource>::GetMemoryResource() noexcept
    {
        return memory_resource_;
    }

    template <typename TMemoryResource>
    inline const TMemoryResource& MemoryResourceT<TMemoryResource>::GetMemoryResource() const noexcept
    {
        return memory_resource_;
    }

}
