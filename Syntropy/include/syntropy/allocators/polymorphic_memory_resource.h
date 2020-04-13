
/// \file polymorphic_memory_resource.h
/// \brief This header is part of the syntropy memory management system. It contains polymorphic memory resources.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <type_traits>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* POLYMORPHIC MEMORY RESOURCE                                          */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying, type-erased, memory resource.
    /// \author Raffaele D. Facendola - August 2018
    class PolymorphicMemoryResource
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~PolymorphicMemoryResource() = default;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        virtual MemoryRange Allocate(Bytes size) noexcept = 0;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        virtual MemoryRange Allocate(Bytes size, Alignment alignment) noexcept = 0;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        virtual void Deallocate(const MemoryRange& block) = 0;

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        virtual void Deallocate(const MemoryRange& block, Alignment alignment) = 0;

        /// \brief Check whether this memory resource owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this memory resource, returns false otherwise.
        virtual bool Owns(const MemoryRange& block) const noexcept = 0;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        virtual Bytes GetMaxAllocationSize() const noexcept = 0;

    };

    /************************************************************************/
    /* POLYMORPHIC MEMORY RESOURCE T <TMEMORY RESOURCE>                     */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying memory resource.
    /// \author Raffaele D. Facendola - August 2018
    template <typename TMemoryResource>
    class PolymorphicMemoryResourceT : public PolymorphicMemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        template <typename... TArguments>
        PolymorphicMemoryResourceT(TArguments&&... arguments) noexcept;

        /// \brief Default destructor.
        virtual ~PolymorphicMemoryResourceT() = default;

        virtual MemoryRange Allocate(Bytes size) noexcept override;

        virtual MemoryRange Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void Deallocate(const MemoryRange& block) override;

        virtual void Deallocate(const MemoryRange& block, Alignment alignment) override;

        virtual bool Owns(const MemoryRange& block) const noexcept override;

        virtual Bytes GetMaxAllocationSize() const noexcept override;

    private:

        /// \brief Underlying memory resource.
        TMemoryResource memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PolymorphicMemoryResourceT<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline PolymorphicMemoryResourceT<TMemoryResource>::PolymorphicMemoryResourceT(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange PolymorphicMemoryResourceT<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        return memory_resource_.Allocate(size);
    }

    template <typename TMemoryResource>
    inline MemoryRange PolymorphicMemoryResourceT<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return memory_resource_.Allocate(size, alignment);
    }

    template <typename TMemoryResource>
    inline void PolymorphicMemoryResourceT<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        memory_resource_.Deallocate(block);
    }

    template <typename TMemoryResource>
    inline void PolymorphicMemoryResourceT<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        memory_resource_.Deallocate(block, alignment);
    }

    template <typename TMemoryResource>
    inline bool PolymorphicMemoryResourceT<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline Bytes PolymorphicMemoryResourceT<TMemoryResource>::GetMaxAllocationSize() const noexcept
    {
        return memory_resource_.GetMaxAllocationSize();
    }

}

