
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
#include "syntropy/memory/system_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    class Allocator;

    /// \brief Exposes methods to get and set the thread-local active allocator.
    namespace Memory
    {
        /// \brief Get the system allocator, an allocator that uses the global operator new and operator delete to allocate and deallocate memory.
        Allocator& GetSystemAllocator() noexcept;

        /// \brief Get the thread-local active allocator.
        /// \remarks The active allocator is used when an explicit allocator cannot be supplied.
        Allocator& GetAllocator() noexcept;

        /// \brief Set the thread-local active allocator.
        /// \return Returns the previous active allocator.
        /// \remarks The active allocator is used when an explicit allocator cannot be supplied.
        Allocator& SetAllocator(Allocator& allocator) noexcept;
    }

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    /// \brief Represents an abstract interface for scope-based allocators.
    /// \author Raffaele D. Facendola - April 2020.
    class Allocator
    {
        friend Allocator& Memory::GetSystemAllocator() noexcept;
        friend Allocator& Memory::GetAllocator() noexcept;
        friend Allocator& Memory::SetAllocator(Allocator&) noexcept;

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

        /// \brief Get the active allocator in the current scope.
        static Pointer<Allocator>& GetScopeAllocator() noexcept;

    };

    /************************************************************************/
    /* ALLOCATOR <ALLOCATOR>                                                */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying, type-erased, allocator.
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

        /// \brief Underlying allocator.
        TAllocator allocator_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Allocator.
    // ==========

    inline Pointer<Allocator>& Allocator::GetScopeAllocator() noexcept
    {
        static thread_local Pointer<Allocator> default_allocator_ = &Memory::GetSystemAllocator();

        return default_allocator_;
    }

    // AllocatorT<TAllocator>.
    // ============================

    template <typename TAllocator>
    template <typename... TArguments>
    inline AllocatorT<TAllocator>::AllocatorT(TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    inline RWByteSpan AllocatorT<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return allocator_.Allocate(size, alignment);
    }

    template <typename TAllocator>
    inline void AllocatorT<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        allocator_.Deallocate(block, alignment);
    }

    template <typename TAllocator>
    inline Bool AllocatorT<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    inline TAllocator& AllocatorT<TAllocator>::GetMemoryResource() noexcept
    {
        return allocator_;
    }

    template <typename TAllocator>
    inline const TAllocator& AllocatorT<TAllocator>::GetMemoryResource() const noexcept
    {
        return allocator_;
    }

    // Memory.
    // =======

    inline Allocator& Memory::GetSystemAllocator() noexcept
    {
        static auto system_allocator = AllocatorT<SystemAllocator>{};

        return system_allocator;
    }

    inline Allocator& Memory::GetAllocator() noexcept
    {
        return *Allocator::GetScopeAllocator();
    }

    inline Allocator& Memory::SetAllocator(Allocator& allocator) noexcept
    {
        auto& previous_allocator = GetAllocator();

        Allocator::GetScopeAllocator() = &allocator;

        return previous_allocator;
    }

}
