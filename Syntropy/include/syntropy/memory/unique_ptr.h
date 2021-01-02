
/// \file unique_ptr.h
/// \brief This header is part of Syntropy memory module. It contains definitions for exclusive-ownership smart pointers.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/memory/new.h"

#include "syntropy/allocators/allocator.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* UNIQUE PTR                                                           */
    /************************************************************************/

    /// \brief Represents a pointer that hold exclusive ownership of an object and deletes it when going out of scope.
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TType>
    class UniquePtr
    {
        template <typename UType>
        friend class UniquePtr;

    public:

        /// \brief Create an empty pointer.
        UniquePtr() = default;

        /// \brief Create an empty pointer.
        UniquePtr(Null rhs);

        /// \brief Unique pointer cannot be copy-constructed.
        UniquePtr(Immutable<UniquePtr> rhs) noexcept = delete;

        /// \brief Move constructor.
        template <typename UType>
        UniquePtr(Movable<UniquePtr<UType>> rhs) noexcept;

        /// \brief Create an unique pointer to an existing object.
        /// \remarks Accessing the object instance through the provided pointer after this call results in undefined behavior.
        template <typename UType>
        UniquePtr(RWPtr<UType> pointee, Mutable<BaseAllocator> allocator) noexcept;

        /// \brief Destroy the underlying object.
        ~UniquePtr() noexcept;

        /// \brief Assign a new object to the pointer, causing existing instances to be destroyed as a result of this method.
        template <typename UType>
        Mutable<UniquePtr> operator=(Movable<UniquePtr<UType>> rhs) noexcept;

        /// \brief Destroy the pointed object and reset the pointer.
        Mutable<UniquePtr> operator=(Null rhs) noexcept;

        /// \brief Reset the pointer, destroying the pointee object (if valid).
        void Reset() noexcept;

        /// \brief Release the pointed object and reset the pointer.
        [[nodiscard]] RWPtr<TType> Release() noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] RWPtr<TType> Get() const noexcept;

        /// \brief Get the allocator the pointed object was allocated by.
        /// \remarks If the pointed object is null, accessing the returned value results in undefined behavior.
        [[nodiscard]] Mutable<BaseAllocator> GetAllocator() const noexcept;

        /// \brief Check whether the pointed object is non-null.
        [[nodiscard]] explicit operator Bool() const noexcept;

        /// \brief Access the pointed object.
        /// \remarks If the pointed object is null, accessing the returned value results in undefined behavior.
        [[nodiscard]] Mutable<TType> operator*() const noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] RWPtr<TType> operator->() const noexcept;

    private:

        /// \brief Pointed object.
        RWPtr<TType> pointee_{ nullptr };

        /// \brief Allocator the pointee was allocated by. Null for empty pointers.
        RWPtr<BaseAllocator> allocator_{ nullptr };
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether two unique pointers refer to the same underlying object.
    template <typename TType, typename UType>
    Bool operator==(Immutable<UniquePtr<TType>> lhs, Immutable<UniquePtr<UType>> rhs) noexcept;

    /// \brief Check whether lhs is empty.
    template <typename TType, typename UType>
    Bool operator==(Immutable<UniquePtr<TType>> lhs, Null rhs) noexcept;

    /// \brief Check whether rhs is empty.
    template <typename TType, typename UType>
    Bool operator==(Null lhs, Immutable<UniquePtr<UType>> rhs) noexcept;

    /// \brief Compare two unique pointers.
    template <typename TType, typename UType>
    Ordering operator<=>(Immutable<UniquePtr<TType>> lhs, Immutable<UniquePtr<UType>> rhs) noexcept;
    
    /// \brief Compare an unique pointer against a null pointer.
    template <typename TType, typename UType>
    Ordering operator<=>(Immutable<UniquePtr<TType>> lhs, Null rhs) noexcept;
    
    /// \brief Compare a null pointer against an unique pointer.
    template <typename TType, typename UType>
    Ordering operator<=>(Null lhs, Immutable<UniquePtr<UType>> rhs) noexcept;


    // Utilities.
    // ==========

    /// \brief Allocate a new object on the active allocator.
    template <typename TType, typename... TArguments>
    UniquePtr<TType> MakeUnique(Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the given allocator.
    template <typename TType, typename...TArguments>
    UniquePtr<TType> MakeUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UniquePtr.
    // ==========

    template <typename TType>
    inline  UniquePtr<TType>::UniquePtr(Null rhs)
        : UniquePtr()
    {

    }

    template <typename TType>
    template <typename UType>
    inline UniquePtr<TType>::UniquePtr(Movable<UniquePtr<UType>> rhs) noexcept
        : pointee_(rhs.pointee_)
        , allocator_(rhs.allocator_)
    {
        rhs.pointee_ = nullptr;
        rhs.allocator_ = nullptr;
    }

    template <typename TType>
    template <typename UType>
    inline  UniquePtr<TType>::UniquePtr(RWPtr<UType> pointee, Mutable<BaseAllocator> allocator) noexcept
        : pointee_(pointee)
        , allocator_(PtrOf(allocator))
    {

    }

    template <typename TType>
    inline UniquePtr<TType>::~UniquePtr() noexcept
    {
        Reset();
    }

    template <typename TType>
    template <typename UType>
    inline Mutable<UniquePtr<TType>> UniquePtr<TType>::operator=(Movable<UniquePtr<UType>> rhs) noexcept
    {
        using Syntropy::Swap;

        Swap(pointee_, rhs.pointee_);
        Swap(allocator_, rhs.allocator_);

        return *this;
    }

    template <typename TType>
    inline Mutable<UniquePtr<TType>> UniquePtr<TType>::operator=(Null rhs) noexcept
    {
        Reset();

        return *this;
    }

    template <typename TType>
    void UniquePtr<TType>::Reset() noexcept
    {
        if (pointee_)
        {
            SYNTROPY_ASSERT(allocator_);

            pointee_->~TType();

            auto block = BytesOf(*pointee_);

            allocator_->Deallocate(block, AlignmentOf<TType>());

            pointee_ = nullptr;
            allocator_ = nullptr;
        }
    }

    template <typename TType>
    inline RWPtr<TType> UniquePtr<TType>::Release() noexcept
    {
        auto pointee = pointee_;

        pointee_ = nullptr;
        allocator_ = nullptr;
    }

    template <typename TType>
    inline RWPtr<TType> UniquePtr<TType>::Get() const noexcept
    {
        return pointee_;
    }

    template <typename TType>
    inline Mutable<BaseAllocator> UniquePtr<TType>::GetAllocator() const noexcept
    {
        SYNTROPY_ASSERT(allocator_);

        return *allocator_;
    }

    template <typename TType>
    inline UniquePtr<TType>::operator Bool() const noexcept
    {
        return !!pointee_;
    }

    template <typename TType>
    inline Mutable<TType> UniquePtr<TType>::operator*() const noexcept
    {
        SYNTROPY_ASSERT(pointee_);

        return *pointee_;
    }

    template <typename TType>
    inline RWPtr<TType> UniquePtr<TType>::operator->() const noexcept
    {
        return pointee_;
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename TType, typename UType>
    inline Bool operator==(Immutable<UniquePtr<TType>> lhs, Immutable<UniquePtr<UType>> rhs) noexcept
    {
        return lhs.Get() == rhs.Get();
    }

    template <typename TType, typename UType>
    inline Bool operator==(Immutable<UniquePtr<TType>> lhs, Null rhs) noexcept
    {
        return ToBool(lhs);
    }

    template <typename TType, typename UType>
    inline Bool operator==(Null lhs, Immutable<UniquePtr<UType>> rhs) noexcept
    {
        return ToBool(rhs);
    }

    template <typename TType, typename UType>
    inline Ordering operator<=>(Immutable<UniquePtr<TType>> lhs, Immutable<UniquePtr<UType>> rhs) noexcept
    {
        return (lhs.Get() <=> rhs.Get());
    }

    template <typename TType, typename UType>
    inline Ordering operator<=>(Immutable<UniquePtr<TType>> lhs, Null rhs) noexcept
    {
        return (lhs.Get() <=> nullptr);
    }

    template <typename TType, typename UType>
    inline Ordering operator<=>(Null lhs, Immutable<UniquePtr<UType>> rhs) noexcept
    {
        return (nullptr <=> rhs.Get());
    }


    // Utilities.
    // ==========

    template <typename TType, typename... TArguments>
    inline UniquePtr<TType> MakeUnique(Forwarding<TArguments>... arguments) noexcept
    {
        return MakeUniqueOnAllocator<TType>(GetAllocator(), Forward<TArguments>(arguments)...);
    }

    template <typename TType, typename...TArguments>
    UniquePtr<TType> MakeUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept
    {
        auto block = allocator.Allocate(SizeOf<TType>(), AlignmentOf<TType>());

        SYNTROPY_ASSERT(Count(block) == SizeOf<TType>());

        auto pointee = new (block) TType(Forward<TArguments>(arguments)...);

        return { pointee, allocator };
    }

}

// ===========================================================================
