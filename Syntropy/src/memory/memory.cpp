#include "memory/memory.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "platform/os.h"
#include "diagnostics/log.h"

#include "syntropy.h"

namespace syntropy
{

    const diagnostics::Context MemoryCtx("SyntropyMemory");

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    size_t Memory::CeilToPageSize(size_t size)
    {
        return Math::Ceil(size, GetPageSize());
    }

    size_t Memory::GetPageSize()
    {
        auto page_size = platform::PlatformMemory::GetPageSize();

        SYNTROPY_ASSERT(Math::IsPow2(page_size));

        return page_size;
    }

    void* Memory::Reserve(size_t size)
    {
        auto address = platform::PlatformMemory::Reserve(size);

        SYNTROPY_ASSERT(IsAlignedTo(address, GetPageSize()));

        return address;
    }

    void* Memory::Allocate(size_t size)
    {
        return platform::PlatformMemory::Allocate(size);
    }

    bool Memory::Release(void* address)
    {
        auto success = platform::PlatformMemory::Release(address);

        SYNTROPY_ASSERT(success);

        return success;
    }

    bool Memory::Commit(void* address, size_t size)
    {
        return platform::PlatformMemory::Commit(address, size);
    }

    bool Memory::Decommit(void* address, size_t size)
    {
        return platform::PlatformMemory::Decommit(address, size);
    }
    
    /************************************************************************/
    /* MEMORY DEBUG                                                         */
    /************************************************************************/

    void MemoryDebug::MarkUninitialized(void* begin, void* end)
    {
        SYNTROPY_DEBUG_ONLY(
            std::fill(
                reinterpret_cast<int8_t*>(begin),
                reinterpret_cast<int8_t*>(end),
                kUninitializedMemoryPattern);
        );
    }

    void MemoryDebug::MarkFree(void* begin, void* end)
    {
        SYNTROPY_DEBUG_ONLY(
            std::fill(
                reinterpret_cast<int8_t*>(begin),
                reinterpret_cast<int8_t*>(end),
                kFreeMemoryPattern);
        );
    }

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    Allocator::Allocator(const HashedString& name)
        : name_(name)
        , context_(MemoryCtx | name_)
    {

    }

    const HashedString& Allocator::GetName() const
    {
        return name_;
    }

    Allocator::operator diagnostics::Context() const
    {
        return context_;
    }

    /************************************************************************/
    /* MEMORY RANGE                                                         */
    /************************************************************************/

    MemoryRange::MemoryRange()
        : base_(nullptr)
        , top_(nullptr)
    {

    }

    MemoryRange::MemoryRange(void* base, void* top)
        : base_(base)
        , top_(top)
    {
        SYNTROPY_ASSERT(reinterpret_cast<uintptr_t>(base) <= reinterpret_cast<uintptr_t>(top));
    }

    MemoryRange::MemoryRange(void* base, size_t size)
        : MemoryRange(base, Memory::AddOffset(base, size))
    {

    }

    MemoryRange::MemoryRange(const MemoryRange& other, size_t alignment)
        : MemoryRange(Memory::Align(other.base_, alignment), other.top_)
    {
        SYNTROPY_ASSERT(other.Contains(*this));
    }

    void* MemoryRange::operator*() const
    {
        return base_;
    }

    void* MemoryRange::operator[](size_t offset) const
    {
        auto ptr = Memory::AddOffset(base_, offset);

        SYNTROPY_ASSERT(Contains(ptr));

        return ptr;
    }

    void* MemoryRange::GetTop() const
    {
        return top_;
    }

    size_t MemoryRange::GetSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(base_, top_));
    }

    bool MemoryRange::Contains(const MemoryRange& memory_range) const
    {
        return Memory::GetDistance(base_, memory_range.base_) >= 0 &&
            Memory::GetDistance(top_, memory_range.top_) <= 0;
    }

    bool MemoryRange::Contains(void* address) const
    {
        return Memory::GetDistance(base_, address) >= 0 &&
            Memory::GetDistance(top_, address) < 0;
    }

    /************************************************************************/
    /* MEMORY POOL                                                          */
    /************************************************************************/

    MemoryPool::MemoryPool()
        : pool_(nullptr)
    {

    }

    MemoryPool::MemoryPool(size_t size)
        : pool_(Memory::Reserve(size))
        , range_(pool_, size)
    {

    }

    MemoryPool::MemoryPool(size_t size, size_t alignment)
        : pool_(Memory::Reserve(size + alignment - 1))
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
            Memory::Release(pool_);
        }
    }

    void* MemoryPool::operator*() const
    {
        return *range_;
    }

    void* MemoryPool::operator[](size_t offset) const
    {
        return range_[offset];
    }

    size_t MemoryPool::GetSize() const
    {
        return range_.GetSize();
    }

    MemoryPool::operator MemoryRange() const
    {
        return range_;
    }

    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    MemoryBuffer::MemoryBuffer()
        : allocator_(nullptr)
    {

    }

    MemoryBuffer::MemoryBuffer(size_t size, Allocator& allocator)
        : range_(SYNTROPY_ALLOC(allocator, size), size)
        , allocator_(std::addressof(allocator))
    {

    }

    MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : MemoryBuffer(other.GetSize(), *other.allocator_)
    {
        std::memmove(*range_, *other.range_, other.GetSize());      // Copy the buffer's content.
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : range_(other.range_)
        , allocator_(other.allocator_)
    {
        other.range_ = MemoryRange();
        other.allocator_ = nullptr;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        if (*range_)
        {
            SYNTROPY_FREE(*allocator_, *range_);
        }
    }

    MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    void* MemoryBuffer::operator*() const
    {
        return *range_;
    }

    void* MemoryBuffer::operator[](size_t offset) const
    {
        return range_[offset];
    }

    size_t MemoryBuffer::GetSize() const
    {
        return range_.GetSize();
    }

    MemoryBuffer::operator MemoryRange() const
    {
        return range_;
    }

    void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(range_, other.range_);
        std::swap(allocator_, other.allocator_);
    }

}

/************************************************************************/
/* NEW \ DELETE                                                         */
/************************************************************************/

void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    auto ptr = allocator.Allocate(size);

    SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. Address: ", ptr, ". Caller: ", stack_trace);

    return ptr;
}

void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    allocator.Free(ptr);

    SYNTROPY_LOG((allocator), "Deallocating memory. Address: ", ptr, ". Caller: ", stack_trace);
}

namespace std
{
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second)
    {
        first.Swap(second);
    }
}
