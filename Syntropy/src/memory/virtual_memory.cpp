#include "memory/virtual_memory.h"

#include "platform/os/os.h"

#include "math/math.h"

#include "diagnostics/diagnostics.h"

namespace syntropy
{

    /************************************************************************/
    /* VIRTUAL MEMORY                                                       */
    /************************************************************************/

    Bytes VirtualMemory::GetPageSize()
    {
        return platform::PlatformMemory::GetPageSize();
    }

    void* VirtualMemory::Reserve(Bytes size)
    {
        return platform::PlatformMemory::Reserve(size);
    }

    void* VirtualMemory::Allocate(Bytes size)
    {
        return platform::PlatformMemory::Allocate(size);
    }

    bool VirtualMemory::Release(void* address)
    {
        return platform::PlatformMemory::Release(address);
    }

    bool VirtualMemory::Commit(void* address, Bytes size)
    {
        return platform::PlatformMemory::Commit(address, size);
    }

    bool VirtualMemory::Decommit(void* address, Bytes size)
    {
        return platform::PlatformMemory::Decommit(address, size);
    }

    Bytes VirtualMemory::CeilToPageSize(Bytes size)
    {
        return Math::Ceil(size, GetPageSize());
    }

    /************************************************************************/
    /* MEMORY POOL                                                          */
    /************************************************************************/

    MemoryPool::MemoryPool()
        : pool_(nullptr)
    {

    }

    MemoryPool::MemoryPool(Bytes size)
        : pool_(VirtualMemory::Reserve(size))
        , range_(pool_, size)
    {

    }

    MemoryPool::MemoryPool(Bytes size, Bytes alignment)
        : pool_(VirtualMemory::Reserve(size + alignment - 1_Bytes))
        , range_(Memory::Align(pool_, alignment), size)       // Align the buffer to the required boundary
    {

    }

    MemoryPool::MemoryPool(MemoryPool&& other)
        : pool_(other.pool_)
        , range_(other.range_)
    {
        other.pool_ = nullptr;
        other.range_ = MemoryRange();
    }

    MemoryPool::~MemoryPool()
    {
        if (pool_)
        {
            VirtualMemory::Release(pool_);
        }
    }

    void* MemoryPool::operator*() const
    {
        return *range_;
    }

    void* MemoryPool::operator[](Bytes offset) const
    {
        return range_[offset];
    }

    Bytes MemoryPool::GetSize() const
    {
        return range_.GetSize();
    }

    MemoryPool::operator MemoryRange() const
    {
        return range_;
    }

}

