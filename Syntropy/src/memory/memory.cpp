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
        : memory_range_(GetMemory().ReserveVirtualRange(ToMemorySize(count)))
        , size_(0)
        , capacity_(ToAddressCount(GetMemory().GetPageSize()))
    {
        // Allocate the first memory block
        GetMemory().AllocMemoryBlock(memory_range_, 0, ToMemorySize(capacity_));
    }

    void MemoryAddressPool::Push(void* address)
    {
        if (++size_ > capacity_)
        {
            // Double the capacity
            auto block = GetMemory().AllocMemoryBlock(memory_range_, 
                                                      ToMemorySize(capacity_), 
                                                      ToMemorySize(capacity_));

            capacity_ += ToAddressCount(block.GetCount());
        }

        std::memcpy(memory_range_.GetAddress(ToMemorySize(size_ - 1)),
                    &address,
                    GetPointerSize());
    }

    void* MemoryAddressPool::Pop()
    {
        void* address;
        
        std::memcpy(&address, 
                    memory_range_.GetAddress(ToMemorySize(--size_)),
                    GetPointerSize());

        if (size_ <= capacity_ / 4 &&
            capacity_ > ToAddressCount(GetMemory().GetPageSize()))
        {
            // Halve the capacity
            auto block = GetMemory().FreeMemoryBlock(memory_range_,
                                                     ToMemorySize(capacity_) / 2,
                                                     ToMemorySize(capacity_) / 2);
            
            capacity_ -= ToAddressCount(block.GetCount());
        }

        return address;
    }

    bool MemoryAddressPool::IsEmpty() const
    {
        return size_ == 0;
    }

    size_t MemoryAddressPool::ToMemorySize(size_t count) const
    {
        return count * GetPointerSize();
    }

    size_t MemoryAddressPool::ToAddressCount(size_t size) const
    {
        return size / GetPointerSize();
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