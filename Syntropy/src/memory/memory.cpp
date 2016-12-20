#include "memory/memory.h"

#include <cstdlib>
#include <cstring>

#include "platform/os.h"

#include "syntropy.h"

void* operator new(std::size_t size)
{
    return std::malloc(size);
}

void operator delete(void* pointer) noexcept 
{
    std::free(pointer);
}

namespace syntropy
{

    //////////////// MEMORY ADDRESS POOL ////////////////

    MemoryAddressPool::MemoryAddressPool(size_t count)
        : memory_range_(GetMemory().ReserveVirtualRange(count * GetPointerSize()))
        , size_(0)
    {
        GetMemory().AllocMemoryBlock(memory_range_, 0, memory_range_.GetCount());
    }

    void MemoryAddressPool::Push(void* address)
    {
        std::memcpy(memory_range_.GetAddress(size_ * GetPointerSize()),
                    &address,
                    GetPointerSize());

        ++size_;
    }

    void* MemoryAddressPool::Pop()
    {
        void* address;
        
        std::memcpy(&address, 
                    memory_range_.GetAddress(--size_ * GetPointerSize()),
                    GetPointerSize());

        return address;
    }

    bool MemoryAddressPool::IsEmpty() const
    {
        return size_ == 0;
    }

    //////////////// MEMORY ////////////////
    
    Memory& Memory::GetInstance()
    {
        return platform::PlatformMemory::GetInstance();
    }

    Memory& GetMemory()
    {
        return Memory::GetInstance();
    }

    constexpr size_t GetPointerSize()
    {
        return sizeof(void*);
    }

}