
/// \file tracing_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains definition for memory resources used to count allocations on another memory resource.
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
    /* TRACING MEMORY RESOURCE                                              */
    /************************************************************************/

    /// \brief Memory resources used to gather allocation counters.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TMemoryResource>
    class TracingMemoryResource
    {
    public:

        /// \brief Create a new tracing memory resource.
        /// \param Arguments used to construct the underlying memory resource.
        template <typename... TArguments>
        TracingMemoryResource(TArguments&&... arguments);

        /// \brief Default copy constructor.
        TracingMemoryResource(const TracingMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        TracingMemoryResource(TracingMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~TracingMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        TracingMemoryResource& operator=(const TracingMemoryResource&) noexcept = default;

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

        /// \brief Check whether this memory resource owns the provided memory block.
        /// The null memory resource only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Get the amount of active allocations on the underlying memory resource.
        /// \return Returns the number of active allocations on the underlying memory resource.
        std::size_t GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations that were performed on the underlying memory resource, ignoring any deallocation.
        /// \return Returns the total amount of allocations that were performed on the underlying memory resource.
        std::size_t GetProgressiveAllocationCount() const noexcept;

    private:

        std::size_t allocation_count_{ 0u };        ///< \brief Number of allocations.

        std::size_t deallocation_count_{ 0u };      ///< \brief Number of deallocations.

        TMemoryResource memory_resource_;           ///< \brief Underlying memory resource.

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TracingMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    TracingMemoryResource<TMemoryResource>::TracingMemoryResource(TArguments&&... arguments)
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange TracingMemoryResource<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        if (auto block = memory_resource_.Allocate(size))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TMemoryResource>
    inline MemoryRange TracingMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = memory_resource_.Allocate(size, alignment))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void TracingMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        memory_resource_.Deallocate(block);

        ++deallocation_count_;
    }

    template <typename TMemoryResource>
    inline void TracingMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        memory_resource_.Deallocate(block, alignment);

        ++deallocation_count_;
    }

    template <typename TMemoryResource>
    inline bool TracingMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline Bytes TracingMemoryResource<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return memory_resource_.GetMaxAllocationSize();
    }

    template <typename TMemoryResource>
    inline std::size_t TracingMemoryResource<TMemoryResource>::GetAllocationCount() const noexcept
    {
        return allocation_count_ - deallocation_count_;
    }

    template <typename TMemoryResource>
    inline std::size_t TracingMemoryResource<TMemoryResource>::GetProgressiveAllocationCount() const noexcept
    {
        return allocation_count_;
    }

}


