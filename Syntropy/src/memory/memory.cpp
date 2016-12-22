#include "memory/memory.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

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
        : storage_(GetMemory().Reserve(ToMemorySize(count)))
        , size_(0)
        , capacity_(ToAddressCount(GetMemory().GetAllocationGranularity()))
    {
        // Allocate the first memory block
        GetMemory().Allocate(StorageOffset(0), 
                             ToMemorySize(capacity_));
    }

    void MemoryAddressPool::Push(void* address)
    {
        if (++size_ > capacity_)
        {
            // Double the capacity
            GetMemory().Allocate(StorageOffset(capacity_), 
                                 ToMemorySize(capacity_));

            capacity_ *= 2;
        }

        std::memcpy(StorageOffset(size_ - 1),
                    &address,
                    GetPointerSize());
    }

    void* MemoryAddressPool::Pop()
    {
        void* address;
        
        std::memcpy(&address, 
                    StorageOffset(--size_),
                    GetPointerSize());

        if (size_ <= capacity_ / 4 &&
            capacity_ > ToAddressCount(GetMemory().GetAllocationGranularity()))
        {
            // Halve the capacity
            capacity_ /= 2;

            GetMemory().Deallocate(StorageOffset(capacity_),
                                   ToMemorySize(capacity_));
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

    void* MemoryAddressPool::StorageOffset(size_t offset) const
    {
        return reinterpret_cast<int8_t*>(storage_) + ToMemorySize(offset);
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