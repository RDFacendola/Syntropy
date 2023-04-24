
// ################################################################################

// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <new>

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

sy::RWByteSpan sy::SystemAllocator::Allocate(Bytes size, Alignment alignment)
{
    auto api_size = static_cast<size_t>(size);
    auto api_alignment = static_cast<std::align_val_t>(alignment);

    if (auto api_block = ::operator new(api_size, api_alignment, std::nothrow))
    {
        return { RWAddress<Byte>{ api_block }, ToInt(size) };
    }

    return {};
}

void sy::SystemAllocator::Deallocate(const RWByteSpan& block, Alignment alignment)
{
    auto api_address = AddressOf(block.GetFront());
    auto api_alignment = static_cast<std::align_val_t>(alignment);

    ::operator delete(api_address, api_alignment, std::nothrow);
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
