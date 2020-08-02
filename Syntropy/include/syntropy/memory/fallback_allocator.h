
/// \file fallback_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains allocators that attempt to allocate on a primary allocator and fallback to a second one on failure.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/utility.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

namespace syntropy
{
    /************************************************************************/
    /* FALLBACK ALLOCATOR                                                   */
    /************************************************************************/

    /// \brief Tier Omega allocator that attempts to perform allocation on the primary allocator and falls back to the other one upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator, typename TFallback>
    class FallbackAllocator
    {
    public:

        /// \brief Default constructor.
        /// This method assumes each allocator is default-constructible.
        FallbackAllocator() noexcept = default;

        /// \brief Default copy constructor.
        /// This method assumes each allocator is copyable.
        FallbackAllocator(const FallbackAllocator&) noexcept = default;

        /// \brief Default move constructor.
        /// This method assumes each allocator is movable.
        FallbackAllocator(FallbackAllocator&&) noexcept = default;

        /// \brief Create a new allocator by initializing both allocators explicitly.
        template <typename TArguments, typename TFallbackArguments>
        FallbackAllocator(TArguments&& arguments, TFallbackArguments&& fallback_arguments) noexcept;

        /// \brief Create a new allocator by initializing the primary allocator explicitly and default-constructing the fallback one.
        template <typename TArguments>
        FallbackAllocator(TArguments&& arguments, DefaultConstructT) noexcept;

        /// \brief Create a new allocator by initializing the fallback allocator explicitly and default-constructing the primary one.
        template <typename TFallbackArguments>
        FallbackAllocator(DefaultConstructT, TFallbackArguments&& fallback_arguments) noexcept;

        /// \brief Default destructor.
        ~FallbackAllocator() noexcept = default;

        /// \brief Default copy-assignment operator.
        /// This method assumes each allocator is copy-assignable.
        FallbackAllocator& operator=(const FallbackAllocator&) noexcept = default;

        /// \brief Default move-assignment operator.
        /// This method assumes each allocator is copy-assignable.
        FallbackAllocator& operator=(FallbackAllocator&&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If the primary allocator could not handle the request, the fallback allocator will be used instead.
        /// If a memory block could not be allocated, returns an empty block.
        RWByteSpan Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const RWByteSpan& block, Alignment alignment) noexcept;

        /// \brief Check whether the allocator owns a memory block.
        Bool Owns(const ByteSpan& block) const noexcept;

    private:

        ///< \brief Primary allocator
        TAllocator allocator_;

        ///< \brief Fallback allocator used upon failure.
        TFallback fallback_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FallbackAllocator<TAllocator, TFallback>.
    // =========================================

    template <typename TAllocator, typename TFallback>
    template <typename TArguments, typename TFallbackArguments>
    inline FallbackAllocator<TAllocator, TFallback>::FallbackAllocator(TArguments&& arguments, TFallbackArguments&& fallback_arguments) noexcept
        : allocator_(std::make_from_tuple<TAllocator>(std::forward<TArguments>(arguments)))
        , fallback_(std::make_from_tuple<TFallback>(std::forward<TFallbackArguments>(fallback_arguments)))
    {

    }
    template <typename TAllocator, typename TFallback>
    template <typename TArguments>
    inline FallbackAllocator<TAllocator, TFallback>::FallbackAllocator(TArguments&& arguments, DefaultConstructT) noexcept
        : allocator_(std::make_from_tuple<TAllocator>(std::forward<TArguments>(arguments)))
    {

    }

    template <typename TAllocator, typename TFallback>
    template <typename TFallbackArguments>
    inline FallbackAllocator<TAllocator, TFallback>::FallbackAllocator(DefaultConstructT, TFallbackArguments&& fallback_arguments) noexcept
        : fallback_(std::make_from_tuple<TFallback>(std::forward<TFallbackArguments>(fallback_arguments)))
    {

    }

    template <typename TAllocator, typename TFallback>
    inline RWByteSpan FallbackAllocator<TAllocator, TFallback>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            return block;
        }

        return fallback_.Allocate(size, alignment);
    }

    template <typename TAllocator, typename TFallback>
    inline void FallbackAllocator<TAllocator, TFallback>::Deallocate(const RWByteSpan& block, Alignment alignment) noexcept
    {
        SYNTROPY_UNDEFINED_BEHAVIOR(Owns(block), "The provided block doesn't belong to this allocator.");

        if (allocator_.Owns(block))
        {
            allocator_.Deallocate(block, alignment);
        }
        else
        {
            fallback_.Deallocate(block, alignment);
        }
    }

    template <typename TAllocator, typename TFallback>
    inline Bool FallbackAllocator<TAllocator, TFallback>::Owns(const ByteSpan& block) const noexcept
    {
        return allocator_.Owns(block) || fallback_.Owns(block);
    }

}