
/// \file allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains base interfaces and definitions for allocators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/syntropy.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/language/utility.h"
#include "syntropy/language/language.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/allocators/system_allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    class Allocator;

    /// \brief Exposes methods to get and set the thread-local allocators.
    namespace Memory
    {
        /// \brief Get the system allocator, an allocator that uses the global operator new and operator delete to allocate and deallocate memory.
        Allocator& GetSystemAllocator() noexcept;

        /// \brief Get the active thread-local allocator.
        /// \remarks The active allocator is used when an explicit allocator cannot be supplied.
        Allocator& GetAllocator() noexcept;

        /// \brief Set the active thread-local allocator.
        /// \return Returns the previous allocator.
        /// \remarks The active allocator is used when an explicit allocator cannot be supplied.
        Allocator& SetAllocator(Allocator& allocator) noexcept;
    }

    /************************************************************************/
    /* CAPABILITIES                                                         */
    /************************************************************************/

    /// \brief Check whether an allocator implements the method ::Owns(block);
    template <typename TAllocator>
    using AllocatorImplementsOwn = decltype(Declval<TAllocator>().Owns(Declval<ByteSpan>()));

    /// \brief Check whether an allocator implements the method ::DeallocateAll();
    template <typename TAllocator>
    using AllocatorImplementsDeallocateAll = decltype(Declval<TAllocator>().DeallocateAll());

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    /// \brief Represents an abstract interface for allocators.
    /// \author Raffaele D. Facendola - April 2020.
    class Allocator
    {
        friend Allocator& Memory::GetSystemAllocator() noexcept;
        friend Allocator& Memory::GetAllocator() noexcept;
        friend Allocator& Memory::SetAllocator(Allocator&) noexcept;

    public:

        /// \brief Default constructor.
        Allocator() noexcept = default;

        /// \brief No copy constructor.
        Allocator(const Allocator&) noexcept = delete;

        /// \brief Default move constructor.
        Allocator(Allocator&&) noexcept = default;

        /// \brief Default virtual destructor.
        virtual ~Allocator() = default;

        /// \brief Default assignment operator.
        Allocator& operator=(const Allocator&) noexcept = default;

        /// \brief Allocate a memory block.
        /// If a memory block could not be allocated, returns an empty block.
        virtual RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept = 0;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept = 0;

    private:

        /// \brief Get the active allocator in the current scope.
        static RWPointer<Allocator>& GetScopeAllocator() noexcept;

    };

    /************************************************************************/
    /* ALLOCATOR <ALLOCATOR>                                                */
    /************************************************************************/

    /// \brief Represents a polymorphic allocator used to type-erase the concrete type of an underlying allocator.
    /// \author Raffaele D. Facendola - April 2020
    template <typename TAllocator>
    class AllocatorT : public Allocator
    {
    public:

        /// \brief Create a new allocator.
        /// \params arguments Arguments used to construct the underlying allocator.
        template <typename... TArguments>
        AllocatorT(TArguments&&... arguments) noexcept;

        /// \brief Default virtual destructor.
        virtual ~AllocatorT() = default;

        virtual RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept override;

        /// \brief Deallocate each allocation performed so far.
        /// This method only participates in overload resolution if the underlying allocator implements ::DeallocateAll() method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsDeallocateAll, TAllocator>>
        void DeallocateAll() noexcept;

        /// \brief Check whether a block belongs to the underlying allocator.
        /// This method only participates in overload resolution if the underlying allocator implements ::Own(block) method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsOwn, TAllocator>>
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Access the underlying allocator.
        TAllocator& GetAllocator();

        /// \brief Access the underlying allocator.
        const TAllocator& GetAllocator() const;

    private:

        /// \brief Underlying allocator.
        TAllocator allocator_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Allocator.
    // ==========

    inline RWPointer<Allocator>& Allocator::GetScopeAllocator() noexcept
    {
        static thread_local RWPointer<Allocator> default_allocator_ = &Memory::GetSystemAllocator();

        return default_allocator_;
    }

    // AllocatorT<TAllocator>.
    // =======================

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
    template <typename>
    inline void AllocatorT<TAllocator>::DeallocateAll() noexcept
    {
        return allocator_.DeallocateAll();
    }

    template <typename TAllocator>
    template <typename>
    inline Bool AllocatorT<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    inline TAllocator& AllocatorT<TAllocator>::GetAllocator()
    {
        return allocator_;
    }

    template <typename TAllocator>
    inline const TAllocator& AllocatorT<TAllocator>::GetAllocator() const
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
