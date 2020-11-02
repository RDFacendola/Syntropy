
/// \file counting_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions for allocators gathering statistics on other allocators.
///
/// \author Raffaele D. Facendola - 2018

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
    /* COUNTING ALLOCATOR <ALLOCATOR>                                       */
    /************************************************************************/

    /// \brief Tier Omega allocator used to count allocations performed on another allocator.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TAllocator>
    class CountingAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param Arguments used to construct the underlying allocator.
        template <typename... TArguments>
        CountingAllocator(TArguments&&... arguments) noexcept;

        /// \brief Default copy constructor.
        CountingAllocator(const CountingAllocator&) noexcept = default;

        /// \brief Default move constructor.
        CountingAllocator(CountingAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~CountingAllocator() noexcept = default;

        /// \brief Default assignment operator.
        CountingAllocator& operator=(const CountingAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        Memory::RWByteSpan Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate each allocation performed so far.
        /// This method only participates in overload resolution if the underlying allocator implements ::DeallocateAll() method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsDeallocateAll, TAllocator>>
        void DeallocateAll() noexcept;

        /// \brief Check whether a block belongs to the underlying allocator.
        /// This method only participates in overload resolution if the underlying allocator implements the ::Own(block) method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsOwn, TAllocator>>
        Bool Owns(const Memory::ByteSpan& block) const noexcept;

        /// \brief Get the amount of active allocations performed on the underlying allocator.
        Int GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations performed on the underlying allocator, ignoring deallocations.
        Int GetProgressiveAllocationCount() const noexcept;

        /// \brief Get the total size of active allocations performed on the underlying allocator.
        Int GetAllocationSize() const noexcept;

        /// \brief Get the total size of allocations performed on the underlying allocator, ignoring deallocate size.
        Int GetProgressiveAllocationSize() const noexcept;

    private:

        /// \brief Progressive number of allocations.
        Int allocation_count_{ 0 };

        /// \brief Progressive number of deallocations.
        Int deallocation_count_{ 0 };

        /// \brief Progressive allocated memory size.
        Memory::Bytes allocation_size_{ ToBytes(0) };

        /// \brief Progressive deallocated memory size.
        Memory::Bytes deallocation_size_{ ToBytes(0) };

        /// \brief Underlying allocator.
        TAllocator allocator_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CountingAllocator<TAllocator>.
    // ==============================

    template <typename TAllocator>
    template <typename... TArguments>
    CountingAllocator<TAllocator>::CountingAllocator(TArguments&&... arguments) noexcept
        : allocator_(Forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    inline Memory::RWByteSpan CountingAllocator<TAllocator>::Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            ++allocation_count_;

            allocation_size_ += size;

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void CountingAllocator<TAllocator>::Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept
    {
        allocator_.Deallocate(block, alignment);

        ++deallocation_count_;

        deallocation_size_ += size_;
    }

    template <typename TAllocator>
    template <typename>
    inline void CountingAllocator<TAllocator>::DeallocateAll() noexcept
    {
        return allocator_.DeallocateAll(block);
    }

    template <typename TAllocator>
    template <typename>
    inline Bool CountingAllocator<TAllocator>::Owns(const Memory::ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    inline Int CountingAllocator<TAllocator>::GetAllocationCount() const noexcept
    {
        return allocation_count_ - deallocation_count_;
    }

    template <typename TAllocator>
    inline Int CountingAllocator<TAllocator>::GetProgressiveAllocationCount() const noexcept
    {
        return allocation_count_;
    }

    template <typename TAllocator>
    inline Int CountingAllocator<TAllocator>::GetAllocationSize() const noexcept
    {
        return allocation_size_ - deallocation_size_;
    }

    template <typename TAllocator>
    inline Int CountingAllocator<TAllocator>::GetProgressiveAllocationSize() const noexcept
    {
        return allocation_size_;
    }
}
