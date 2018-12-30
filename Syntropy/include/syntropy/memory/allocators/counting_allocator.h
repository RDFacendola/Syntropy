
/// \file counting_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocator used to count allocations on another allocator.
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
    /* COUNTING ALLOCATOR                                                   */
    /************************************************************************/

    /// \brief Basic allocator that counts allocation on an underlying allocator.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TAllocator>
    class CountingAllocator
    {
    public:

        /// \brief Create a new counting allocator.
        /// \param Arguments used to construct the underlying allocator.
        template <typename... TArguments>
        CountingAllocator(TArguments&&... arguments);

        /// \brief Default copy constructor.
        CountingAllocator(const CountingAllocator&) noexcept = default;

        /// \brief Default move constructor.
        CountingAllocator(CountingAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~CountingAllocator() noexcept = default;

        /// \brief Default assignment operator.
        CountingAllocator& operator=(const CountingAllocator&) noexcept = default;

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

        /// \brief Get the amount of active allocations on the underlying allocator.
        /// \return Returns the number of active allocations on the underlying allocator.k
        std::size_t GetAllocationCount() const noexcept;

        /// \brief Get the total amount of allocations that were performed on the underlying allocator, ignoring any deallocation.
        /// \return Returns the total amount of allocations that were performed on the underlying allocator.
        std::size_t GetProgressiveAllocationCount() const noexcept;

    private:

        std::size_t allocation_count_{ 0u };        ///< \brief Number of allocations performed on the allocator.

        std::size_t deallocation_count_{ 0u };      ///< \brief Number of deallocations performed on the allocator.

        TAllocator allocator_;                      ///< \brief Underlying allocator.
    };

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

namespace syntropy
{

    template <typename TAllocator>
    template <typename... TArguments>
    CountingAllocator<TAllocator>::CountingAllocator(TArguments&&... arguments)
        : allocator_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    inline MemoryRange CountingAllocator<TAllocator>::Allocate(Bytes size) noexcept
    {
        if (auto block = allocator_.Allocate(size))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline MemoryRange CountingAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            ++allocation_count_;

            return block;
        }

        return {};
    }

    template <typename TAllocator>
    inline void CountingAllocator<TAllocator>::Deallocate(const MemoryRange& block)
    {
        allocator_.Deallocate(block);

        ++deallocation_count_;
    }

    template <typename TAllocator>
    inline void CountingAllocator<TAllocator>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        allocator_.Deallocate(block, alignment);

        ++deallocation_count_;
    }

    template <typename TAllocator>
    inline bool CountingAllocator<TAllocator>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator>
    inline Bytes CountingAllocator<TAllocator>::GetMaxAllocationSize() const noexcept
    {
        return allocator_.GetMaxAllocationSize();
    }

    template <typename TAllocator>
    std::size_t CountingAllocator<TAllocator>::GetAllocationCount() const noexcept
    {
        return allocation_count_ - deallocation_count_;
    }

    template <typename TAllocator>
    std::size_t CountingAllocator<TAllocator>::GetProgressiveAllocationCount() const noexcept
    {
        return allocation_count_;
    }
}
