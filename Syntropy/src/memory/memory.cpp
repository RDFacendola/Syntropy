#include "memory/memory.h"

#include <cstdlib>

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

    //////////////// MEMORY ADDRESS STACK ////////////////

    MemoryAddressStack::MemoryAddressStack(size_t count)
        : memory_range_(GetMemory().ReserveVirtualRange(count))
        , size_(0)
    {
        auto block = GetMemory().AllocMemoryBlock(memory_range_, 0, memory_range_.GetCount());
        SYNTROPY_UNUSED(block);
    }

    void MemoryAddressStack::Push(void* address)
    {
        *memory_range_.GetPointer<uint64_t>(size_) = reinterpret_cast<uint64_t>(address);
        ++size_;
    }

    void* MemoryAddressStack::Pop()
    {
        return reinterpret_cast<void*>(*memory_range_.GetPointer<uint64_t>(--size_));
    }

    bool MemoryAddressStack::IsEmpty() const
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


}