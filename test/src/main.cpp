
#include <vector>
#include <iostream>
#include <string>

#include "syntropy/core/range.h"
#include "syntropy/core/string.h"

#include "syntropy/language/macro.h"
#include "syntropy/language/sstream.h"

#include "syntropy/containers/array.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/containers/set.h"

#include "syntropy/patterns/scope_guard.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/observer_ptr.h"
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"

#include "syntropy/allocators/memory_context.h"
#include "syntropy/allocators/memory_resource.h"

#include "syntropy/allocators/null_memory_resource.h"
#include "syntropy/allocators/heap_memory_resource.h"
#include "syntropy/allocators/stack_memory_resource.h"
#include "syntropy/allocators/virtual_memory_resource.h"

#include "syntropy/allocators/fixed_memory_resource.h"
#include "syntropy/allocators/linear_memory_resource.h"
#include "syntropy/allocators/pool_memory_resource.h"

#include "syntropy/allocators/passthrough_memory_resource.h"
#include "syntropy/allocators/chain_memory_resource.h"
#include "syntropy/allocators/counting_memory_resource.h"

#include "syntropy/allocators/polymorphic_allocator.h"
#include "syntropy/allocators/scope_allocator.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/math/constants.h"

int main(int argc, char **argv)
{
    using namespace syntropy::literals;

    auto hmr = syntropy::HeapMemoryResource{};

    auto V = syntropy::Vector<syntropy::Vector<int>>{ {1, 2}, {3, 4, 5} };

    auto ctx = syntropy::MakeMemoryContext(hmr);        // ERROR!!!! Containers will take a reference to the wrapper allocator, causing undefined behavior when the context goes out of scope.

    auto S = syntropy::Map<int, syntropy::Vector<int>>{ {1, {1, 1, 1} }, {2, {2, 2}} };

    {
        S.insert(std::make_pair(3, syntropy::Vector<int>{3, 3, 3}));
    }


    return 0;
}