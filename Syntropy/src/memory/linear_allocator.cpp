#include "memory/linear_allocator.h"

#include <iterator>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy
{

    //////////////// SEQUENTIAL ALLOCATOR ////////////////

    SequentialAllocator::SequentialAllocator(size_t capacity, size_t alignment)
        : memory_(MemoryRange(Memory::Reserve(capacity, alignment), capacity))
        , head_(*memory_)
        , page_head_(*memory_)
    {

    }

    SequentialAllocator::~SequentialAllocator()
    {
        Memory::Release(*memory_);
    }

    void* SequentialAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::Offset(head_, size);

        // Allocate each memory page between the old page head and the new one
        if (std::distance(page_head_, head_) > 0)
        {
            auto next_page = Memory::Align(head_, Memory::GetPageSize());       // Ceil to next memory page boundary.

            auto allocation_size = Memory::GetSize(page_head_, next_page);

            memory_.Allocate(page_head_, allocation_size);                      // Allocate the missing memory pages.

            page_head_ = next_page;
        }

        return block;
    }

    void SequentialAllocator::Free(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        head_ = Memory::Offset(head_, -static_cast<int64_t>(size));

        // Free each memory page between the old page head and the new one
        if (static_cast<size_t>(std::distance(head_, page_head_)) >= Memory::GetPageSize())
        {
            auto next_page = Memory::AlignDown(head_, Memory::GetPageSize());   // Ceil to previous memory page boundary.

            auto free_size = Memory::GetSize(next_page, page_head_);

            page_head_ = next_page;

            memory_.Free(page_head_, free_size);                                // Free the extra amount of memory.

        }
    }

    size_t SequentialAllocator::GetSize() const
    {
        return Memory::GetSize(*memory_, head_);
    }

    size_t SequentialAllocator::GetCommitSize() const
    {
        return Memory::GetSize(*memory_, page_head_);
    }

    size_t SequentialAllocator::GetCapacity() const
    {
        return memory_.GetCapacity();
    }

    int8_t* SequentialAllocator::GetBasePointer()
    {
        return *memory_;
    }

    //////////////// LINEAR ALLOCATOR ////////////////

    LinearAllocator::LinearAllocator(size_t capacity, size_t alignment)
        : memory_(MemoryRange(Memory::Reserve(capacity, alignment), capacity))
        , head_(*memory_)
        , status_(nullptr)
    {
        memory_.Allocate(*memory_, memory_.GetCapacity());      // Allocate everything upfront.
    }

    LinearAllocator::~LinearAllocator()
    {
        Memory::Release(*memory_);
    }

    void* LinearAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::Offset(head_, size);

        SYNTROPY_ASSERT(GetSize() <= GetCapacity());

        return block;
    }

    void* LinearAllocator::Allocate(size_t size, size_t alignment)
    {
        head_ = Memory::Align(head_, alignment);        // Add a padding so that the allocated block is aligned

        auto block = Allocate(size);

        SYNTROPY_ASSERT(Memory::IsAlignedTo(block, alignment));

        return block;
    }

    void LinearAllocator::Free()
    {
        head_ = *memory_;
    }

    void LinearAllocator::SaveStatus()
    {
        status_ = head_;

        *reinterpret_cast<uintptr_t*>(Allocate(sizeof(uintptr_t))) = reinterpret_cast<uintptr_t>(status_);    // Push the current status pointer on the stack
    }

    void LinearAllocator::RestoreStatus()
    {
        SYNTROPY_ASSERT(status_);

        head_ = status_;                                                                // Restore the last status
        status_ = reinterpret_cast<int8_t*>(*reinterpret_cast<uintptr_t*>(head_));      // Move to the next status
    }

    size_t LinearAllocator::GetSize() const
    {
        return Memory::GetSize(*memory_, head_);
    }

    size_t LinearAllocator::GetCapacity() const
    {
        return memory_.GetCapacity();
    }

    int8_t* LinearAllocator::GetBasePointer()
    {
        return *memory_;
    }

    //////////////// SCOPE ALLOCATOR ////////////////

    void ScopeAllocator::Finalizer::operator()() const
    {
        destructor_(object_);
    }

    ScopeAllocator::ScopeAllocator(LinearAllocator& allocator)
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

    //////////////// DOUBLE BUFFERED ALLOCATOR ////////////////

    DoubleBufferedAllocator::DoubleBufferedAllocator(size_t capacity, size_t alignment)
        : allocators_{ {capacity, alignment}, {capacity, alignment} }
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

    LinearAllocator& DoubleBufferedAllocator::GetCurrentAllocator()
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

    size_t DoubleBufferedAllocator::GetSize() const
    {
        return allocators_[0].GetSize() + allocators_[1].GetSize();
    }

    size_t DoubleBufferedAllocator::GetCapacity() const
    {
        return allocators_[0].GetCapacity() + allocators_[1].GetCapacity();
    }

}