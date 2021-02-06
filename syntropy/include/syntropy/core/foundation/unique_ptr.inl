
/// \file unique_ptr.inl
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE UNIQUE PTR                                                      */
    /************************************************************************/

    template <typename TType, typename TTraits>
    inline  BaseUniquePtr<TType, TTraits>
    ::BaseUniquePtr(Null rhs)
        : BaseUniquePtr()
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    inline BaseUniquePtr<TType, TTraits>
    ::BaseUniquePtr(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
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
    inline BaseUniquePtr<TType, TTraits>
    ::BaseUniquePtr(Immutable<RWPtr<UType>> pointee,
                    Immutable<Memory::Bytes> size,
                    Mutable<Memory::BaseAllocator> allocator) noexcept
         : pointee_(pointee)
         , size_(size)
         , allocator_(PtrOf(allocator))
    {

    }

    template <typename TType, typename TTraits>
    inline BaseUniquePtr<TType, TTraits>
    ::~BaseUniquePtr() noexcept
    {
        Reset();
    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    inline Mutable<BaseUniquePtr<TType, TTraits>> BaseUniquePtr<TType, TTraits>
    ::operator=(Movable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
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
    inline Mutable<BaseUniquePtr<TType, TTraits>> BaseUniquePtr<TType, TTraits>

    ::operator=(Null rhs) noexcept
    {
        Reset();

        return *this;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline BaseUniquePtr<TType, TTraits>
    ::operator Bool() const noexcept
    {
        return !!pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TReference
    BaseUniquePtr<TType, TTraits>
    ::operator*() const noexcept
    {
        SYNTROPY_ASSERT(pointee_);

        return *pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TPointer
    BaseUniquePtr<TType, TTraits>
    ::operator->() const noexcept
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

            auto block = MakeByteSpan(Memory::ToBytePtr(pointee_), size_);

            allocator_->Deallocate(block, Memory::AlignmentOf<TType>());

            pointee_ = nullptr;
            size_ = 0_Bytes;
            allocator_ = nullptr;
        }
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TPointer
    BaseUniquePtr<TType, TTraits>
    ::Release() noexcept
    {
        using namespace Syntropy::Memory::Literals;

        auto pointee = pointee_;

        pointee_ = nullptr;
        size_ = 0_Bytes;
        allocator_ = nullptr;

        return pointee;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline typename BaseUniquePtr<TType, TTraits>::TPointer
    BaseUniquePtr<TType, TTraits>
    ::Get() const noexcept
    {
        return pointee_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Memory::Bytes
    BaseUniquePtr<TType, TTraits>
    ::GetSize() const noexcept
    {
        return size_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Mutable<Memory::BaseAllocator>
    BaseUniquePtr<TType, TTraits>
    ::GetAllocator() const noexcept
    {
        SYNTROPY_ASSERT(allocator_);

        return *allocator_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] inline Bool
    operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
               Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return lhs.Get() == rhs.Get();
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Bool
    operator==(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
               Null rhs) noexcept
    {
        return ToBool(lhs);
    }

    template <typename UType, typename UTraits>
    [[nodiscard]] inline Bool
    operator==(Null lhs, Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return ToBool(rhs);
    }

    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] inline Ordering
    operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
                Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return (lhs.Get() <=> rhs.Get());
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline Ordering
    operator<=>(Immutable<BaseUniquePtr<TType, TTraits>> lhs,
                Null rhs) noexcept
    {
        return (lhs.Get() <=> nullptr);
    }

    template <typename UType, typename UTraits>
    inline Ordering
    operator<=>(Null lhs,
                Immutable<BaseUniquePtr<UType, UTraits>> rhs) noexcept
    {
        return (nullptr <=> rhs.Get());
    }

    // Access.
    // =======

    template <typename TType, typename TTraits>
    [[nodiscard]] inline constexpr UniquePtr<TType>
    ToReadOnly(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        if (rhs)
        {
            auto allocator = PtrOf(rhs.GetAllocator());
            auto size = rhs.GetSize();
            auto pointee = rhs.Release();

            return { ToReadOnly(pointee), size, *allocator };
        }

        return {};
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] inline constexpr UniquePtr<TType>
    ToReadOnly(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        return ToReadOnly(rhs);
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType>
    ToReadWrite(Mutable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        if (rhs)
        {
            auto allocator = PtrOf(rhs.GetAllocator());
            auto size = rhs.GetSize();
            auto pointee = rhs.Release();

            return { ToReadWrite(pointee), size, *allocator };
        }

        return {};
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWUniquePtr<TType>
    ToReadWrite(Movable<BaseUniquePtr<TType, TTraits>> rhs) noexcept
    {
        return ToReadWrite(rhs);
    }

    // Utilities.
    // ==========

    template <typename TType, typename... TArguments>
    [[nodiscard]] inline UniquePtr<TType>
    MakeUnique(Forwarding<TArguments>... arguments) noexcept
    {
        return MakeUniqueOnAllocator<TType>(
            Memory::GetAllocator(),
            Forward<TArguments>(arguments)...);
    }

    template <typename TType, typename...TArguments>
    [[nodiscard]] inline UniquePtr<TType>
    MakeUniqueOnAllocator(Mutable<Memory::BaseAllocator> allocator,
                          Forwarding<TArguments>... arguments) noexcept
    {
        auto block = allocator.Allocate(Memory::SizeOf<TType>(),
                                        Memory::AlignmentOf<TType>());

        auto pointee
            = new (Data(block)) TType(Forward<TArguments>(arguments)...);

        return { pointee, Memory::SizeOf<TType>(), allocator };
    }

    template <typename TType, typename... TArguments>
    [[nodiscard]] inline RWUniquePtr<TType>
    MakeRWUnique(Forwarding<TArguments>... arguments) noexcept
    {
        return ToReadWrite(
            MakeUniqueOnAllocator<TType>(Memory::GetAllocator(),
                                         Forward<TArguments>(arguments)...));
    }

    template <typename TType, typename...TArguments>
    [[nodiscard]] inline RWUniquePtr<TType>
    MakeRWUniqueOnAllocator(Mutable<Memory::BaseAllocator> allocator,
                            Forwarding<TArguments>... arguments) noexcept
    {
        return ToReadWrite(
            MakeUniqueOnAllocator<TType>(allocator,
                                         Forward<TArguments>(arguments)...));
    }

}

// ===========================================================================
