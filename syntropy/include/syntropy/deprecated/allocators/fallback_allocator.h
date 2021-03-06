
/// \file fallback_allocator.h
/// \brief This header is part of the Syntropy allocators module. It contains allocators that attempt to allocate on a primary allocator and fallback to a second one upon failure.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/templates/traits.h"
#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/language/foundation/foundation.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/allocators/allocator.h"

namespace Syntropy
{
    /************************************************************************/
    /* FALLBACK ALLOCATOR <ALLOCATOR, FALLBACK>                             */
    /************************************************************************/

    /// \brief Tier Omega allocator that attempts to perform allocation on the primary allocator and falls back to the other one upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TAllocator, typename TFallback>
    class FallbackAllocator
    {
    public:

        struct DefaultConstruct {};

        /// \brief Default constructor.
        FallbackAllocator() noexcept = default;

        /// \brief Default copy constructor.
        FallbackAllocator(const FallbackAllocator&) noexcept = default;

        /// \brief Default move constructor.
        FallbackAllocator(FallbackAllocator&&) noexcept = default;

        /// \brief Create a new allocator by initializing both allocators explicitly.
        template <typename TArguments, typename TFallbackArguments>
        FallbackAllocator(TArguments&& arguments, TFallbackArguments&& fallback_arguments) noexcept;

        /// \brief Create a new allocator by initializing the primary allocator explicitly and default-constructing the fallback one.
        template <typename TArguments>
        FallbackAllocator(TArguments&& arguments, DefaultConstruct) noexcept;

        /// \brief Create a new allocator by initializing the fallback allocator explicitly and default-constructing the primary one.
        template <typename TFallbackArguments>
        FallbackAllocator(DefaultConstruct, TFallbackArguments&& fallback_arguments) noexcept;

        /// \brief Default destructor.
        ~FallbackAllocator() noexcept = default;

        /// \brief Default copy-assignment operator.
        FallbackAllocator& operator=(const FallbackAllocator&) noexcept = default;

        /// \brief Default move-assignment operator.
        FallbackAllocator& operator=(FallbackAllocator&&) noexcept = default;

        /// \brief Allocate a new memory block.
        /// If a memory block could not be allocated, returns an empty block.
        Memory::RWByteSpan Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept;

        /// \brief Deallocate each allocation performed so far.
        /// This method only participates in overload resolution if both allocators implement ::DeallocateAll() method.
        template<typename = Templates::EnableIf<Templates::IsValidExpression<AllocatorImplementsDeallocateAll, TAllocator> && Templates::IsValidExpression<AllocatorImplementsDeallocateAll, TFallback>>>
        void DeallocateAll() noexcept;

        /// \brief Check whether either allocator owns a memory block.
        /// This method only participates in overload resolution if both allocator implement ::Own(block) method.
        template<typename = Templates::EnableIf<Templates::IsValidExpression<AllocatorImplementsOwn, TAllocator>&& Templates::IsValidExpression<AllocatorImplementsOwn, TFallback>>>
        Bool Owns(const Memory::ByteSpan& block) const noexcept;

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
        : allocator_(std::make_from_tuple<TAllocator>(Forward<TArguments>(arguments)))
        , fallback_(std::make_from_tuple<TFallback>(Forward<TFallbackArguments>(fallback_arguments)))
    {

    }
    template <typename TAllocator, typename TFallback>
    template <typename TArguments>
    inline FallbackAllocator<TAllocator, TFallback>::FallbackAllocator(TArguments&& arguments, DefaultConstruct) noexcept
        : allocator_(std::make_from_tuple<TAllocator>(Forward<TArguments>(arguments)))
    {

    }

    template <typename TAllocator, typename TFallback>
    template <typename TFallbackArguments>
    inline FallbackAllocator<TAllocator, TFallback>::FallbackAllocator(DefaultConstruct, TFallbackArguments&& fallback_arguments) noexcept
        : fallback_(std::make_from_tuple<TFallback>(Forward<TFallbackArguments>(fallback_arguments)))
    {

    }

    template <typename TAllocator, typename TFallback>
    inline Memory::RWByteSpan FallbackAllocator<TAllocator, TFallback>::Allocate(Memory::Bytes size, Memory::Alignment alignment) noexcept
    {
        if (auto block = allocator_.Allocate(size, alignment))
        {
            return block;
        }

        return fallback_.Allocate(size, alignment);
    }

    template <typename TAllocator, typename TFallback>
    inline void FallbackAllocator<TAllocator, TFallback>::Deallocate(const Memory::RWByteSpan& block, Memory::Alignment alignment) noexcept
    {
        if constexpr (Templates::IsValidExpression<AllocatorImplementsOwn, TAllocator>)
        {
            if (allocator_.Owns(block))
            {
                allocator_.Deallocate(block, alignment);
            }
            else
            {
                fallback_.Deallocate(block, alignment);
            }
        }
        else if constexpr (Templates::IsValidExpression<AllocatorImplementsOwn, TFallback>)
        {
            if (fallback_.Owns(block))
            {
                fallback_.Deallocate(block, alignment);
            }
            else
            {
                allocator_.Deallocate(block, alignment);
            }
        }
        else
        {
            static_assert(Templates::AlwaysFalse<TAllocator, TFallback>, "Cannot infer block owner: either TAllocator or TFallback must provide ::Owns(...) method.");
        }
    }

    template <typename TAllocator, typename TFallback>
    template <typename>
    inline void FallbackAllocator<TAllocator, TFallback>::DeallocateAll() noexcept
    {
        allocator_.DeallocateAll();
        fallback_.DeallocateAll();
    }

    template <typename TAllocator, typename TFallback>
    template <typename>
    inline Bool FallbackAllocator<TAllocator, TFallback>::Owns(const Memory::ByteSpan& block) const noexcept
    {
        allocator_.Owns(block) || fallback_.Owns(block);
    }

}