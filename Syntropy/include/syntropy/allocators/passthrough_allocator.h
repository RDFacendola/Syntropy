
/// \file passthrough_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains allocators used to forward calls to other allocators.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/language/language.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/allocators/null_allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* PASSTHROUGH ALLOCATOR <ALLOCATOR>                                    */
    /************************************************************************/

    /// \brief Tier Omega allocator that forwards requests to another allocator.
    /// \author Raffaele D. Facendola - September 2018
    template <typename TAllocator>
    class PassthroughAllocator
    {
    public:

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
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Deallocate each allocation performed so far.
        /// This method only participates in overload resolution if the underlying allocator implements ::DeallocateAll() method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsDeallocateAll, TAllocator>>
        void DeallocateAll() noexcept;

        /// \brief Check whether a block belongs to the underlying allocator.
        /// This method only participates in overload resolution if the underlying allocator implements the ::Own(block) method.
        template<typename = EnableIfValidExpressionT<AllocatorImplementsOwn, TAllocator>>
        Bool Owns(const ByteSpan& block) const noexcept;

    private:

        ///< \brief Underlying allocator.
        RWPointer<TAllocator> allocator_{ nullptr };

    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type used to adapt any concrete allocator type to a polymorphic allocator interface.
    /// \see AllocatorT<...>
    template <typename TAllocator>
    using AllocatorReference = AllocatorT<PassthroughAllocator<TAllocator>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new allocator reference, wrapping an existing allocator.
    template <typename TAllocator>
    AllocatorReference<TAllocator> MakeAllocatorReference(TAllocator& allocator) noexcept;

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
        return allocator_->Allocate(size, alignment);
    }

    template <typename TAllocator>
    inline void PassthroughAllocator<TAllocator>::Deallocate(const RWByteSpan& block, Alignment alignment)
    {
        allocator_->Deallocate(block, alignment);
    }

    template <typename TAllocator>
    template <typename>
    inline void PassthroughAllocator<TAllocator>::DeallocateAll() noexcept
    {
        return allocator_->DeallocateAll(block);
    }

    template <typename TAllocator>
    template <typename>
    inline Bool PassthroughAllocator<TAllocator>::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_->Owns(block);
    }

    // Non-member functions.
    // =====================

    template <typename TAllocator>
    inline AllocatorReference<TAllocator> MakeAllocatorReference(TAllocator& allocator) noexcept
    {
        return PassthroughAllocator<TAllocator>(allocator);
    }

}
