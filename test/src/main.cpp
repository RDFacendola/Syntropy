
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

#include "syntropy/allocators/passthrough_memory_resource.h"
#include "syntropy/allocators/fixed_memory_resource.h"

#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/range.h"

int main(int argc, char **argv)
{
    using namespace syntropy::literals;

     auto nmr = syntropy::StackMemoryResource<1024>{};
 
     nmr.Allocate(10_Bytes);

    return 0;
}