
/// \file memory_resource.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions and interfaces for memory resource.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/syntropy.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"

#include "syntropy/allocators/heap_memory_resource.h"

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    class MemoryResource;

    /// \brief Get a memory resource that uses the global operator new and operator delete to allocate memory.
    MemoryResource& GetNewDeleteMemoryResource() noexcept;

    /// \brief Get the thread-local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    MemoryResource& GetDefaultMemoryResource() noexcept;

    /// \brief Set the thread-local default memory resource.
    /// \return Returns the previous value of the local default memory resource.
    /// \remarks The local default memory resource is used by certain facilities when an explicit memory resource is not supplied.
    MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept;

    /************************************************************************/
    /* MEMORY RESOURCE                                                      */
    /************************************************************************/

    /// \brief Represents an abstract interface to an unbounded set of classes encapsulating memory resources.
    /// This class mimics and replaces standard's std::memory_resource. Standard's global default memory resoures are deemed
    /// to be harmful since it may cause non-coherent allocation behavior in the same scope.
    /// \author Raffaele D. Facendola - April 2020.
    class MemoryResource
    {
        friend MemoryResource& GetNewDeleteMemoryResource() noexcept;
        friend MemoryResource& GetDefaultMemoryResource() noexcept;
        friend MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept;

    public:

        /// \brief Default virtual destructor.
        virtual ~MemoryResource() = default;

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

    private:

        /// \brief Thread-local default memory resource.
        static inline thread_local MemoryResource* default_memory_resource_ = &GetNewDeleteMemoryResource();

    };

    /************************************************************************/
    /* MEMORY RESOURCE T <TMEMORY RESOURCE>                                 */
    /************************************************************************/

    /// \brief Tier Omega memory resource used to forward calls to an underlying, type-erased, memory resource.
    /// \author Raffaele D. Facendola - April 2020
    template <typename TMemoryResource>
    class MemoryResourceT : public MemoryResource
    {
    public:

        /// \brief Create a new memory resource.
        template <typename... TArguments>
        MemoryResourceT(TArguments&&... arguments) noexcept;

        /// \brief Default destructor.
        virtual ~MemoryResourceT() = default;

        virtual MemoryRange Allocate(Bytes size) noexcept override;

        virtual MemoryRange Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void Deallocate(const MemoryRange& block) override;

        virtual void Deallocate(const MemoryRange& block, Alignment alignment) override;

        virtual bool Owns(const MemoryRange& block) const noexcept override;

        /// \brief Get the underlying memory resource.
        TMemoryResource& GetMemoryResource();

        /// \brief Get the underlying memory resource.
        const TMemoryResource& GetMemoryResource() const;

    private:

        /// \brief Underlying memory resource.
        TMemoryResource memory_resource_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    inline MemoryResource& GetNewDeleteMemoryResource() noexcept
    {
        static auto new_delete_resource = MemoryResourceT<HeapMemoryResource>{};

        return new_delete_resource;
    }

    inline MemoryResource& GetDefaultMemoryResource() noexcept
    {
        return *MemoryResource::default_memory_resource_;
    }

    inline MemoryResource& SetDefaultMemoryResource(MemoryResource& memory_resource) noexcept
    {
        auto& previous_memory_resource = GetDefaultMemoryResource();

        MemoryResource::default_memory_resource_ = &memory_resource;

        return previous_memory_resource;
    }

    // MemoryResourceT<TMemoryResource>.

    template <typename TMemoryResource>
    template <typename... TArguments>
    inline MemoryResourceT<TMemoryResource>::MemoryResourceT(TArguments&&... arguments) noexcept
        : memory_resource_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TMemoryResource>
    inline MemoryRange MemoryResourceT<TMemoryResource>::Allocate(Bytes size) noexcept
    {
        return memory_resource_.Allocate(size);
    }

    template <typename TMemoryResource>
    inline MemoryRange MemoryResourceT<TMemoryResource>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return memory_resource_.Allocate(size, alignment);
    }

    template <typename TMemoryResource>
    inline void MemoryResourceT<TMemoryResource>::Deallocate(const MemoryRange& block)
    {
        memory_resource_.Deallocate(block);
    }

    template <typename TMemoryResource>
    inline void MemoryResourceT<TMemoryResource>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        memory_resource_.Deallocate(block, alignment);
    }

    template <typename TMemoryResource>
    inline bool MemoryResourceT<TMemoryResource>::Owns(const MemoryRange& block) const noexcept
    {
        return memory_resource_.Owns(block);
    }

    template <typename TMemoryResource>
    inline TMemoryResource& MemoryResourceT<TMemoryResource>::GetMemoryResource()
    {
        return memory_resource_;
    }

    template <typename TMemoryResource>
    inline const TMemoryResource& MemoryResourceT<TMemoryResource>::GetMemoryResource() const
    {
        return memory_resource_;
    }

}
