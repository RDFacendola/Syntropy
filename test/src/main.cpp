
#include <vector>
#include <string>

#include "syntropy/language/macro.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/observer_ptr.h"

#include "syntropy/allocators/null_memory_resource.h"
#include "syntropy/allocators/heap_memory_resource.h"
#include "syntropy/allocators/stack_memory_resource.h"

#include "syntropy/allocators/fixed_memory_resource.h"
#include "syntropy/allocators/linear_memory_resource.h"
#include "syntropy/allocators/pool_memory_resource.h"

#include "syntropy/allocators/passthrough_memory_resource.h"
#include "syntropy/allocators/chain_memory_resource.h"
#include "syntropy/allocators/counting_memory_resource.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/range.h"

int main(int argc, char **argv)
{
    using namespace syntropy::literals;

    auto nmr0 = syntropy::StackMemoryResource<1024>{};
    auto nmr1 = syntropy::StackMemoryResource<1024>{};

    auto lmr = syntropy::LinearMemoryResource<syntropy::StackMemoryResource<1024>>(1024_Bytes);
    auto pmr = syntropy::PoolMemoryResource<syntropy::StackMemoryResource<1024>>(syntropy::BytesOf<int>(), 1024_Bytes);

    lmr.Allocate(15_Bytes);
    pmr.Allocate(4_Bytes);

    return 0;
}