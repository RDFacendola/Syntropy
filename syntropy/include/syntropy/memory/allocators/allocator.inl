
/// \file allocator.h
///
/// \author Raffaele D. Facendola - 2020

#pragma once

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE ALLOCATOR                                                       */
    /************************************************************************/

    [[nodiscard]] inline Mutable<RWPtr<BaseAllocator>>
    BaseAllocator::GetScopeAllocator() noexcept
    {
        static thread_local RWPtr<BaseAllocator> default_allocator_
            = PtrOf(GetSystemAllocator());

        return default_allocator_;
    }

    /************************************************************************/
    /* POLYMORPHIC ALLOCATOR                                                */
    /************************************************************************/

    template <Concepts::Allocator TAllocator>
    template <typename... TArguments>
    inline PolymorphicAllocator<TAllocator>
    ::PolymorphicAllocator(Forwarding<TArguments>... arguments) noexcept
        : allocator_(Forward<TArguments>(arguments)...)
    {

    }

    template <Concepts::Allocator TAllocator>
    [[nodiscard]] inline RWByteSpan PolymorphicAllocator<TAllocator>
    ::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return allocator_.Allocate(size, alignment);
    }

    template <Concepts::Allocator TAllocator>
    inline void PolymorphicAllocator<TAllocator>
    ::Deallocate(Immutable<RWByteSpan> block, Alignment alignment) noexcept
    {
        allocator_.Deallocate(block, alignment);
    }

    template <Concepts::Allocator TAllocator>
    [[nodiscard]] inline Mutable<TAllocator> PolymorphicAllocator<TAllocator>
    ::GetAllocator()
    {
        return allocator_;
    }

    template <Concepts::Allocator TAllocator>
    inline Immutable<TAllocator> PolymorphicAllocator<TAllocator>
    ::GetAllocator() const
    {
        return allocator_;
    }

    // Non-member functions.
    // =====================

    [[nodiscard]] inline Mutable<BaseAllocator>
    GetSystemAllocator() noexcept
    {
        static auto system_allocator = PolymorphicAllocator<SystemAllocator>{};

        return system_allocator;
    }

    [[nodiscard]] inline Mutable<BaseAllocator>
    GetAllocator() noexcept
    {
        return *BaseAllocator::GetScopeAllocator();
    }

    inline Mutable<BaseAllocator>
    SetAllocator(Mutable<BaseAllocator> allocator) noexcept
    {
        auto& previous_allocator = GetAllocator();

        BaseAllocator::GetScopeAllocator() = PtrOf(allocator);

        return previous_allocator;
    }

}

// ===========================================================================
