
/// \file quota_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for quota-based allocators.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/language/templates/traits.h"
#include "syntropy/language/foundation.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/allocators/allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* QUOTA ALLOCATOR <ALLOCATOR>                                          */
    /************************************************************************/

    /// \brief Tier Omega allocator used to forward allocations to an underlying allocator up to a given quota.
    /// \author Raffaele D. Facendola - August 2020
    template <typename TAllocator>
    class QuotaAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param quota Maximum size that can be requested from the underlying allocator.
        /// \params arguments Arguments used to construct the underlying allocator.
        template <typename... TArguments>
        QuotaAllocator(Bytes quota, TArguments&&... arguments) noexcept;

        /// \brief No copy constructor.
        QuotaAllocator(const QuotaAllocator&) = delete;

        /// \brief Default move constructor.
        QuotaAllocator(QuotaAllocator&& rhs) noexcept = default;

        /// \brief Default destructor.
        ~QuotaAllocator() = default;

        /// \brief No assignment operator.
        QuotaAllocator& operator=(QuotaAllocator rhs) = delete;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated or the allocator exceeded its quota, returns an empty block.
        Memory::RWByteSpan Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept;

        /// \brief Check whether a block belongs to the underlying allocator.
        /// This method only participates in overload resolution if the underlying allocator implements the ::Own(block) method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsOwn, TAllocator>>
        Bool Owns(const Memory::ByteSpan& block) const noexcept;

        /// \brief Deallocate each allocation performed so far.
        /// This method only participates in overload resolution if the underlying allocator implements ::DeallocateAll() method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsDeallocateAll, TAllocator>>
        void DeallocateAll() noexcept;

        /// \brief Get the maximum memory that can be allocated on the underlying allocator.
        Memory::Bytes GetQuota() const noexcept;

        /// \brief Get the amount of memory currently allocated on the underlying allocator.
        Memory::Bytes GetAllocationSize() const noexcept;

    private:

        /// \brief Underlying allocator.
        TAllocator allocator_;

        /// \brief Maximum size that can be allocated from the underlying allocator.
        Memory::Bytes quota_{ 0 };

        /// \brief Current allocation size.
        Memory::Bytes allocation_size_{ 0 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // QuotaAllocator<TAllocator>.
    // ===========================

    template <typename TAllocator>
    template <typename... TArguments>
    inline QuotaAllocator<TAllocator>::QuotaAllocator(Memory::Bytes quota, TArguments&&... arguments) noexcept
        : allocator_(Forward<TArguments>(arguments)...)
        , quota_(quota)
    {

    }

    template <typename TAllocator>
    inline Memory::RWByteSpan QuotaAllocator<TAllocator>::Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        if ((allocation_size_ + size) <= quota_)
        {
            auto block = allocator_.Allocate(size, alignment);

            allocation_size_ += Size(block);

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void QuotaAllocator<TAllocator>::Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment)
    {
        allocator_.Deallocate(block, alignment);

        allocation_size_ -= Size(block);
    }

    template <typename TAllocator>
    inline Memory::Bytes QuotaAllocator<TAllocator>::GetQuota() const noexcept
    {
        return quota_;
    }

    template <typename TAllocator>
    inline Memory::Bytes QuotaAllocator<TAllocator>::GetAllocationSize() const noexcept
    {
        return allocation_size_;
    }

    template <typename TAllocator>
    template <typename>
    inline Bool QuotaAllocator<TAllocator>::Owns(const Memory::ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    template <typename>
    inline void QuotaAllocator<TAllocator>::DeallocateAll() noexcept
    {
        return allocator_.DeallocateAll(block);
    }

}

