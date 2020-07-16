
/// \file stack_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources using system stack memory.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_span.h"

namespace syntropy
{
    /************************************************************************/
    /* STACK MEMORY RESOURCE <KSIZE, KALIGNMENT>                            */
    /************************************************************************/

    /// \brief Tier 0 memory resource used to allocate a single block of memory on system stack.
    /// \author Raffaele D. Facendola - August 2018
    template <Int kSize, std::align_val_t kAlignment = std::align_val_t(alignof(void*))>
    class StackMemoryResource
    {
        static_assert(kSize >= alignof(std::max_align_t));

    public:

        /// \brief Create a new memory resource.
        StackMemoryResource() noexcept = default;

        /// \brief No copy constructor.
        StackMemoryResource(const StackMemoryResource&) = delete;

        /// \brief No move constructor.
        StackMemoryResource(StackMemoryResource&& rhs) = delete;

        /// \brief Default destructor.
        ~StackMemoryResource() = default;

        /// \brief No assignment operator.
        StackMemoryResource& operator=(StackMemoryResource rhs) = delete;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        RWMemorySpan Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWMemorySpan& block, Alignment alignment = MaxAlignmentOf());

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        Bool Owns(const MemorySpan& block) const noexcept;

    private:

        /// \brief Stack storage.
        std::aligned_storage_t<kSize, std::size_t(kAlignment)> storage_;

        /// \brief Whether the memory resource is free and can be used for allocation.
        Bool is_free_{ true };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StackMemoryResource<kSize, kAlignment>.

    template <Int kSize, std::align_val_t kAlignment>
    inline RWMemorySpan StackMemoryResource<kSize, kAlignment>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (is_free_ && (size <= Bytes(kSize)) && (alignment <= Alignment(kAlignment)))
        {
            is_free_ = false;

            return { &storage_, size };
        }

        return {};
    }

    template <Int kSize, std::align_val_t kAlignment>
    inline void StackMemoryResource<kSize, kAlignment>::Deallocate(const RWMemorySpan& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(alignment <= Alignment(kAlignment));
        SYNTROPY_ASSERT(Owns(block));

        is_free_ = true;
    }

    template <Int kSize, std::align_val_t kAlignment>
    inline Bool StackMemoryResource<kSize, kAlignment>::Owns(const MemorySpan& block) const noexcept
    {
        return MemorySpan{ &storage_, Bytes{kSize} }.Contains(block);
    }

}

