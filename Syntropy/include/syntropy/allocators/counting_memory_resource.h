
/// \file counting_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains definition for memory resources used to count allocations on another memory resource.
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
    /* COUNTING MEMORY RESOURCE <TMEMORY RESOURCE>                          */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to count allocations performed on another memory resource.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TMemoryResource>
    class CountingMemoryResource
    {
    public:

        /// \brief Create a new counting memory resource.
        /// \param Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        CountingMemoryResource(TArguments&&... arguments) noexcept;

        /// \brief Default copy constructor.
        CountingMemoryResource(const CountingMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        CountingMemoryResource(CountingMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~CountingMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        CountingMemoryResource& operator=(const CountingMemoryResource&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the memory resource owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

        /// \brief Get the amount of active allocations on the underlying memory resource.
        /// \return Returns the number of active allocations on the underlying memory resource.
        Int GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations that were performed on the underlying memory resource, ignoring deallocations.
        /// \return Returns the total amount of allocations that were performed on the underlying memory resource.
        Int GetProgressiveAllocationCount() const noexcept;

    private:

        ///< \brief Number of allocations.
        Int allocation_count_{ 0 };

        ///< \brief Number of deallocations.
        Int deallocation_count_{ 0 };

        ///< \brief Underlying memory resource.
        TMemoryResource memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CountingMemoryResource<TMemoryResource>.
    // ========================================

    template <typename TMemoryResource>
    template <typename... TArguments>
    CountingMemoryResource<TMemoryResource>::CountingMemoryResource(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline RWByteSpan CountingMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = memory_resource_.Allocate(size, alignment))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void CountingMemoryResource<TMemoryResource>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        memory_resource_.Deallocate(block, alignment);

        ++deallocation_count_;
    }

    template <typename TMemoryResource>
    inline Bool CountingMemoryResource<TMemoryResource>::Owns(const ByteSpan& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline Int CountingMemoryResource<TMemoryResource>::GetAllocationCount() const noexcept
    {
        return allocation_count_ - deallocation_count_;
    }

    template <typename TMemoryResource>
    inline Int CountingMemoryResource<TMemoryResource>::GetProgressiveAllocationCount() const noexcept
    {
        return allocation_count_;
    }

}
