
/// \file chain_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators that defer actual allocation to other allocators according to different policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* CHAIN ALLOCATOR                                                      */
    /************************************************************************/

    /// \brief Basic allocator that attempts to perform allocation on the primary allocator and falls back to other allocators upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename THeadAllocator, typename... TRestAllocators>
    class ChainAllocator : private THeadAllocator, private ChainAllocator<TRestAllocators...>
    {
        /// \brief Default constructor.
        ChainAllocator() noexcept = default;

        /// \brief Default copy constructor.
        ChainAllocator(const ChainAllocator&) noexcept = default;

        /// \brief Default move constructor.
        ChainAllocator(ChainAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~ChainAllocator() noexcept = default;

        /// \brief Default assignment operator.
        ChainAllocator& operator=(const ChainAllocator&) noexcept = default;

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Check whether this allocator owns the provided memory block.
        /// The null allocator only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;
    };

    /// \brief Partial template specialization for a chain allocator with a single underlying allocator.
    /// \author Raffaele D. Facendola - August 2018
    template <typename THeadAllocator>
    class ChainAllocator<THeadAllocator> : private THeadAllocator
    {
        /// \brief Default constructor.
        ChainAllocator() noexcept = default;

        /// \brief Default copy constructor.
        ChainAllocator(const ChainAllocator&) noexcept = default;

        /// \brief Default move constructor.
        ChainAllocator(ChainAllocator&&) noexcept = default;

        /// \brief Default destructor.
        ~ChainAllocator() noexcept = default;

        /// \brief Default assignment operator.
        ChainAllocator& operator=(const ChainAllocator&) noexcept = default;

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
        void Deallocate(const MemoryRange& block) noexcept;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment) noexcept;

        /// \brief Check whether this allocator owns the provided memory block.
        /// The null allocator only contains empty ranges.
        /// \return Returns true if the provided memory range is empty, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;
    };


}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
 
namespace syntropy
{
    template <typename THeadAllocator, typename... TRestAllocators>
    inline MemoryRange ChainAllocator<THeadAllocator, TRestAllocators...>::Allocate(Bytes size) noexcept
    {
        if (auto Block = THeadAllocator::Allocate(size))
        {
            return Block;
        }
        else
        {
            return ChainAllocator<TRestAllocators...>::Allocate(size);                          // Fallback to the rest of the chain.
        }
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline MemoryRange ChainAllocator<THeadAllocator, TRestAllocators...>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto Block = THeadAllocator::Allocate(size, alignment))
        {
            return Block;
        }
        else
        {
            return ChainAllocator<TRestAllocators...>::Allocate(size, alignment);               // Fallback to the rest of the chain.
        }
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline void ChainAllocator<THeadAllocator, TRestAllocators...>::Deallocate(const MemoryRange& block)
    {
        if (THeadAllocator::Owns(block))
        {
            THeadAllocator::Deallocate(block);
        }
        else
        {
            ChainAllocator<TRestAllocators...>::Deallocate(block);
        }
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline void ChainAllocator<THeadAllocator, TRestAllocators...>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        if (THeadAllocator::Owns(block))
        {
            THeadAllocator::Deallocate(block);
        }
        else
        {
            ChainAllocator<TRestAllocators...>::Deallocate(block);
        }
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline bool ChainAllocator<THeadAllocator, TRestAllocators...>::Owns(const MemoryRange& block) const noexcept
    {
        return THeadAllocator::Owns(block) || ChainAllocator<TRestAllocators...>::Owns(block);
    }

    //

    template <typename THeadAllocator>
    inline MemoryRange ChainAllocator<THeadAllocator>::Allocate(Bytes size) noexcept
    {
        return THeadAllocator::Allocate(size);
    }

    template <typename THeadAllocator>
    inline MemoryRange ChainAllocator<THeadAllocator>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return THeadAllocator::Allocate(size, alignment);
    }

    template <typename THeadAllocator>
    inline void ChainAllocator<THeadAllocator>::Deallocate(const MemoryRange& block)
    {
        return THeadAllocator::Deallocate(block);
    }

    template <typename THeadAllocator>
    inline void ChainAllocator<THeadAllocator>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        return THeadAllocator::Deallocate(block, alignment);
    }

    template <typename THeadAllocator>
    inline bool ChainAllocator<THeadAllocator>::Owns(const MemoryRange& block) const noexcept
    {
        return THeadAllocator::Owns(block);
    }
}
