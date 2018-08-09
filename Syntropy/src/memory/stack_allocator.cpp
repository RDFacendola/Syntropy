#include "memory/allocators/stack_allocator.h"

#include "diagnostics/assert.h"

namespace syntropy
{

    /************************************************************************/
    /* STACK ALLOCATOR                                                      */
    /************************************************************************/

    StackAllocator::StackAllocator(Bytes capacity, Alignment alignment)
        : memory_pool_(capacity, alignment)
        , memory_range_(memory_pool_)
        , head_(memory_range_.Begin())
        , status_(nullptr)
    {
        VirtualMemory::Commit(memory_range_);                   // Allocate everything upfront.
    }

    StackAllocator::StackAllocator(const MemoryRange& memory_range, Alignment alignment)
        : memory_range_(memory_range.Begin().GetAligned(alignment), memory_range.End())
        , head_(memory_range_.Begin())
        , status_(nullptr)
    {
        VirtualMemory::Commit(memory_range_);                           // Allocate everything upfront.
    }

    void* StackAllocator::Allocate(Bytes size)
    {
        SYNTROPY_PRECONDITION(size > 0_Bytes);

        std::lock_guard<std::mutex> lock(mutex_);

        auto block = head_;

        head_ += size;

        SYNTROPY_ASSERT(head_ <= memory_range_.End());               // Out-of-memory check.

        return block;
    }

    void* StackAllocator::Allocate(Bytes size, Alignment alignment)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        head_ = head_.GetAligned(alignment);                            // Align the head to the provided alignment.

        auto block = Allocate(size);

        SYNTROPY_ASSERT(MemoryAddress(block).IsAlignedTo(alignment));

        return block;
    }

    void StackAllocator::Free()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        head_ = memory_range_.Begin();
    }

    void StackAllocator::SaveStatus()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        status_ = head_;

        *reinterpret_cast<uintptr_t*>(Allocate(Bytes(sizeof(uintptr_t)))) = uintptr_t(status_);    // Push the current status pointer on the stack
    }

    void StackAllocator::RestoreStatus()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        SYNTROPY_PRECONDITION(status_ != MemoryAddress(nullptr));

        auto previous_head = head_;

        head_ = status_;                                                                    // Restore the last status
        status_ = reinterpret_cast<int8_t*>(*reinterpret_cast<uintptr_t*>(*head_));         // Move to the next status
    }

    Bytes StackAllocator::GetAllocationSize() const
    {
        return Bytes(head_ - memory_range_.Begin());
    }

    Bytes StackAllocator::GetCommitSize() const
    {
        return memory_range_.GetSize();
    }

    /************************************************************************/
    /* SCOPE ALLOCATOR                                                      */
    /************************************************************************/

    void ScopeAllocator::Finalizer::operator()() const
    {
        destructor_(object_);
    }

    ScopeAllocator::ScopeAllocator(StackAllocator& allocator)
        : allocator_(allocator)
        , finalizer_list_(nullptr)
    {
        allocator.SaveStatus();         // Save the current status of the allocator
    }

    ScopeAllocator::~ScopeAllocator()
    {
        while (finalizer_list_)
        {
            (*finalizer_list_)();                           // Destroy each allocated object in reverse order
            finalizer_list_ = finalizer_list_->next_;       // Move to the next finalizer
        }

        allocator_.RestoreStatus();                         // Restore the old status of the allocator
    }

    /************************************************************************/
    /* DOUBLE BUFFERED ALLOCATOR                                            */
    /************************************************************************/

    DoubleBufferedAllocator::DoubleBufferedAllocator(Bytes capacity, Alignment alignment)
        : allocators_{ { capacity, alignment },{ capacity, alignment } }
        , current_(allocators_)
    {

    }

    void* DoubleBufferedAllocator::Allocate(Bytes size)
    {
        return current_->Allocate(size);
    }

    void* DoubleBufferedAllocator::Allocate(Bytes size, Alignment alignment)
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

    StackAllocator& DoubleBufferedAllocator::GetCurrentAllocator()
    {
        return *current_;
    }

    void DoubleBufferedAllocator::SaveStatus()
    {
        current_->SaveStatus();
    }

    void DoubleBufferedAllocator::RestoreStatus()
    {
        current_->RestoreStatus();
    }

    Bytes DoubleBufferedAllocator::GetAllocationSize() const
    {
        return allocators_[0].GetAllocationSize() + allocators_[1].GetAllocationSize();
    }

    Bytes DoubleBufferedAllocator::GetCommitSize() const
    {
        return allocators_[0].GetCommitSize() + allocators_[1].GetCommitSize();
    }

}

