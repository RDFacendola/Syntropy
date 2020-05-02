
/// \file fallback_memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains memory resources that attempt to allocate on a primary memory resource and fallback to a second one on failure.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/utility.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* FALLBACK MEMORY RESOURCE                                             */
    /************************************************************************/

    /// \brief Tier Omega memory resource that attempts to perform allocation on the primary memory resource and falls back to the other one upon failure.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource, typename TFallbackResource>
    class FallbackMemoryResource
    {
    public:

        /// \brief Default constructor.
        /// This method requires that each underlying memory resource is default-constructible.
        FallbackMemoryResource() noexcept = default;

        /// \brief Default copy constructor.
        /// This method requires that each underlying memory resource is copyable.
        FallbackMemoryResource(const FallbackMemoryResource&) noexcept = default;

        /// \brief Default move constructor.
        /// This method requires that each underlying memory resource is movable.
        FallbackMemoryResource(FallbackMemoryResource&&) noexcept = default;

        /// \brief Create a new memory resource by initializing the memory resource and the fallback memory resource explicitly.
        template <typename TArguments, typename TFallbackArguments>
        FallbackMemoryResource(TArguments&& arguments, TFallbackArguments&& fallback_arguments);

        /// \brief Create a new memory resource by initializing the memory resource explicitly and default-constructing the fallback one.
         template <typename TArguments>
         FallbackMemoryResource(TArguments&& arguments, DefaultConstructT);

         /// \brief Create a new memory resource by initializing the fallback memory resource explicitly and default-constructing the primary one.
         template <typename TFallbackArguments>
         FallbackMemoryResource(DefaultConstructT, TFallbackArguments&& fallback_arguments);

        /// \brief Default destructor.
        ~FallbackMemoryResource() noexcept = default;

        /// \brief Default assignment operator.
        /// This method requires that each underlying memory resource is assignable.
        FallbackMemoryResource& operator=(const FallbackMemoryResource&) noexcept = default;

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

    private:

        ///< \brief Primary memory resource the requests will be attempted on.
        TMemoryResource memory_resource_;

        ///< \brief Fallback memory resource to use upon failure.
        TFallbackResource fallback_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FallbackMemoryResource<TMemoryResource, TFallbackResource>.

    template <typename TMemoryResource, typename TFallbackResource>
    template <typename TArguments, typename TFallbackArguments>
    inline FallbackMemoryResource<TMemoryResource, TFallbackResource>::FallbackMemoryResource(TArguments&& arguments, TFallbackArguments&& fallback_arguments)
        : memory_resource_(std::make_from_tuple<TMemoryResource>(std::forward<TArguments>(arguments)))
        , fallback_resource_(std::make_from_tuple<TFallbackResource>(std::forward<TFallbackArguments>(fallback_arguments)))
    {

    }
    template <typename TMemoryResource, typename TFallbackResource>
    template <typename TArguments>
    inline FallbackMemoryResource<TMemoryResource, TFallbackResource>::FallbackMemoryResource(TArguments&& arguments, DefaultConstructT)
        : memory_resource_(std::make_from_tuple<TMemoryResource>(std::forward<TArguments>(arguments)))
    {

    }

    template <typename TMemoryResource, typename TFallbackResource>
    template <typename TFallbackArguments>
    inline FallbackMemoryResource<TMemoryResource, TFallbackResource>::FallbackMemoryResource(DefaultConstructT, TFallbackArguments&& fallback_arguments)
        : fallback_resource_(std::make_from_tuple<TFallbackResource>(std::forward<TFallbackArguments>(fallback_arguments)))
    {

    }

    template <typename TMemoryResource, typename TFallbackResource>
    inline MemoryRange FallbackMemoryResource<TMemoryResource, TFallbackResource>::Allocate(Bytes size) noexcept
    {
        if (auto block = memory_resource_.Allocate(size))
        {
            return block;
        }

        return fallback_resource_.Allocate(size);
    }

    template <typename TMemoryResource, typename TFallbackResource>
    inline MemoryRange FallbackMemoryResource<TMemoryResource, TFallbackResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto block = memory_resource_.Allocate(size, alignment))
        {
            return block;
        }

        return fallback_resource_.Allocate(size, alignment);
    }

    template <typename TMemoryResource, typename TFallbackResource>
    inline void FallbackMemoryResource<TMemoryResource, TFallbackResource>::Deallocate(const MemoryRange& block)
    {
        SYNTROPY_ASSERT(Owns(block));

        if (memory_resource_.Owns(block))
        {
            memory_resource_.Deallocate(block);

            return;
        }

        fallback_resource_.Deallocate(block);
    }

    template <typename TMemoryResource, typename TFallbackResource>
    inline void FallbackMemoryResource<TMemoryResource, TFallbackResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        SYNTROPY_ASSERT(Owns(block));

        if (memory_resource_.Owns(block))
        {
            memory_resource_.Deallocate(block, alignment);

            return;
        }

        fallback_resource_.Deallocate(block, alignment);
    }

    template <typename TMemoryResource, typename TFallbackResource>
    inline bool FallbackMemoryResource<TMemoryResource, TFallbackResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block) || fallback_resource_.Owns(block);
    }

}