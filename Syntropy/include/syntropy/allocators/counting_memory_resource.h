
/// \file counting_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains definition for memory resources used to count allocations on another memory resource.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* COUNTING MEMORY RESOURCE <TMEMORY RESOURCE>                          */
    /************************************************************************/

    /// \brief Tier Omega memory resources used to count allocations performed on another memory resource.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TMemoryResource>
    class CountingMemoryResource
    {
    public:

        /// \brief Create a new counting memory resource.
        /// \param Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        CountingMemoryResource(TArguments&&... arguments);

        /// \brief Default copy constructor.
        CountingMemoryResource(const CountingMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        CountingMemoryResource(CountingMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~CountingMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        CountingMemoryResource& operator=(const CountingMemoryResource&) noexcept = default;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Expects an empty range.
        /// \param alignment Block alignment.
        void Deallocate(const MemoryRange& block, Alignment alignment = MaxAlignmentOf());

        /// \brief Check whether this memory resource owns the provided memory block.
        /// The null memory resource only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the amount of active allocations on the underlying memory resource.
        /// \return Returns the number of active allocations on the underlying memory resource.
        std::int64_t GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations that were performed on the underlying memory resource, ignoring any deallocation.
        /// \return Returns the total amount of allocations that were performed on the underlying memory resource.
        std::int64_t GetProgressiveAllocationCount() const noexcept;

    private:

        ///< \brief Number of allocations.
        std::int64_t allocation_count_{ 0 };

        ///< \brief Number of deallocations.
        std::int64_t deallocation_count_{ 0 };

        ///< \brief Underlying memory resource.
        TMemoryResource memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // CountingMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    CountingMemoryResource<TMemoryResource>::CountingMemoryResource(TArguments&&... arguments)
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange CountingMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = memory_resource_.Allocate(size, alignment))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void CountingMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        memory_resource_.Deallocate(block, alignment);

        ++deallocation_count_;
    }

    template <typename TMemoryResource>
    inline bool CountingMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline std::int64_t CountingMemoryResource<TMemoryResource>::GetAllocationCount() const noexcept
    {
        return allocation_count_ - deallocation_count_;
    }

    template <typename TMemoryResource>
    inline std::int64_t CountingMemoryResource<TMemoryResource>::GetProgressiveAllocationCount() const noexcept
    {
        return allocation_count_;
    }

}
