
/// \file quota_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains definitions for quota-based allocators.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace syntropy
{
    /************************************************************************/
    /* QUOTA ALLOCATOR <ALLOCATOR>                                          */
    /************************************************************************/

    /// \brief Tier Omega allocator used to relay allocations to an underlying allocator up to a given quota.
    /// This allocator is meant to be used to share a single underlying allocator to many sub-allocators.
    /// \author Raffaele D. Facendola - August 2020
    template <typename TAllocator>
    class QuotaAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param quota Maximum size allocated on the underlying allocator.
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
        RWByteSpan Allocate(Bytes size, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment = MaxAlignmentOf()) noexcept;

        /// \brief Get the maximum memory that can be allocated on the underlying allocator.
        Bytes GetQuota() const noexcept;

        /// \brief Get the amount of memory currently allocated on the underlying allocator.
        Bytes GetAllocationSize() const noexcept;

    private:

        /// \brief Underlying allocator.
        TAllocator allocator_;

        /// \brief Maximum size that can be allocated from the underlying allocator.
        Bytes quota_{ 0 };

        /// \brief Current allocation size.
        Bytes allocation_size_{ 0 };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // QuoteAllocator<TAllocator>.

    template <typename TAllocator>
    template <typename... TArguments>
    inline QuotaAllocator<TAllocator>::QuotaAllocator(Bytes quota, TArguments&&... arguments) noexcept
        : allocator_(std::forward<TArguments>(arguments)...)
        , quota_(quota)
    {

    }

    template <typename TAllocator>
    inline RWByteSpan QuotaAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto quota = quota_ + size; quota <= quota_)
        {
            if (auto block = allocator_.Allocate(size, alignment))
            {
                quota_ = quota;

                return block;
            }
        }

        return {};
    }

    template <typename TAllocator>
    inline void QuotaAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        quota_ -= Size(block);

        SYNTROPY_ASSERT(quota_ >= Bytes{ 0 });

        allocator_.Deallocate(block, alignment);
    }

    template <typename TAllocator>
    inline Bytes QuotaAllocator<TAllocator>::GetQuota() const noexcept
    {
        return quota_;
    }

    template <typename TAllocator>
    inline Bytes QuotaAllocator<TAllocator>::GetAllocationSize() const noexcept
    {
        return allocation_size_;
    }

}

