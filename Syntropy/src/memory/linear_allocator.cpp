#include "memory/linear_allocator.h"

#include <iterator>

#include "memory/memory.h"
#include "diagnostics/debug.h"
#include "math/math.h"

namespace syntropy
{

    //////////////// SEQUENTIAL ALLOCATOR ////////////////

    SequentialAllocator::SequentialAllocator(size_t capacity, size_t alignment)
        : memory_pool_(capacity, alignment)             // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                   // Get the full range out of the memory pool.
        , head_(*memory_range_)
        , page_head_(*memory_range_)
    {

    }

    SequentialAllocator::SequentialAllocator(const MemoryRange& memory_range, size_t alignment)
        : memory_range_(memory_range, alignment)        // Copy the memory range without taking ownership.
        , head_(*memory_range_)
        , page_head_(*memory_range_)
    {

    }

    void* SequentialAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::AddOffset(head_, size);

        // Commit each memory page between the old page head and the new one
        auto next_page = Memory::Align(head_, Memory::GetPageSize());               // Ceil to next memory page boundary.

        auto allocation_size = Memory::GetDistance(page_head_, next_page);

        if (allocation_size > 0)
        {
            Memory::Commit(page_head_, static_cast<size_t>(allocation_size));       // Commit the missing memory pages.

            page_head_ = next_page;
        }

        return block;
    }

    void SequentialAllocator::Free(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        head_ = Memory::SubOffset(head_, size);

        // Decommit each memory page between the old page head and the new one
        auto next_page = Memory::Align(head_, Memory::GetPageSize());               // Ceil to previous memory page boundary.

        auto free_size = Memory::GetDistance(next_page, page_head_);

        if(free_size > 0)
        {
            page_head_ = next_page;

            Memory::Decommit(page_head_, static_cast<size_t>(free_size));           // Free the extra amount of memory.
        }
    }

    size_t SequentialAllocator::GetSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, head_));
    }

    size_t SequentialAllocator::GetEffectiveSize() const
    {
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, page_head_));
    }

    size_t SequentialAllocator::GetCapacity() const
    {
        return memory_range_.GetSize();
    }

    void* SequentialAllocator::GetBasePointer()
    {
        return *memory_range_;
    }

    const void* SequentialAllocator::GetBasePointer() const
    {
        return *memory_range_;
    }

    bool SequentialAllocator::ContainsAddress(void* address) const
    {
        return memory_range_.Contains(address);
    }

    //////////////// LINEAR ALLOCATOR ////////////////

    LinearAllocator::LinearAllocator(size_t capacity, size_t alignment)
        : memory_pool_(capacity, alignment)             // Allocate a new virtual address range.
        , memory_range_(memory_pool_)                   // Get the full range out of the memory pool.
        , head_(*memory_range_)
        , status_(nullptr)
    {
        // Allocate everything upfront.
        Memory::Commit(*memory_range_, memory_range_.GetSize());
    }

    LinearAllocator::LinearAllocator(const MemoryRange& memory_range, size_t alignment)
        : memory_range_(memory_range, alignment)        // Copy the memory range without taking ownership.
        , head_(*memory_range_)
        , status_(nullptr)
    {
        // Allocate everything upfront.
        Memory::Commit(*memory_range_, memory_range_.GetSize());
    }

    void* LinearAllocator::Allocate(size_t size)
    {
        SYNTROPY_ASSERT(size > 0);

        auto block = head_;

        head_ = Memory::AddOffset(head_, size);

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
        head_ = *memory_range_;
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
        return static_cast<size_t>(Memory::GetDistance(*memory_range_, head_));
    }

    size_t LinearAllocator::GetEffectiveSize() const
    {
        return GetCapacity();   // Memory is allocated upfront.
    }

    size_t LinearAllocator::GetCapacity() const
    {
        return memory_range_.GetSize();
    }

    void* LinearAllocator::GetBasePointer()
    {
        return *memory_range_;
    }

    bool LinearAllocator::ContainsAddress(void* address) const
    {
        return MemoryRange(*memory_range_, head_).Contains(address);
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

    size_t DoubleBufferedAllocator::GetEffectiveSize() const
    {
        return allocators_[0].GetEffectiveSize() + allocators_[1].GetEffectiveSize();
    }

    size_t DoubleBufferedAllocator::GetCapacity() const
    {
        return allocators_[0].GetCapacity() + allocators_[1].GetCapacity();
    }

}