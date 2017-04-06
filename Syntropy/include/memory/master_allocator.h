
/// \file master_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains global allocators meant to be used directly by the application.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>

#include "memory.h"
#include "segregated_allocator.h"

namespace syntropy
{

    /// \brief Master allocator used to handle allocation of any size.
    /// Aggregates multiple allocators, using the most effective one when an allocation is requested to reduce fragmentation and increase performances.
    /// Small and large allocators can be shared by multiple instances to save on virtual memory addresses or to keep similar-sized allocations together and prevent fragmentation.
    /// \author Raffaele D. Facendola - March 2017
    class MasterAllocator : public Allocator
    {
    public:

        /// \brief Configuration for a master allocator.
        /// Different master allocators may share part of their state (such as sub-allocators and other stuffs)
        struct Configuration
        {
            size_t max_small_size_;                                 /// \brief Maximum size for a small allocation, in bytes.

            size_t max_medium_size_;                                /// \brief Maximum size for a medium allocation, in bytes.

            size_t max_large_size_;                                 /// \brief Maximum size for a large allocation, in bytes.

            LinearSegregatedFitAllocator& small_allocator_;         /// \brief Allocator used to handle small allocations.

            ExponentialSegregatedFitAllocator& large_allocator_;    /// \brief Allocator used to handle large allocations.

            size_t medium_allocator_capacity_;                      /// \brief Maximum capacity for the medium allocator, in bytes.

            size_t medium_sli_;                                     /// \brief Maximum second level index of the medium allocator, as log2(SLI).
         };

        /// \brief Create a new master allocator.
        /// \param name Name of the allocator.
        /// \param configuration Configuration of the allocator.
        MasterAllocator(const HashedString& name, const Configuration& configuration);

        /// \brief No copy constructor.
        MasterAllocator(const MasterAllocator&) = delete;

        /// \brief No assignment operator.
        MasterAllocator& operator=(const MasterAllocator&) = delete;

        /// \brief Default virtual destructor.
        virtual ~MasterAllocator() = default;

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Belongs(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

    private:

        Configuration configuration_;                               /// \brief Allocator configuration.

        TwoLevelSegregatedFitAllocator medium_allocator_;           /// \brief Handles allocation of medium objects.

    };

}

