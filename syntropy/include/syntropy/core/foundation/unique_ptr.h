
/// \file unique_ptr.h
///
/// \brief This header is part of Syntropy core module.
/// It contains definitions for exclusive-ownership smart pointers.
///
/// \author Raffaele D. Facendola - August 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"
#include "syntropy/core/comparisons/compare.h"

#include "syntropy/diagnostics/foundation/assert.h"

#include "syntropy/memory/allocators/allocator.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE UNIQUE PTR                                                      */
    /************************************************************************/

    /// \brief Represents a pointer that hold exclusive ownership of an
    ///        object and deletes it when going out of scope.
    ///
    /// \author Raffaele D. Facendola - December 2020.
    template <typename TType, typename TTraits>
    class BaseUniquePtr
    {
        template <typename UType, typename UTraits>
        friend class BaseUniquePtr;

    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Create an empty pointer.
        BaseUniquePtr() = default;

        /// \brief Create an empty pointer.
        BaseUniquePtr(Null rhs);

        /// \brief No copy-constructor: ownership of the pointee is exclusive.
        BaseUniquePtr(Immutable<BaseUniquePtr> rhs) noexcept = delete;

        /// \brief Move constructor.
        template <typename UType, typename UTraits>
        BaseUniquePtr(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

        /// \brief Acquire an object's ownership.
        ///
        /// \remarks Accessing the object instance through the provided pointer
        ///          after this call results in undefined behavior.
        /// \remarks The caller is responsible to fill in the allocation size
        //           and allocator, otherwise the behavior of this method is
        ///          undefined.
        template <typename UType>
        BaseUniquePtr(Immutable<RWPtr<UType>> pointee,
                      Immutable<Memory::Bytes> size,
                      Mutable<Memory::BaseAllocator> allocator) noexcept;

        /// \brief Destroy the underlying object.
        ~BaseUniquePtr() noexcept;

        /// \brief Assign a new object to the pointer, causing existing
        ///        instances to be destroyed as a result of this method.
        template <typename UType, typename UTraits>
        Mutable<BaseUniquePtr>
        operator=(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

        /// \brief Destroy the pointed object and reset the pointer.
        Mutable<BaseUniquePtr>
        operator=(Null rhs) noexcept;

        /// \brief Check whether the pointed object is non-null.
        [[nodiscard]] explicit
        operator Bool() const noexcept;

        /// \brief Access the pointed object.
        /// \remarks If the pointed object is null, accessing the returned
        ///          value results in undefined behavior.
        [[nodiscard]] TReference
        operator*() const noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] TPointer
        operator->() const noexcept;

        /// \brief Reset the pointer, destroying the pointee object (if valid).
        void Reset() noexcept;

        /// \brief Transfer the ownership of the pointee to the caller.
        [[nodiscard]] TPointer
        Release() noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] TPointer
        Get() const noexcept;

        /// \brief Get the size of the pointed object, in Bytes.
        [[nodiscard]] Memory::Bytes
        GetSize() const noexcept;

        /// \brief Get the allocator the pointed object was allocated on.
        /// \remarks If the pointed object is null, accessing the returned
        ///          value results in undefined behavior.
        [[nodiscard]] Mutable<Memory::BaseAllocator>
        GetAllocator() const noexcept;

    private:

        /// \brief Pointed object.
        /// The underlying pointer has to be read-write in order for it to
        /// be destroyed.
        RWPtr<TType> pointee_{ nullptr };

        /// \brief Size of the pointed object.
        Memory::Bytes size_;

        /// \brief Allocator the pointee was allocated by. Null for
        ///        empty pointers.
        RWPtr<Memory::BaseAllocator> allocator_{ nullptr };
    };

    /************************************************************************/
    /* UNIQUE PTR                                                           */
    /************************************************************************/

    /// \brief Tag for read-only unique pointers.
    template <typename TType>
    struct UniquePtrTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = Ptr<TType>;

        /// \brief Reference type.
        using TReference = Immutable<TType>;
    };

    /// \brief Represents an owning pointer to a read-only value.
    template <typename TType>
    using UniquePtr = BaseUniquePtr<TType, UniquePtrTypeTraits<TType>>;

    /************************************************************************/
    /* RW UNIQUE PTR                                                        */
    /************************************************************************/

    /// \brief Tag for read-write unique pointers.
    template <typename TType>
    struct RWUniquePtrTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = RWPtr<TType>;

        /// \brief Reference type.
        using TReference = Mutable<TType>;
    };

    /// \brief Represents an owning pointer to a read-write value.
    template <typename TType>
    using RWUniquePtr = BaseUniquePtr<TType, RWUniquePtrTypeTraits<TType>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether two unique pointers refer to the same
    ///        underlying object.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] Bool
    operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
               Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    /// \brief Check whether lhs is empty.
    template <typename TType, typename TTraits>
    [[nodiscard]] Bool
    operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
               Null rhs) noexcept;

    /// \brief Check whether rhs is empty.
    template <typename UType, typename UTraits>
    [[nodiscard]] Bool
    operator==(Null lhs,
               Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    /// \brief Compare two unique pointers.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] Ordering
    operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
                Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    /// \brief Compare an unique pointer against a null pointer.
    template <typename TType, typename TTraits>
    [[nodiscard]] Ordering
    operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
                Null rhs) noexcept;

    /// \brief Compare a null pointer against an unique pointer.
    template <typename UType, typename UTraits>
    [[nodiscard]] Ordering
    operator<=>(Null lhs,
                Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only unique pointer.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr UniquePtr<TType>
    ToReadOnly(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-only unique pointer.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr UniquePtr<TType>
    ToReadOnly(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write unique pointer.
    /// \remarks If the original unique pointer is not read-writable,
    ///          accessing the returned values results in undefined behavior.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType>
    ToReadWrite(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write unique pointer.
    /// \remarks If the original unique pointer is not read-writable,
    ///          accessing the returned values results in undefined behavior.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType>
    ToReadWrite(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Allocate a new object on the active allocator.
    template <typename TType, typename... TArguments>
    [[nodiscard]]  UniquePtr<TType>
    MakeUnique(Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the given allocator.
    template <typename TType, typename...TArguments>
    [[nodiscard]]  UniquePtr<TType>
    MakeUniqueOnAllocator(Mutable<Memory::BaseAllocator> allocator,
                          Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the active allocator.
    template <typename TType, typename... TArguments>
    [[nodiscard]]  RWUniquePtr<TType>
    MakeRWUnique(Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the given allocator.
    template <typename TType, typename...TArguments>
    [[nodiscard]]  RWUniquePtr<TType>
    MakeRWUniqueOnAllocator(Mutable<Memory::BaseAllocator> allocator,
                            Forwarding<TArguments>... arguments) noexcept;

}

// ===========================================================================

#include "details/unique_ptr.inl"

// ===========================================================================
