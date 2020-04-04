#include "syntropy/allocators/memory_resource.h"

#include <new>

namespace syntropy
{
    /************************************************************************/
    /* MEMORY RESOURCE                                                      */
    /************************************************************************/

    thread_local MemoryResource* MemoryResource::default_memory_resource_ = &MemoryResource::GetNewDeleteResource();

    MemoryResource& MemoryResource::GetNewDeleteResource() noexcept
    {
        class NewDeleteResource : public MemoryResource
        {
            virtual void* DoAllocate(std::size_t bytes, std::size_t alignment) override
            {
                return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
            }

            virtual void DoDeallocate(void* storage, std::size_t bytes, std::size_t alignment)
            {
                return ::operator delete(storage, bytes, static_cast<std::align_val_t>(alignment));
            }

            virtual bool DoIsEqual(const MemoryResource& rhs) const noexcept
            {
                return this == &rhs;
            }
        };

        static auto new_delete_resource = NewDeleteResource{};

        return new_delete_resource;
    }

}

