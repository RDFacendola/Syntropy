#include "memory/linear_allocator.h"

#include <iterator>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy
{

    //////////////// LINEAR ALLOCATOR ////////////////

    LinearAllocator::LinearAllocator(size_t capacity)
        : base_(reinterpret_cast<int8_t*>(GetMemory().Allocate(capacity)))
        , head_(base_)
        , status_(nullptr)
        , capacity_(capacity)
    {

    }

    LinearAllocator::~LinearAllocator()
    {
        GetMemory().Free(base_);
    }

    void* LinearAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ += size;

        SYNTROPY_ASSERT(GetSize() <= GetCapacity());

        return block;
    }

    void* LinearAllocator::Allocate(size_t size, size_t alignment)
    {
        head_ = reinterpret_cast<int8_t*>(Math::NextMultipleOf(reinterpret_cast<size_t>(head_), alignment));        // Align the head to the requested amount

        auto block = Allocate(size);

        SYNTROPY_ASSERT(reinterpret_cast<size_t>(block) % alignment == 0);

        return block;
    }

    void LinearAllocator::Free()
    {
        head_ = base_;
    }

    void LinearAllocator::SaveStatus()
    {
        // Push the current status pointer on the stack

        *reinterpret_cast<size_t*>(head_) = reinterpret_cast<size_t>(status_);
        
        status_ = head_;
        head_ += sizeof(size_t);

        SYNTROPY_ASSERT(GetSize() <= GetCapacity());
    }

    void LinearAllocator::RestoreStatus()
    {
        // Restore the last status
        SYNTROPY_ASSERT(status_);

        head_ = status_;
        status_ = reinterpret_cast<int8_t*>(*reinterpret_cast<size_t*>(head_));
    }

    size_t LinearAllocator::GetSize() const
    {
        return std::distance(base_, head_);
    }

    size_t LinearAllocator::GetCapacity() const
    {
        return capacity_;
    }

    //////////////// DOUBLE BUFFERED ALLOCATOR ////////////////

    DoubleBufferedAllocator::DoubleBufferedAllocator(size_t capacity)
        : allocators_{ {capacity}, {capacity} }
        , current_(allocators_)
    {

    }

    void* DoubleBufferedAllocator::Allocate(size_t size)
    {
        return current_->Allocate(size);
    }

    void* DoubleBufferedAllocator::Allocate(size_t size, size_t alignment)
    {
        return current_->Allocate(size, alignment);
    }

    void DoubleBufferedAllocator::Free()
    {
        current_->Free();
    }

    void DoubleBufferedAllocator::Flip()
    {
        current_ = allocators_ + (std::distance(allocators_, current_) + 1) % 2;
        Free();
    }

    void DoubleBufferedAllocator::SaveStatus()
    {
        current_->SaveStatus();
    }

    void DoubleBufferedAllocator::RestoreStatus()
    {
        current_->RestoreStatus();
    }

    size_t DoubleBufferedAllocator::GetSize() const
    {
        return allocators_[0].GetSize() + allocators_[1].GetSize();
    }

    size_t DoubleBufferedAllocator::GetCapacity() const
    {
        return allocators_[0].GetCapacity() + allocators_[1].GetCapacity();
    }

}