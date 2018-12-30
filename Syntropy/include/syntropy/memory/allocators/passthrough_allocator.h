
/// \file passthrough_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocator used to reference other allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/allocators/null_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* PASSTHROUGH ALLOCATOR                                                */
    /************************************************************************/

    /// \brief Basic allocator that relays allocation requests to another allocator.
    /// If no allocator is bound, this allocator behaves like a syntropy::NullAllocator.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TAllocator>
    class PassthroughAllocator : private NullAllocator
    {
    public:

        /// \brief Default constructor.
        PassthroughAllocator() noexcept = default;

        /// \brief Create a new passthrough allocator referencing another allocator.
        /// \param allocator Allocator the allocation requests are relayed to.
        PassthroughAllocator(TAllocator& allocator);

        /// \brief Default copy constructor.
        PassthroughAllocator(const PassthroughAllocator&) noexcept = default;

        /// \brief Default move constructor.
        PassthroughAllocator(PassthroughAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~PassthroughAllocator() noexcept = default;

        /// \brief Default assignment operator.
        PassthroughAllocator& operator=(const PassthroughAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate. Expects an empty range.
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Expects an empty range.
        /// \param alignment Block alignment.
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this allocator owns the provided memory block.
        /// The null allocator only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        TAllocator* allocator_{ nullptr };      ///< \brief Allocator the allocation requests are relayed to.
    };

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{
    template <typename TAllocator>
    PassthroughAllocator<TAllocator>::PassthroughAllocator(TAllocator& allocator)
        : allocator_(std::addressof(allocator))
    {

    }

    template <typename TAllocator>
    inline MemoryRange PassthroughAllocator<TAllocator>::Allocate(Bytes size) noexcept
    {
        if (allocator_)
        {
            return allocator_->Allocate(size);
        }

        return NullAllocator::Allocate(size);
    }

    template <typename TAllocator>
    inline MemoryRange PassthroughAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (allocator_)
        {
            return allocator_->Allocate(size, alignment);
        }

        return NullAllocator::Allocate(size);
    }

    template <typename TAllocator>
    inline void PassthroughAllocator<TAllocator>::Deallocate(const MemoryRange& block)
    {
        if (allocator_)
        {
            allocator_->Deallocate(block);
        }
        else
        {
            NullAllocator::Deallocate(block);
        }
    }

    template <typename TAllocator>
    inline void PassthroughAllocator<TAllocator>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        if (allocator_)
        {
            allocator_->Deallocate(block, alignment);
        }
        else
        {
            NullAllocator::Deallocate(block, alignment);
        }
    }

    template <typename TAllocator>
    inline bool PassthroughAllocator<TAllocator>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_ ? allocator_->Owns(block) : NullAllocator::Owns(block);
    }

    template <typename TAllocator>
    inline Bytes PassthroughAllocator<TAllocator>::GetMaxAllocationSize() const noexcept
    {
        return allocator_ ? allocator_->GetMaxAllocationSize() : NullAllocator::GetMaxAllocationSize();
    }
}
