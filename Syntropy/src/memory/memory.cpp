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
        return platform::PlatformMemory::GetPageSize();
    }

    void* Memory::Allocate(size_t size)
    {
        return platform::PlatformMemory::Allocate(size);
    }

    bool Memory::Release(void* address)
    {
        auto page_size = GetPageSize();

        auto reserved_block = GetReservedBlockHeader(address, page_size);

        SYNTROPY_ASSERT(address == reserved_block->block_address_);                             // The provided address must be a block address returned by Reserve()

        auto result = platform::PlatformMemory::Release(reserved_block->base_address_);

        SYNTROPY_ASSERT(result);

        return result;
    }

    void* Memory::Reserve(size_t size, size_t alignment)
    {
        auto page_size = GetPageSize();

        auto extended_size = size + alignment + page_size;                                      // [PADDING|RESERVED BLOCK HEADER||PAGE PADDING|RESERVED BLOCK]

        auto base_address = platform::PlatformMemory::Reserve(extended_size);

        // The header must be allocated in a different page to prevent client code from accidentally deallocating it while working with the memory block

        auto block_address = Memory::Align(Memory::Offset(base_address, page_size),             // Requires some space before the reserved block for the header
                                           alignment);                                          // The resulting block must be aligned

        // Fill the block header

        auto reserved_block = GetReservedBlockHeader(block_address, page_size);

        Memory::Commit(reserved_block, page_size);

        reserved_block->base_address_ = base_address;
        reserved_block->block_address_ = block_address;
        reserved_block->size_ = size;
        reserved_block->extended_size_ = extended_size;
        reserved_block->alignment_ = alignment;

        return block_address;
    }

    bool Memory::Commit(void* address, size_t size)
    {
        return platform::PlatformMemory::Commit(address, size);
    }

    bool Memory::Decommit(void* address, size_t size)
    {
        return platform::PlatformMemory::Decommit(address, size);
    }

    Memory::ReservedBlockHeader* Memory::GetReservedBlockHeader(void* address, size_t page_size)
    {
        address = Memory::AlignDown(address, page_size);                                                                // Page containing the base address of the block to free.

        return reinterpret_cast<ReservedBlockHeader*>(Memory::Offset(address, -static_cast<int64_t>(page_size)));       // The previous page contains the infos of the reserved region. This page is always committed.
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

    MemoryRange::MemoryRange(void* base, size_t capacity)
        : base_(reinterpret_cast<int8_t*>(base))
        , capacity_(capacity)
    {
        SYNTROPY_ASSERT(base_);
    }

    int8_t* MemoryRange::operator*() const
    {
        return base_;
    }

    int8_t* MemoryRange::operator[](size_t offset) const
    {
        SYNTROPY_ASSERT(Contains(base_ + offset, 1));
        return Memory::Offset(base_, offset);
    }

    size_t MemoryRange::GetCapacity() const
    {
        return capacity_;
    }

    bool MemoryRange::Contains(void* address, size_t size) const
    {
        return reinterpret_cast<uintptr_t>(base_) <= reinterpret_cast<uintptr_t>(address) &&
               (Memory::GetSize(base_, address) + size) <= capacity_;
    }

    void MemoryRange::Allocate(void* address, size_t size)
    {
        SYNTROPY_ASSERT(Contains(address, size));

        auto result = Memory::Commit(address, size);

        SYNTROPY_ASSERT(result);
    }

    void MemoryRange::Free(void* address, size_t size)
    {
        SYNTROPY_ASSERT(Contains(address, size));

        auto result = Memory::Decommit(address, size);

        SYNTROPY_ASSERT(result);
    }

    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    MemoryBuffer::MemoryBuffer()
        : buffer_(nullptr)
        , size_(0)
        , allocator_(nullptr)
    {

    }

    MemoryBuffer::MemoryBuffer(size_t size, Allocator& allocator)
        : buffer_(allocator.Allocate(size))
        , size_(size)
        , allocator_(std::addressof(allocator))
    {

    }

    MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : MemoryBuffer(other.size_, *other.allocator_)
    {
        std::memmove(buffer_, other.buffer_, other.size_);      // Copy the buffer's content.
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : buffer_(other.buffer_)
        , size_(other.size_)
        , allocator_(other.allocator_)
    {
        other.buffer_ = nullptr;
        other.size_ = 0;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        if (buffer_)
        {
            allocator_->Free(buffer_);
        }
    }

    MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    void* MemoryBuffer::operator*() const
    {
        return buffer_;
    }

    void* MemoryBuffer::operator[](size_t offset) const
    {
        return Memory::Offset(buffer_, offset);
    }

    size_t MemoryBuffer::GetSize() const
    {
        return size_;
    }

    void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
        std::swap(allocator_, other.allocator_);
    }

}

/************************************************************************/
/* NEW \ DELETE                                                         */
/************************************************************************/

void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. (", stack_trace, ")");

    return allocator.Allocate(size);
}

void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    SYNTROPY_LOG((allocator), "Deallocating. (", stack_trace, ")");

    allocator.Free(ptr);
}

namespace std
{
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second)
    {
        first.Swap(second);
    }
}
