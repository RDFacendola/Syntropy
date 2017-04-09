
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

        /// \brief Create a new master allocator.
        /// \param name Name of the allocator.
        /// \param capacity Maximum capacity of the allocator. This capacity doesn't account for the capacity of both the small and the large allocator.
        /// \param small_allocator Allocator used to handle small allocations.
        /// \param large_allocator Allocator used to handle large allocations.
        MasterAllocator(const HashedString& name, size_t capacity, LinearSegregatedFitAllocator& small_allocator, ExponentialSegregatedFitAllocator& large_allocator);

        /// \brief No copy constructor.
        MasterAllocator(const MasterAllocator&) = delete;

        /// \brief No assignment operator.
        MasterAllocator& operator=(const MasterAllocator&) = delete;

        /// \brief Default virtual destructor.
        virtual ~MasterAllocator() = default;

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Owns(void* block) const override;

        virtual size_t GetMaxAllocationSize() const override;

    private:

        static constexpr size_t kDefaultSecondLevelIndex = 5;       ///< \brief Default SLI value for the medium allocator.

        size_t max_small_size_;                                     ///< \brief Maximum size for a small allocation, in bytes.

        size_t max_medium_size_;                                    ///< \brief Maximum size for a medium allocation, in bytes.

        size_t max_large_size_;                                     ///< \brief Maximum size for a large allocation, in bytes.

        LinearSegregatedFitAllocator& small_allocator_;             ///< \brief Allocator used to handle small allocations.

        TwoLevelSegregatedFitAllocator medium_allocator_;           ///< \brief Handles allocation of medium objects.

        ExponentialSegregatedFitAllocator& large_allocator_;        ///< \brief Allocator used to handle large allocations.

    };

}

