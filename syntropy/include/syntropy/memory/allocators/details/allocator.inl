
/// \file allocator.h
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE ALLOCATOR                                                       */
    /************************************************************************/

    [[nodiscard]] inline Mutable<RWPtr<BaseAllocator>>
    BaseAllocator::GetAllocator() noexcept
    {
        static thread_local RWPtr<BaseAllocator> default_allocator_
            = PtrOf(GetSystemAllocator());

        return default_allocator_;
    }

    /************************************************************************/
    /* POLYMORPHIC ALLOCATOR                                                */
    /************************************************************************/

    template <Templates::Allocator TAllocator>
    template <typename... TArguments>
    inline PolymorphicAllocator<TAllocator>
    ::PolymorphicAllocator(Forwarding<TArguments>... arguments) noexcept
        : allocator_(Forward<TArguments>(arguments)...)
    {

    }

    template <Templates::Allocator TAllocator>
    [[nodiscard]] inline RWByteSpan PolymorphicAllocator<TAllocator>
    ::Allocate(Bytes size, Alignment alignment) noexcept
    {
        return allocator_.Allocate(size, alignment);
    }

    template <Templates::Allocator TAllocator>
    inline void PolymorphicAllocator<TAllocator>
    ::Deallocate(Immutable<RWByteSpan> block, Alignment alignment) noexcept
    {
        allocator_.Deallocate(block, alignment);
    }

    template <Templates::Allocator TAllocator>
    [[nodiscard]] inline Mutable<TAllocator> PolymorphicAllocator<TAllocator>
    ::GetAllocator()
    {
        return allocator_;
    }

    template <Templates::Allocator TAllocator>
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
    GetScopeAllocator() noexcept
    {
        return *BaseAllocator::GetAllocator();
    }

    inline Mutable<BaseAllocator>
    SetAllocator(Mutable<BaseAllocator> allocator) noexcept
    {
        auto& scope_allocator = GetScopeAllocator();

        BaseAllocator::GetAllocator() = PtrOf(allocator);

        return scope_allocator;
    }

}

// ===========================================================================
