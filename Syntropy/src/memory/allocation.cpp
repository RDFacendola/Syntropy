#include "memory/allocation.h"

#include <cstring>

#include "memory/memory.h"

namespace syntropy
{

    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    MemoryBuffer::MemoryBuffer()
        : base_(nullptr)
        , size_(0)
        , allocator_(nullptr)
    {

    }

    MemoryBuffer::MemoryBuffer(size_t size, Allocator& allocator)
        : base_(SYNTROPY_NEW(allocator) int8_t[size])
        , size_(size)
        , allocator_(std::addressof(allocator))
    {

    }

    MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : MemoryBuffer(other.size_, *other.allocator_)
    {
        std::memmove(base_, other.base_, other.size_);      // Copy the buffer's content.
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : base_(other.base_)
        , size_(other.size_)
        , allocator_(other.allocator_)
    {
        other.base_ = nullptr;
        other.size_ = 0;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        if (base_)
        {
            SYNTROPY_DELETE(*allocator_, base_);
        }
    }

    MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    void* MemoryBuffer::operator*() const
    {
        return base_;
    }

    void* MemoryBuffer::operator[](size_t offset) const
    {
        return Memory::Offset(base_, offset);
    }

    size_t MemoryBuffer::GetSize() const
    {
        return size_;
    }

    void MemoryBuffer::Swap(MemoryBuffer& other) noexcept
    {
        std::swap(base_, other.base_);
        std::swap(size_, other.size_);
        std::swap(allocator_, other.allocator_);
    }

}

/************************************************************************/
/* NEW \ DELETE                                                         */
/************************************************************************/

void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    SYNTROPY_UNUSED(stack_trace);

    return allocator.Allocate(size);
}

void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
{
    SYNTROPY_UNUSED(stack_trace);

    allocator.Free(ptr);
}

namespace std
{

    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second)
    {
        first.Swap(second);
    }

}
