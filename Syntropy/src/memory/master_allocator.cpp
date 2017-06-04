#include "memory/allocators/master_allocator.h"

#include "diagnostics/log.h"

namespace syntropy
{

    /************************************************************************/
    /* MASTER ALLOCATOR                                                     */
    /************************************************************************/

    MasterAllocator::MasterAllocator(const HashedString& name, size_t capacity, LinearSegregatedFitAllocator& small_allocator, ExponentialSegregatedFitAllocator& large_allocator)
        : Allocator(name)
        , max_small_size_(small_allocator.GetMaxAllocationSize())
        , max_medium_size_(large_allocator.GetClassSize() >> 1)
        , max_large_size_(large_allocator.GetMaxAllocationSize())
        , small_allocator_(small_allocator)
        , medium_allocator_(name, capacity, kDefaultSecondLevelIndex)
        , large_allocator_(large_allocator)
    {

    }

    void* MasterAllocator::Allocate(size_t size)
    {
        if (size <= max_small_size_)
        {
            return small_allocator_.Allocate(size);     
        }
        else if (size <= max_medium_size_)
        {
            return medium_allocator_.Allocate(size);
        }
        else if (size <= max_large_size_)
        {
            return large_allocator_.Allocate(size);
        } 
        else
        {
            SYNTROPY_ERROR((MemoryCtx | GetName()), "Cannot allocate ", size, " bytes.");

            throw std::bad_alloc();
        }
    }

    void* MasterAllocator::Allocate(size_t size, size_t alignment)
    {
        if (size <= max_small_size_)
        {
            return small_allocator_.Allocate(size, alignment);
        }
        else if (size <= max_medium_size_)
        {
            return medium_allocator_.Allocate(size, alignment);
        }
        else if (size <= max_large_size_)
        {
            return large_allocator_.Allocate(size, alignment);
        }
        else
        {
            SYNTROPY_ERROR((MemoryCtx | GetName()), "Cannot allocate ", size, " bytes.");

            throw std::bad_alloc();
        }
    }

    void MasterAllocator::Free(void* block)
    {
        if (small_allocator_.Owns(block))
        {
            small_allocator_.Free(block);
        }
        else if (medium_allocator_.Owns(block))
        {
            medium_allocator_.Free(block);
        }
        else if (large_allocator_.Owns(block))
        {
            large_allocator_.Free(block);
        }
        else
        {
            SYNTROPY_ERROR((MemoryCtx | GetName()), "Can't free the block ", block, " as it's not owned by this allocator.");
        }
    }

    bool MasterAllocator::Owns(void* block) const
    {
        return small_allocator_.Owns(block) ||
            medium_allocator_.Owns(block) ||
            large_allocator_.Owns(block);
    }

    size_t MasterAllocator::GetMaxAllocationSize() const
    {
        return large_allocator_.GetMaxAllocationSize();
    }

}
