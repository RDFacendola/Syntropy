
/// \file chain_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains allocators that defer actual allocation to other allocators according to different policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits.h>

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

#include "memory/allocators/null_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* CHAIN ALLOCATOR                                                      */
    /************************************************************************/

    /// \brief Basic allocator that attempts to perform allocation on the primary allocator and falls back to other allocators upon failure.
    /// This definition is used to break the chain after each other allocator failed.
    /// \author Raffaele D. Facendola - August 2018
    template <typename...>
    class ChainAllocator : public NullAllocator
    {
    public:

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
    };

    /// \brief Basic allocator that attempts to perform allocation on the primary allocator and falls back to other allocators upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename THeadAllocator, typename... TRestAllocators>
    class ChainAllocator<THeadAllocator, TRestAllocators...>
    {
    public:

        /// \brief Default constructor.
        /// This method requires that each underlying allocator is default-constructible.
        ChainAllocator() noexcept = default;

        /// \brief Default copy constructor.
        /// This method requires that each underlying allocator is copyable.
        ChainAllocator(const ChainAllocator&) noexcept = default;

        /// \brief Default move constructor.
        /// This method requires that each underlying allocator is movable.
        ChainAllocator(ChainAllocator&&) noexcept = default;

        /// \brief Create a new chain allocator by initializing each sub-allocator explicitly.
        /// This overload only participates in overload resolution if the provided arguments may not be used to perform copy or move construction of this allocator.
        template <typename THeadArgument, typename... TRestArguments, typename = std::enable_if_t<!std::is_same_v<std::decay_t<THeadArgument>, ChainAllocator>>>
        ChainAllocator(THeadArgument&& head_argument, TRestArguments&&... rest_arguments);

        /// \brief Default destructor.
        ~ChainAllocator() noexcept = default;

        /// \brief Default assignment operator.
        /// This method requires that each underlying allocator is assignable.
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
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        THeadAllocator head_allocator_;                             ///< \brief Primary allocator the requests will be attempted on.

        ChainAllocator<TRestAllocators...> rest_allocators_;        ///< \brief Fallback allocators to use upon failure of the primary allocator.
    };

    /// \brief Create a new ChainAllocator by chaining together different sub-allocators.
    /// \param allocators List of allocators to chain together.
    /// \return Returns the ChainAllocator resulting from chaining together allocators in the provided order.
    template <typename... TAllocators>
    ChainAllocator<TAllocators...> MakeChainAllocator(TAllocators&&... allocators);

}

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/
 
namespace syntropy
{
    template <typename THeadAllocator, typename... TRestAllocators>
    template <typename THeadArgument, typename... TRestArguments, typename>
    ChainAllocator<THeadAllocator, TRestAllocators...>::ChainAllocator(THeadArgument&& head_argument, TRestArguments&&... rest_arguments)
        : head_allocator_(std::forward<THeadArgument>(head_argument))
        , rest_allocators_(std::forward<TRestArguments>(rest_arguments)...)
    {

    }


    template <typename THeadAllocator, typename... TRestAllocators>
    inline MemoryRange ChainAllocator<THeadAllocator, TRestAllocators...>::Allocate(Bytes size) noexcept
    {
        if (auto Block = head_allocator_.Allocate(size))
        {
            return Block;
        }
        
        return rest_allocators_.Allocate(size);
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline MemoryRange ChainAllocator<THeadAllocator, TRestAllocators...>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto Block = head_allocator_.Allocate(size, alignment))
        {
            return Block;
        }

        return rest_allocators_.Allocate(size, alignment);
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline void ChainAllocator<THeadAllocator, TRestAllocators...>::Deallocate(const MemoryRange& block)
    {
        if (head_allocator_.Owns(block))
        {
            head_allocator_.Deallocate(block);
        }

        rest_allocators_.Deallocate(block);
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline void ChainAllocator<THeadAllocator, TRestAllocators...>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        if (head_allocator_.Owns(block))
        {
            head_allocator_.Deallocate(block, alignment);
        }

        rest_allocators_.Deallocate(block, alignment);
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline bool ChainAllocator<THeadAllocator, TRestAllocators...>::Owns(const MemoryRange& block) const noexcept
    {
        return head_allocator_.Owns(block) || rest_allocators_.Owns(block);
    }

    template <typename THeadAllocator, typename... TRestAllocators>
    inline Bytes ChainAllocator<THeadAllocator, TRestAllocators...>::GetMaxAllocationSize() const noexcept
    {
        using std::max;

        return max(head_allocator_.GetMaxAllocationSize(), rest_allocators_.GetMaxAllocationSize());
    }

    template <typename... TAllocators>
    inline ChainAllocator<TAllocators...> MakeChainAllocator(TAllocators&&... allocators)
    {
        return ChainAllocator<TAllocators...>(std::forward<TAllocators>(allocators)...);
    }
}
