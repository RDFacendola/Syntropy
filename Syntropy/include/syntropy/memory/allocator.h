
/// \file allocator.h
/// \brief This header is part of the Syntropy memory module. It contains definitions for base allocators.
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

    class Allocator;

    /// \brief Get a memory resource that uses the global operator new and operator delete to allocate memory.
    Allocator& GetSystemMemoryResource() noexcept;

    /// \brief Get the thread-local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    Allocator& GetDefaultMemoryResource() noexcept;

    /// \brief Set the thread-local default memory resource.
    /// \return Returns the previous value of the local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    Allocator& SetDefaultMemoryResource(Allocator& memory_resource) noexcept;

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    /// \brief Represents an abstract interface to an unbounded set of classes encapsulating memory resources.
    /// This class mimics and replaces standard's std::memory_resource. Standard's global default memory resoures are deemed
    /// to be harmful since it may cause non-coherent allocation behavior in the same scope.
    /// \author Raffaele D. Facendola - April 2020.
    class Allocator
    {
        friend Allocator& GetSystemMemoryResource() noexcept;
        friend Allocator& GetDefaultMemoryResource() noexcept;
        friend Allocator& SetDefaultMemoryResource(Allocator& memory_resource) noexcept;

    public:

        /// \brief Default virtual destructor.
        virtual ~Allocator() = default;

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
        static Pointer<Allocator>& GetScopeMemoryResource() noexcept;

    };

    /************************************************************************/
    /* ALLOCATOR <ALLOCATOR>                                                */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying, type-erased, memory resource.
    /// \author Raffaele D. Facendola - April 2020
    template <typename TAllocator>
    class AllocatorT : public Allocator
    {
    public:

        /// \brief Create a new memory resource.
        template <typename... TArguments>
        AllocatorT(TArguments&&... arguments) noexcept;

        /// \brief Default virtual destructor.
        virtual ~AllocatorT() = default;

        virtual RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept override;

        virtual Bool Owns(const ByteSpan& block) const noexcept override;

        /// \brief Get the underlying memory resource.
        TAllocator& GetMemoryResource() noexcept;

        /// \brief Get the underlying memory resource.
        const TAllocator& GetMemoryResource() const noexcept;

    private:

        /// \brief Underlying memory resource.
        TAllocator memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    inline Allocator& GetSystemMemoryResource() noexcept
    {
        static auto system_memory_resource = AllocatorT<SystemMemoryResource>{};

        return system_memory_resource;
    }

    inline Allocator& GetDefaultMemoryResource() noexcept
    {
        return *Allocator::GetScopeMemoryResource();
    }

    inline Allocator& SetDefaultMemoryResource(Allocator& memory_resource) noexcept
    {
        auto& previous_memory_resource = GetDefaultMemoryResource();

        Allocator::GetScopeMemoryResource() = &memory_resource;

        return previous_memory_resource;
    }

    // Allocator.
    // ==========

    inline Pointer<Allocator>& Allocator::GetScopeMemoryResource() noexcept
    {
        static thread_local Pointer<Allocator> default_memory_resource_ = &GetSystemMemoryResource();

        return default_memory_resource_;
    }

    // AllocatorT<TAllocator>.
    // ============================

    template <typename TAllocator>
    template <typename... TArguments>
    inline AllocatorT<TAllocator>::AllocatorT(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    inline RWByteSpan AllocatorT<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return memory_resource_.Allocate(size, alignment);
    }

    template <typename TAllocator>
    inline void AllocatorT<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        memory_resource_.Deallocate(block, alignment);
    }

    template <typename TAllocator>
    inline Bool AllocatorT<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TAllocator>
    inline TAllocator& AllocatorT<TAllocator>::GetMemoryResource() noexcept
    {
        return memory_resource_;
    }

    template <typename TAllocator>
    inline const TAllocator& AllocatorT<TAllocator>::GetMemoryResource() const noexcept
    {
        return memory_resource_;
    }

}
