
// ################################################################################

// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <stdlib.h>

// ################################################################################

module syntropy.allocator;

import syntropy.types;

// ################################################################################

// ================================================================================
// ALLOCATOR
// ================================================================================

sy::RWAddress<sy::Allocator>& GetThreadAllocatorSingleton()
{
    static thread_local auto allocator_ = sy::AddressOf<sy::Allocator>(sy::GetSystemAllocator());

    return allocator_;
}

sy::Allocator& sy::GetThreadAllocator()
{
    return *GetThreadAllocatorSingleton();
}

void sy::SetThreadAllocator(Allocator& allocator)
{
    GetThreadAllocatorSingleton() = sy::AddressOf(allocator);
}

// ================================================================================
// SYSTEM ALLOCATOR
// ================================================================================

sy::RWByteSpan sy::SystemAllocator::Allocate(Bytes size)
{
    if (auto address = malloc(static_cast<size_t>(size)))
    {
        return { RWAddress<Byte>{ address }, ToInt(size) };
    }

    return {};
}

void sy::SystemAllocator::Deallocate(const RWByteSpan& block)
{
    auto address = AddressOf(block.GetFront());

    free(address);
}

sy::SystemAllocator& sy::GetSystemAllocator()
{
    static auto system_allocator = SystemAllocator{};

    return system_allocator;
}

// ================================================================================
// ALLOCATOR GUARD
// ================================================================================

sy::AllocatorGuard::AllocatorGuard(Allocator& allocator)
    : _allocator{ GetThreadAllocator() }
{
    SetThreadAllocator(allocator);
}

sy::AllocatorGuard::~AllocatorGuard()
{
    SetThreadAllocator(_allocator);
}

// ################################################################################
