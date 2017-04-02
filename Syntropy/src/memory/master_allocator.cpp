#include "memory/master_allocator.h"

#include "diagnostics/log.h"

namespace syntropy
{

    /************************************************************************/
    /* MASTER ALLOCATOR                                                     */
    /************************************************************************/

    MasterAllocator::MasterAllocator(const HashedString& name, const Configuration& configuration)
        : Allocator(name)
        , configuration_(configuration)
        , medium_allocator_(configuration_.medium_allocator_capacity_, configuration_.medium_sli_)
    {

    }

    void* MasterAllocator::Allocate(size_t size)
    {
        if (size <= configuration_.max_small_size_)
        {
            // Small allocation.
            return configuration_.small_allocator_.Allocate(size);
        }
        else if (size <= configuration_.max_medium_size_)
        {
            // Medium allocation.
            return medium_allocator_.Allocate(size);
        }
        else if (size <= configuration_.max_large_size_)
        {
            // Large allocation.
            return configuration_.large_allocator_.Allocate(size);
        } 

        SYNTROPY_ERROR((MemoryCtx | GetName()), "Cannot allocate ", size, " bytes.");

        throw std::bad_alloc();
    }

    void* MasterAllocator::Allocate(size_t size, size_t alignment)
    {
        if (size <= configuration_.max_small_size_)
        {
            // Small aligned allocation.
            return configuration_.small_allocator_.Allocate(size, alignment);
        }
        else if (size <= configuration_.max_medium_size_)
        {
            // Medium aligned allocation.
            return medium_allocator_.Allocate(size, alignment);
        }
        else if (size <= configuration_.max_large_size_)
        {
            // Large aligned allocation.
            return configuration_.large_allocator_.Allocate(size, alignment);
        }
        else
        {

            SYNTROPY_ERROR((MemoryCtx | GetName()), "Cannot allocate ", size, " bytes.");

            throw std::bad_alloc();
        }
    }

    void MasterAllocator::Free(void* block)
    {
        if (configuration_.small_allocator_.Belongs(block))
        {
            // Small free.
            configuration_.small_allocator_.Free(block);
        }
        else if (medium_allocator_.Belongs(block))
        {
            // Medium free.
            medium_allocator_.Free(block);
        }
        else if (configuration_.large_allocator_.Belongs(block))
        {
            // Large free.
            configuration_.large_allocator_.Free(block);
        }
        else
        {
            SYNTROPY_ERROR((MemoryCtx | GetName()), "The block ", block, " doesn't belong to the allocator");
        }
    }

    bool MasterAllocator::Belongs(void* block) const
    {
        return configuration_.small_allocator_.Belongs(block) ||
            medium_allocator_.Belongs(block) ||
            configuration_.large_allocator_.Belongs(block);
    }

    size_t MasterAllocator::GetMaxAllocationSize() const
    {
        return configuration_.large_allocator_.GetMaxAllocationSize();
    }

}
