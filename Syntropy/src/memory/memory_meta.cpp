#include  "memory/memory_meta.h"

#include "reflection/reflection.h"

namespace syntropy::memory
{
    reflection::AutoRegisterClass<Allocator> allocator_class;

//     reflection::AutoRegisterClass<LinearSegregatedFitAllocator> linear_segregated_fit_allocator;
//     reflection::AutoRegisterClass<ExponentialSegregatedFitAllocator> exponential_segregated_fit_allocator;
//     reflection::AutoRegisterClass<TwoLevelSegregatedFitAllocator> two_level_segregated_fit_allocator;
}