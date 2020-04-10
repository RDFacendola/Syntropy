
/// \file fixed_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources allocating a single memory block.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* FIXED MEMORY RESOURCE                                                */
    /************************************************************************/

    /// \brief Tier 1 memory resource used to allocate a single block of memory from an underlying allocator.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource>
    class FixedMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        /// \param arguments Arguments used to construct the underlying memory allocator.
        /// \param max_size Maximum size for an allocated block.
        /// \param max_alignment Maximum alignment for an allocated block.
        template <typename... TArguments>
        FixedMemoryResource(Bytes max_size, Alignment max_alignment, TArguments&&... arguments);

        /// \brief No copy constructor.
        FixedMemoryResource(const FixedMemoryResource&) = delete;

        /// \brief Default move constructor.
        FixedMemoryResource(FixedMemoryResource&& rhs) = default;

        /// \brief Default destructor.
        ~FixedMemoryResource() = default;

        /// \brief No assignment operator.
        FixedMemoryResource& operator=(FixedMemoryResource rhs) = delete;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        /// \brief Underlying memory resource.
        TMemoryResource memory_resource_;

        /// \brief Maximum block size.
        Bytes max_size_;

        /// \brief Maximum block alignment.
        Alignment max_alignment_;

        /// \brief Allocated memory block. Empty if no allocation was performed.
        MemoryRange block_;

        /// \brief Whether the memory resource is free and can be used for allocation.
        bool is_free_{ true };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FixedMemoryResource<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline FixedMemoryResource<TMemoryResource>::FixedMemoryResource(Bytes max_size, Alignment max_alignment, TArguments&&... arguments)
        : memory_resource_(std::forward<TArguments>(arguments)...)
        , max_size_(max_size)
        , max_alignment_(max_alignment)
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange FixedMemoryResource<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        if (is_free_ && size <= max_size_ && (block_ = memory_resource_.Allocate(max_size_)))
        {
            is_free_ = false;

            return { block_.Begin(), size };
        }

        return {};
    }

    template <typename TMemoryResource>
    inline MemoryRange FixedMemoryResource<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (is_free_ && size <= max_size_ && alignment <= max_alignment_ && (block_ = memory_resource_.Allocate(max_size_, alignment)))
        {
            is_free_ = false;

            return { block_.Begin(), size };
        }

        return {};
    }

    template <typename TMemoryResource>
    inline void FixedMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(Owns(block));

        memory_resource_.Deallocate(block_);

        is_free_ = true;
    }

    template <typename TMemoryResource>
    inline void FixedMemoryResource<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(Owns(block));
        SYNTROPY_ASSERT(alignment <= max_alignment_);

        memory_resource_.Deallocate(block_, alignment);

        is_free_ = true;
    }

    template <typename TMemoryResource>
    inline bool FixedMemoryResource<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return !is_free_ && block_.Contains(block);
    }

    template <typename TMemoryResource>
    inline Bytes FixedMemoryResource<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return std::min(max_size_, memory_resource_.GetMaxAllocationSize());
    }

}

