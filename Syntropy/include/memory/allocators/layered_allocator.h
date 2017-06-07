
/// \file layered_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains application-level allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>
#include <functional>

#include "type_traits.h"

#include "memory/memory.h"

#include "memory/allocators/segregated_allocator.h"

namespace syntropy
{

    /// \brief Represents a compound allocator composed by any number of sub allocators (layer).
    /// Each layer handles allocation request up to a certain size, if the requested allocation exceeds that limit the next layer is used instead.
    /// This process continues until one of the layer could fulfill the request or the layers are exhausted.
    /// This allocator can be used to redirect certain allocation sizes to the most efficient allocator that could handle the requests.
    /// Different layered allocators may refer to some shared allocators to avoid fragmentation.
    /// \author Raffaele D. Facendola - June 2017
    class LayeredAllocator : public Allocator
    {
    public:

        /// \brief Represents an allocation layer.
        struct Layer
        {
            std::reference_wrapper<Allocator> allocator_;       ///< \brief Allocator used to handle the allocation of this layer.

            size_t max_size_;                                   ///< \brief Maximum allocation size that can be handled by this layer.

            /// \brief Get a reference to the allocator.
            Allocator* operator->();

            /// \brief Get a reference to the allocator.
            const Allocator* operator->() const;
        };

        /// \brief Create a new layered allocator.
        /// \param name Name of the allocator.
        /// \param layers Layers in this allocator. The order of these layers determines the priority of the allocator handling incoming allocation requests.
        LayeredAllocator(const HashedString& name, const std::vector<Layer>& layers);

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Owns(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

    private:

        std::vector<Layer> layers_;     ///< \brief Allocation layers.
    };

}

