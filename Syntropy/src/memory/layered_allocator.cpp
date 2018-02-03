#include "memory/allocators/layered_allocator.h"

#include "diagnostics/log.h"

namespace syntropy
{

    /************************************************************************/
    /* LAYERED ALLOCATOR :: LAYER                                           */
    /************************************************************************/

    Allocator* LayeredAllocator::Layer::operator->()
    {
        return &(allocator_.get());
    }

    const Allocator* LayeredAllocator::Layer::operator->() const
    {
        return &(allocator_.get());
    }

    /************************************************************************/
    /* LAYERED ALLOCATOR                                                    */
    /************************************************************************/

    LayeredAllocator::LayeredAllocator(const HashedString& name, const std::vector<Layer>& layers)
        : Allocator(name)
        , layers_(layers)
    {

    }

    void* LayeredAllocator::Allocate(size_t size)
    {
        for (auto&& layer : layers_)
        {
            if (size <= layer.max_size_)
            {
                return layer->Allocate(size);
            }
        }

        SYNTROPY_CRITICAL((*this), "Cannot allocate ", size, " bytes.");

        return nullptr;
    }

    void* LayeredAllocator::Allocate(size_t size, size_t alignment)
    {
        for (auto&& layer : layers_)
        {
            if (size <= layer.max_size_)
            {
                return layer->Allocate(size, alignment);
            }
        }

        SYNTROPY_CRITICAL((*this), "Cannot allocate ", size, " bytes.");

        return nullptr;
    }

    void LayeredAllocator::Free(void* block)
    {
        for (auto&& layer : layers_)
        {
            if(layer->Owns(block))
            {
                layer->Free(block);
                return;
            }
        }

        SYNTROPY_ERROR((*this), "Can't free the block ", block, " as it's not owned by this allocator.");
    }

    bool LayeredAllocator::Owns(void* block) const
    {
        return std::any_of(layers_.begin(), layers_.end(), [block](const Layer& layer)
        {
            return layer->Owns(block);
        });
    }

    size_t LayeredAllocator::GetMaxAllocationSize() const
    {
        return layers_.back().max_size_;
    }

}
