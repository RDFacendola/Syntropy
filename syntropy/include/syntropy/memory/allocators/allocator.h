
/// \file allocator.h
///
/// \brief This header is part of the Syntropy allocators module.
///        It contains base interfaces and definitions for allocators.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/memory/foundation/byte_span.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"

#include "syntropy/memory/allocators/system_allocator.h"

// ===========================================================================

namespace Syntropy::Concepts
{
    /************************************************************************/
    /* ALLOCATOR CONCEPTS                                                   */
    /************************************************************************/

    /// \brief Concept for allocators.
    template <typename TAllocator>
    concept Allocator
        = requires(Mutable<TAllocator> allocator,
                   Memory::Bytes size,
                   Memory::Alignment alignment)
          {
              // Allocating a memory block.
              {  allocator.Allocate(size, alignment) }
                -> SameAs<Memory::RWByteSpan>;
          }
        && requires(Mutable<TAllocator> allocator,
                    Immutable<Memory::RWByteSpan> block,
                    Memory::Alignment alignment)
          {
              // Deallocate a memory block.
              allocator.Deallocate(block, alignment);
          };
}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    class BaseAllocator;

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Get the system allocator, an allocator that uses the global
    ///        operator new and operator delete to allocate and
    ///        deallocate memory.
    [[nodiscard]] Mutable<BaseAllocator>
    GetSystemAllocator() noexcept;

    /// \brief Get the active thread-local allocator.
    ///
    /// \remarks The active allocator is used when an explicit allocator
    ///          cannot be supplied.
    [[nodiscard]] Mutable<BaseAllocator>
    GetScopeAllocator() noexcept;

    /// \brief Set the active thread-local allocator.
    /// \return Returns the previous allocator.
    ///
    /// \remarks The active allocator is used when an explicit allocator
    ///          cannot be supplied.
    Mutable<BaseAllocator>
    SetAllocator(Mutable<BaseAllocator> allocator) noexcept;

    /************************************************************************/
    /* BASE ALLOCATOR                                                       */
    /************************************************************************/

    /// \brief Represents an abstract interface for allocators.
    /// \author Raffaele D. Facendola - April 2020.
    class BaseAllocator
    {
        friend Mutable<BaseAllocator>
        GetSystemAllocator() noexcept;

        friend Mutable<BaseAllocator>
        GetScopeAllocator() noexcept;

        friend Mutable<BaseAllocator>
        SetAllocator(Mutable<BaseAllocator>) noexcept;

    public:

        /// \brief Default constructor.
        BaseAllocator() noexcept = default;

        /// \brief No copy constructor.
        BaseAllocator(Immutable<BaseAllocator>) noexcept = delete;

        /// \brief Default move constructor.
        BaseAllocator(Movable<BaseAllocator>) noexcept = default;

        /// \brief Default virtual destructor.
        virtual ~BaseAllocator() = default;

        /// \brief Default assignment operator.
        Mutable<BaseAllocator>
        operator=(Immutable<BaseAllocator>) noexcept = default;

        /// \brief Allocate a memory block.
        /// If a memory block could not be allocated, returns an empty block.
        [[nodiscard]] virtual RWByteSpan
        Allocate(Bytes size, Alignment alignment) noexcept = 0;

        /// \brief Deallocate a memory block.
        /// \remarks The behavior of this function is undefined unless the
        ///          provided block was returned by a previous call to
        ///          ::Allocate(size, alignment).
        virtual void
        Deallocate(Immutable<RWByteSpan> block,
                   Alignment alignment) noexcept = 0;

    private:

        /// \brief Get the active allocator in the current scope.
        [[nodiscard]] static Mutable<RWPtr<BaseAllocator>>
        GetAllocator() noexcept;

    };

    /************************************************************************/
    /* POLYMORPHIC ALLOCATOR                                                */
    /************************************************************************/

    /// \brief Represents a polymorphic allocator used to type-erase the
    ///        concrete type of an underlying allocator.
    /// \author Raffaele D. Facendola - April 2020
    template <Concepts::Allocator TAllocator>
    class PolymorphicAllocator : public BaseAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \params arguments Arguments used to construct the underlying
        ///         allocator.
        template <typename... TArguments>
        PolymorphicAllocator(Forwarding<TArguments>... arguments) noexcept;

        /// \brief Default virtual destructor.
        virtual ~PolymorphicAllocator() = default;

        [[nodiscard]] virtual RWByteSpan
        Allocate(Bytes size, Alignment alignment) noexcept override;

        virtual void
        Deallocate(Immutable<RWByteSpan> block,
                   Alignment alignment) noexcept override;

        /// \brief Access the underlying allocator.
        [[nodiscard]] Mutable<TAllocator>
        GetAllocator();

        /// \brief Access the underlying allocator.
        [[nodiscard]] Immutable<TAllocator>
        GetAllocator() const;

    private:

        /// \brief Underlying allocator.
        TAllocator allocator_;

    };
}

// ===========================================================================

#include "allocator.inl"

// ===========================================================================
