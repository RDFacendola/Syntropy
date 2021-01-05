
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

#include "syntropy/allocators/allocator.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE UNIQUE PTR                                                      */
    /************************************************************************/

    /// \brief Represents a pointer that hold exclusive ownership of an object and deletes it when going out of scope.
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
        /// \remarks Accessing the object instance through the provided pointer after this call results in undefined behavior.
        /// \remarks The caller is responsible to fill in the allocation size and allocator, otherwise the behavior of this method is undefined.
        template <typename UType>
        BaseUniquePtr(Immutable<RWPtr<UType>> pointee, Immutable<Bytes> size, Mutable<BaseAllocator> allocator) noexcept;

        /// \brief Destroy the underlying object.
        ~BaseUniquePtr() noexcept;

        /// \brief Assign a new object to the pointer, causing existing instances to be destroyed as a result of this method.
        template <typename UType, typename UTraits>
        Mutable<BaseUniquePtr> operator=(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

        /// \brief Destroy the pointed object and reset the pointer.
        Mutable<BaseUniquePtr> operator=(Null rhs) noexcept;

        /// \brief Check whether the pointed object is non-null.
        [[nodiscard]] explicit operator Bool() const noexcept;

        /// \brief Access the pointed object.
        /// \remarks If the pointed object is null, accessing the returned value results in undefined behavior.
        [[nodiscard]] TReference operator*() const noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] TPointer operator->() const noexcept;

        /// \brief Reset the pointer, destroying the pointee object (if valid).
        void Reset() noexcept;

        /// \brief Transfer the ownership of the pointee to the caller.
        [[nodiscard]] TPointer Release() noexcept;

        /// \brief Access the pointed object.
        [[nodiscard]] TPointer Get() const noexcept;

        /// \brief Get the size of the pointed object, in Bytes.
        [[nodiscard]] Bytes GetSize() const noexcept;

        /// \brief Get the allocator the pointed object was allocated on.
        /// \remarks If the pointed object is null, accessing the returned value results in undefined behavior.
        [[nodiscard]] Mutable<BaseAllocator> GetAllocator() const noexcept;

    private:

        /// \brief Pointed object.
        /// The underlying pointer has to be read-write in order for it to be destroyed.
        RWPtr<TType> pointee_{ nullptr };

        /// \brief Size of the pointed object.
        Bytes size_;

        /// \brief Allocator the pointee was allocated by. Null for empty pointers.
        RWPtr<BaseAllocator> allocator_{ nullptr };
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

    /// \brief Check whether two unique pointers refer to the same underlying object.
    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]]  Bool operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    /// \brief Check whether lhs is empty.
    template <typename TType, typename TTraits>
    [[nodiscard]]  Bool operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Null rhs) noexcept;

    /// \brief Check whether rhs is empty.
    template <typename UType, typename UTraits>
    [[nodiscard]]  Bool operator==(Null lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    /// \brief Compare two unique pointers.
    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]]  Ordering operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;
    
    /// \brief Compare an unique pointer against a null pointer.
    template <typename TType, typename TTraits>
    [[nodiscard]]  Ordering operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Null rhs) noexcept;
    
    /// \brief Compare a null pointer against an unique pointer.
    template <typename UType, typename UTraits>
    [[nodiscard]]  Ordering operator<=>(Null lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only unique pointer.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr UniquePtr<TType> ToReadOnly(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-only unique pointer.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr UniquePtr<TType> ToReadOnly(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write unique pointer.
    /// \remarks If the original unique pointer is not read-writable, accessing the returned values results in undefined behavior.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType> ToReadWrite(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write unique pointer.
    /// \remarks If the original unique pointer is not read-writable, accessing the returned values results in undefined behavior.
    /// \remarks To preserve uniqueness, rhs is moved from.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType> ToReadWrite(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Allocate a new object on the active allocator.
    template <typename TType, typename... TArguments>
    [[nodiscard]]  UniquePtr<TType> MakeUnique(Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the given allocator.
    template <typename TType, typename...TArguments>
    [[nodiscard]]  UniquePtr<TType> MakeUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept;
    
    /// \brief Allocate a new object on the active allocator.
    template <typename TType, typename... TArguments>
    [[nodiscard]]  RWUniquePtr<TType> MakeRWUnique(Forwarding<TArguments>... arguments) noexcept;

    /// \brief Allocate a new object on the given allocator.
    template <typename TType, typename...TArguments>
    [[nodiscard]]  RWUniquePtr<TType> MakeRWUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BaseUniquePtr.
    // ==========

    template <typename TType, typename TTraits>
    inline  BaseUniquePtr<TType, TTraits>::BaseUniquePtr(Null rhs)
        : BaseUniquePtr()
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    inline BaseUniquePtr<TType, TTraits>::BaseUniquePtr(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
        : pointee_(rhs.Get())
        , size_(rhs.size_)
        , allocator_(rhs.allocator_)
    {
        using namespace Syntropy::Memory::Literals;

        rhs.pointee_ = nullptr;
        rhs.size_ = 0_Bytes;
        rhs.allocator_ = nullptr;
    }

    template <typename TType, typename TTraits>
    template <typename UType>
    inline BaseUniquePtr<TType, TTraits>::BaseUniquePtr(Immutable<RWPtr<UType>> pointee, Immutable<Bytes> size, Mutable<BaseAllocator> allocator) noexcept
         : pointee_(pointee)
         , size_(size)
         , allocator_(PtrOf(allocator))
    {

    }

    template <typename TType, typename TTraits>
    inline BaseUniquePtr<TType, TTraits>::~BaseUniquePtr() noexcept
    {
        Reset();
    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    inline Mutable<BaseUniquePtr<TType, TTraits>> BaseUniquePtr<TType, TTraits>::operator=(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        using namespace Syntropy::Memory::Literals;

        Reset();

        pointee_ = rhs.pointee_;
        size_ = rhs.size_;
        allocator_ = rhs.allocator_;

        rhs.pointee_ = nullptr;
        rhs.size_ = 0_Bytes;
        rhs.allocator_ = nullptr;

        return *this;
    }

    template <typename TType, typename TTraits>
    inline Mutable<BaseUniquePtr<TType, TTraits>> BaseUniquePtr<TType, TTraits>::operator=(Null rhs) noexcept
    {
        Reset();

        return *this;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline BaseUniquePtr<TType, TTraits>::operator Bool() const noexcept
    {
        return !!pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TReference BaseUniquePtr<TType, TTraits>::operator*() const noexcept
    {
        SYNTROPY_ASSERT(pointee_);

        return *pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TPointer BaseUniquePtr<TType, TTraits>::operator->() const noexcept
    {
        return pointee_;
    }

    template <typename TType, typename TTraits>
    void BaseUniquePtr<TType, TTraits>::Reset() noexcept
    {
        using namespace Syntropy::Memory::Literals;

        if (pointee_)
        {
            SYNTROPY_ASSERT(allocator_);

            pointee_->~TType();

            auto block = MakeByteSpan(ToBytePtr(pointee_), size_);

            allocator_->Deallocate(block, AlignmentOf<TType>());

            pointee_ = nullptr;
            size_ = 0_Bytes;
            allocator_ = nullptr;
        }
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TPointer BaseUniquePtr<TType, TTraits>::Release() noexcept
    {
        using namespace Syntropy::Memory::Literals;

        auto pointee = pointee_;

        pointee_ = nullptr;
        size_ = 0_Bytes;
        allocator_ = nullptr;

        return pointee;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]]  inline typename BaseUniquePtr<TType, TTraits>::TPointer BaseUniquePtr<TType, TTraits>::Get() const noexcept
    {
        return pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]]  inline Bytes BaseUniquePtr<TType, TTraits>::GetSize() const noexcept
    {
        return size_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]]  inline Mutable<BaseAllocator> BaseUniquePtr<TType, TTraits>::GetAllocator() const noexcept
    {
        SYNTROPY_ASSERT(allocator_);

        return *allocator_;
    }

    // Non-member functions.
    // =====================

    // Comparison.

    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] inline Bool operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return lhs.Get() == rhs.Get();
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Bool operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Null rhs) noexcept
    {
        return ToBool(lhs);
    }

    template <typename UType, typename UTraits>
    [[nodiscard]] inline Bool operator==(Null lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return ToBool(rhs);
    }

    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] inline Ordering operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return (lhs.Get() <=> rhs.Get());
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Ordering operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs, Null rhs) noexcept
    {
        return (lhs.Get() <=> nullptr);
    }

    template <typename UType, typename UTraits>
    inline Ordering operator<=>(Null lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return (nullptr <=> rhs.Get());
    }

    // Access.

    template <typename TType, typename TTraits>
    [[nodiscard]] inline constexpr UniquePtr<TType> ToReadOnly(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        if (rhs)
        {
            auto allocator = PtrOf(rhs.GetAllocator());
            auto size = rhs.GetSize();
            auto pointee = rhs.Release();

            return { Syntropy::ToReadOnly(pointee), size, *allocator };
        }

        return {};
    }
    
    template <typename TType, typename TTraits>
    [[nodiscard]] inline constexpr UniquePtr<TType> ToReadOnly(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        return ToReadOnly(rhs);
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType> ToReadWrite(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        if (rhs)
        {
            auto allocator = PtrOf(rhs.GetAllocator());
            auto size = rhs.GetSize();
            auto pointee = rhs.Release();

            return { Syntropy::ToReadWrite(pointee), size, *allocator };
        }

        return {};
    }
    
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType> ToReadWrite(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        return ToReadWrite(rhs);
    }

    // Utilities.
    
    template <typename TType, typename... TArguments>
    [[nodiscard]] inline UniquePtr<TType> MakeUnique(Forwarding<TArguments>... arguments) noexcept
    {
        return MakeUniqueOnAllocator<TType>(GetAllocator(), Forward<TArguments>(arguments)...);
    }

    template <typename TType, typename...TArguments>
    [[nodiscard]] inline UniquePtr<TType> MakeUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept
    {
        auto block = allocator.Allocate(SizeOf<TType>(), AlignmentOf<TType>());

        auto pointee = new (Data(block)) TType(Forward<TArguments>(arguments)...);

        return { pointee, SizeOf<TType>(), allocator };
    }

    template <typename TType, typename... TArguments>
    [[nodiscard]] inline RWUniquePtr<TType> MakeRWUnique(Forwarding<TArguments>... arguments) noexcept
    {
        return ToReadWrite(MakeUniqueOnAllocator<TType>(GetAllocator(), Forward<TArguments>(arguments)...));
    }

    template <typename TType, typename...TArguments>
    [[nodiscard]] inline RWUniquePtr<TType> MakeRWUniqueOnAllocator(Mutable<BaseAllocator> allocator, Forwarding<TArguments>... arguments) noexcept
    {
        return ToReadWrite(MakeUniqueOnAllocator<TType>(allocator, Forward<TArguments>(arguments)...));
    }

}

// ===========================================================================
