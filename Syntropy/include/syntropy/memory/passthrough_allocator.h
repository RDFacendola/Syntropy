
/// \file passthrough_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains allocators used to forward calls to other allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/core/types.h"
#include "syntropy/allocators/null_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* PASSTHROUGH ALLOCATOR <ALLOCATOR>                                    */
    /************************************************************************/

    /// \brief Tier Omega allocator that forwards requests to another underlying allocator.
    /// If no allocator is bound, this allocator behaves like a syntropy::NullAllocator.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TAllocator>
    class PassthroughAllocator : private NullAllocator
    {
    public:

        /// \brief Default constructor.
        PassthroughAllocator() noexcept = default;

        /// \brief Create a new pass-through allocator.
        PassthroughAllocator(TAllocator& allocator);

        /// \brief Default copy constructor.
        PassthroughAllocator(const PassthroughAllocator&) noexcept = default;

        /// \brief Default move constructor.
        PassthroughAllocator(PassthroughAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~PassthroughAllocator() noexcept = default;

        /// \brief Default assignment operator.
        PassthroughAllocator& operator=(const PassthroughAllocator&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// Forward the allocation request to the underlying allocator, if any, otherwise returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

    private:

        ///< \brief Underlying allocator.
        Pointer<TAllocator> allocator_{ nullptr };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PassthroughAllocator<TAllocator>.
    // =================================

    template <typename TAllocator>
    PassthroughAllocator<TAllocator>::PassthroughAllocator(TAllocator& allocator)
        : allocator_(std::addressof(allocator))
    {

    }

    template <typename TAllocator>
    inline RWByteSpan PassthroughAllocator<TAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (allocator_)
        {
            return allocator_->Allocate(size, alignment);
        }

        return NullAllocator::Allocate(size);
    }

    template <typename TAllocator>
    inline void PassthroughAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        if (allocator_)
        {
            allocator_->Deallocate(block, alignment);
        }
        else
        {
            NullAllocator::Deallocate(block, alignment);
        }
    }

    template <typename TAllocator>
    inline Bool PassthroughAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_ ? allocator_->Owns(block) : NullAllocator::Owns(block);
    }

}
