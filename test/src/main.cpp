
#include <vector>
#include <iostream>
#include <string>

#include "syntropy/core/range.h"
#include "syntropy/core/string.h"
#include "syntropy/core/label.h"

#include "syntropy/platform/endianness.h"
#include "syntropy/platform/system.h"

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
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"

#include "syntropy/allocators/memory_context.h"
#include "syntropy/allocators/memory_resource.h"

#include "syntropy/allocators/null_memory_resource.h"
#include "syntropy/allocators/system_memory_resource.h"
#include "syntropy/allocators/stack_memory_resource.h"
#include "syntropy/allocators/virtual_memory_resource.h"
#include "syntropy/allocators/linear_virtual_memory_resource.h"

#include "syntropy/allocators/fixed_memory_resource.h"
#include "syntropy/allocators/linear_memory_resource.h"
#include "syntropy/allocators/pool_memory_resource.h"

#include "syntropy/allocators/passthrough_memory_resource.h"
#include "syntropy/allocators/fallback_memory_resource.h"
#include "syntropy/allocators/counting_memory_resource.h"

#include "syntropy/allocators/polymorphic_allocator.h"
#include "syntropy/allocators/scope_allocator.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/diagnostics/severity.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/diagnostics/debugger.h"

#include "syntropy/math/constants.h"
#include "syntropy/math/hash.h"
#include "syntropy/math/metrics.h"

int main(int argc, char **argv)
{
    using namespace syntropy::Literals;

    auto cpu_info = syntropy::System::GetCPUInfo();
    auto storage_info = syntropy::System::GetStorageInfo();
    auto memory_info = syntropy::System::GetMemoryInfo();
    auto display_info = syntropy::System::GetDisplayInfo();
    auto platform_info = syntropy::System::GetPlatformInfo();

    return 0;
}