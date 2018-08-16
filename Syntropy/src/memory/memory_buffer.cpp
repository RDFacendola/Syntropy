#include "memory/memory_buffer.h"

#include <cstring>

namespace syntropy
{

    /************************************************************************/
    /* MEMORY BUFFER                                                        */
    /************************************************************************/

    MemoryBuffer::MemoryBuffer()
        : allocator_(nullptr)
    {

    }

    MemoryBuffer::MemoryBuffer(Bytes /*size*/, Allocator& /*allocator*/)
        //: range_(SYNTROPY_ALLOC(allocator, size), size)
        //, allocator_(std::addressof(allocator))
    {

    }

    MemoryBuffer::MemoryBuffer(const MemoryBuffer& other)
        : MemoryBuffer(other.GetSize(), *other.allocator_)
    {
        std::memmove(range_.Begin(), other.range_.Begin(), std::size_t(other.GetSize()));           // Copy the buffer's content.
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
        if (range_)
        {
            SYNTROPY_FREE(*allocator_, range_.Begin());
        }
    }

    MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer other)
    {
        Swap(other);
        return *this;
    }

    void* MemoryBuffer::operator*() const
    {
        return range_.Begin();
    }

    void* MemoryBuffer::operator[](Bytes offset) const
    {
        return range_[offset];
    }

    Bytes MemoryBuffer::GetSize() const
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

namespace std
{
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second)
    {
        first.Swap(second);
    }
}
