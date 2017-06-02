#include "memory/stack_allocator.h"

#include "diagnostics/assert.h"

namespace syntropy
{

    /************************************************************************/
    /* STACK ALLOCATOR                                                      */
    /************************************************************************/

    StackAllocator::StackAllocator(size_t capacity, size_t alignment)
        : memory_pool_(capacity, alignment)             // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                   // Get the full range out of the memory pool.
        , head_(*memory_range_)
        , status_(nullptr)
    {
        // Allocate everything upfront.
        VirtualMemory::Commit(*memory_range_, memory_range_.GetSize());
    }

    StackAllocator::StackAllocator(const MemoryRange& memory_range, size_t alignment)
        : memory_range_(memory_range, alignment)        // Copy the memory range without taking ownership.
        , head_(*memory_range_)
        , status_(nullptr)
    {
        // Allocate everything upfront.
        VirtualMemory::Commit(*memory_range_, memory_range_.GetSize());
    }

    void* StackAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::AddOffset(head_, size);

        SYNTROPY_ASSERT(Memory::GetDistance(memory_range_.GetTop(), head_) <= 0);       // Out-of-memory check.

        MemoryDebug::MarkUninitialized(block, head_);

        return block;
    }

    void* StackAllocator::Allocate(size_t size, size_t alignment)
    {
        head_ = Memory::Align(head_, alignment);        // Add a padding so that the allocated block is aligned

        auto block = Allocate(size);

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void StackAllocator::Free()
    {
        MemoryDebug::MarkFree(*memory_range_, head_);

        head_ = *memory_range_;
    }

    void StackAllocator::SaveStatus()
    {
        status_ = head_;

        *reinterpret_cast<uintptr_t*>(Allocate(sizeof(uintptr_t))) = reinterpret_cast<uintptr_t>(status_);    // Push the current status pointer on the stack
    }

    void StackAllocator::RestoreStatus()
    {
        SYNTROPY_ASSERT(status_);

        auto previous_head = head_;

        head_ = status_;                                                                // Restore the last status
        status_ = reinterpret_cast<int8_t*>(*reinterpret_cast<uintptr_t*>(head_));      // Move to the next status

        MemoryDebug::MarkFree(head_, previous_head);
    }

    size_t StackAllocator::GetAllocationSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, head_));
    }

    size_t StackAllocator::GetCommitSize() const
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

    DoubleBufferedAllocator::DoubleBufferedAllocator(size_t capacity, size_t alignment)
        : allocators_{ { capacity, alignment },{ capacity, alignment } }
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

    size_t DoubleBufferedAllocator::GetAllocationSize() const
    {
        return allocators_[0].GetAllocationSize() + allocators_[1].GetAllocationSize();
    }

    size_t DoubleBufferedAllocator::GetCommitSize() const
    {
        return allocators_[0].GetCommitSize() + allocators_[1].GetCommitSize();
    }

}

