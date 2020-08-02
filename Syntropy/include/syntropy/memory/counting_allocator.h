
/// \file counting_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains definition for allocators used to gather statistics on other allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace syntropy
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

        /// \brief Create a new allocator resource.
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
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Get the amount of active allocations on the underlying allocator.
        /// \return Returns the number of active allocations on the underlying allocator.
        Int GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations that were performed on the underlying allocator, ignoring deallocations.
        /// \return Returns the total amount of allocations that were performed on the underlying allocator.
        Int GetProgressiveAllocationCount() const noexcept;

    private:

        ///< \brief Number of allocations.
        Int allocation_count_{ 0 };

        ///< \brief Number of deallocations.
        Int deallocation_count_{ 0 };

        ///< \brief Underlying allocator.
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
        : allocator_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    inline RWByteSpan CountingAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void CountingAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        allocator_.Deallocate(block, alignment);

        ++deallocation_count_;
    }

    template <typename TAllocator>
    inline Bool CountingAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
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

}
