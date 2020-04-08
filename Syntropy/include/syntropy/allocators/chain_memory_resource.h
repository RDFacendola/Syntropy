
/// \file chain_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains memory resources that defer actual allocation to other memory resources according to different policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits.h>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/allocators/null_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* CHAIN MEMORY RESOURCE                                                */
    /************************************************************************/

    /// \brief Basic memory resource that attempts to perform allocation on the primary memory resource and falls back to others upon failure.
    /// This definition is used to break the chain after each other memory resource failed.
    /// \author Raffaele D. Facendola - August 2018
    template <typename...>
    class ChainMemoryResource : public NullMemoryResource
    {
    public:

        /// \brief Default constructor.
        ChainMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        ChainMemoryResource(const ChainMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        ChainMemoryResource(ChainMemoryResource&&) noexcept = default;

        /// \brief Default destructor.
        ~ChainMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        ChainMemoryResource& operator=(const ChainMemoryResource&) noexcept = default;

    };

    /// \brief Basic memory resource that attempts to perform allocation on the primary memory resource and falls back to others upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource, typename... TMemoryResources>
    class ChainMemoryResource<TMemoryResource, TMemoryResources...>
    {
    public:

        /// \brief Default constructor.
        /// This method requires that each underlying memory resource is default-constructible.
        ChainMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        /// This method requires that each underlying memory resource is copyable.
        ChainMemoryResource(const ChainMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        /// This method requires that each underlying memory resource is movable.
        ChainMemoryResource(ChainMemoryResource&&) noexcept = default;

        /// \brief Create a new chain memory resource by initializing each sub-memory resource explicitly.
        /// This overload only participates in overload resolution if the provided arguments may not be used to perform copy or move construction of this memory resource.
        template <typename TArgument, typename... TArguments, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TArgument>, ChainMemoryResource>>>
        ChainMemoryResource(TArgument&& argument, TArguments&&... arguments);

        /// \brief Default destructor.
        ~ChainMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        /// This method requires that each underlying memory resource is assignable.
        ChainMemoryResource& operator=(const ChainMemoryResource&) noexcept = default;

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

        /// \brief Get the maximum allocation size that can be handled by this memory resource.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this memory resource.
        Bytes GetMaxAllocationSize() const noexcept;

    private:

        ///< \brief Primary memory resource the requests will be attempted on.
        TMemoryResource memory_resource_;

        ///< \brief Fallback memory resources to use upon failure.
        ChainMemoryResource<TMemoryResources...> memory_resources_;

    };

    /// \brief Create a new ChainMemoryResource by chaining together different memory resources.
    /// \param memory_resources List of memory resources to chain together.
    /// \return Returns the ChainMemoryResource resulting from chaining together memory resources in the provided order.
    template <typename... TMemoryResources>
    ChainMemoryResource<TMemoryResources...> MakeChainMemoryResource(TMemoryResources&&... memory_resources);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ChainMemoryResource<TMemoryResource, TMemoryResources>.

    template <typename TMemoryResource, typename... TMemoryResources>
    template <typename TArgument, typename... TArguments, typename>
    ChainMemoryResource<TMemoryResource, TMemoryResources...>::ChainMemoryResource(TArgument&& argument, TArguments&&... arguments)
        : memory_resource_(std::forward<TArgument>(argument))
        , memory_resources_(std::forward<TArguments>(arguments)...)
    {

    }


    template <typename TMemoryResource, typename... TMemoryResources>
    inline MemoryRange ChainMemoryResource<TMemoryResource, TMemoryResources...>::Allocate(Bytes size) noexcept
    {
        if (auto block = memory_resource_.Allocate(size))
        {
            return block;
        }
        
        return memory_resources_.Allocate(size);
    }

    template <typename TMemoryResource, typename... TMemoryResources>
    inline MemoryRange ChainMemoryResource<TMemoryResource, TMemoryResources...>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = memory_resource_.Allocate(size, alignment))
        {
            return block;
        }

        return memory_resources_.Allocate(size, alignment);
    }

    template <typename TMemoryResource, typename... TMemoryResources>
    inline void ChainMemoryResource<TMemoryResource, TMemoryResources...>::Deallocate(const MemoryRange& block)
    {
        if (memory_resource_.Owns(block))
        {
            memory_resource_.Deallocate(block);
        }

        memory_resources_.Deallocate(block);
    }

    template <typename TMemoryResource, typename... TMemoryResources>
    inline void ChainMemoryResource<TMemoryResource, TMemoryResources...>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        if (memory_resource_.Owns(block))
        {
            memory_resource_.Deallocate(block, alignment);
        }

        memory_resources_.Deallocate(block, alignment);
    }

    template <typename TMemoryResource, typename... TMemoryResources>
    inline bool ChainMemoryResource<TMemoryResource, TMemoryResources...>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block) || memory_resources_.Owns(block);
    }

    template <typename TMemoryResource, typename... TMemoryResources>
    inline Bytes ChainMemoryResource<TMemoryResource, TMemoryResources...>::GetMaxAllocationSize() const noexcept
    {
        using std::max;

        return max(memory_resource_.GetMaxAllocationSize(), memory_resources_.GetMaxAllocationSize());
    }

    template <typename... TMemoryResources>
    inline ChainMemoryResource<TMemoryResources...> MakeChainMemoryResource(TMemoryResources&&... memory_resources)
    {
        return ChainMemoryResource<TMemoryResources...>(std::forward<TMemoryResources>(memory_resources)...);
    }

}