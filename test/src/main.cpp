
#include <iostream>

#include "syntropy/core/range.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/label.h"
#include "syntropy/core/context.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/platform/endianness.h"
#include "syntropy/platform/system.h"
#include "syntropy/platform/threading.h"
#include "syntropy/platform/intrinsics.h"

#include "syntropy/language/macro.h"
#include "syntropy/language/scope_guard.h"
#include "syntropy/language/utility.h"
#include "syntropy/language/tuple.h"
#include "syntropy/language/event.h"

#include "syntropy/application/command_line.h"
#include "syntropy/application/command_line_argument.h"

#include "syntropy/containers/array.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/map.h"
#include "syntropy/containers/set.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/virtual_memory.h"
#include "syntropy/memory/virtual_memory_range.h"
#include "syntropy/memory/memory_buffer.h"

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
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/diagnostics/debugger.h"
#include "syntropy/diagnostics/log_event.h"
#include "syntropy/diagnostics/log_channel.h"
#include "syntropy/diagnostics/log_manager.h"

#include "syntropy/math/constants.h"
#include "syntropy/math/numeric.h"
#include "syntropy/math/math.h"
#include "syntropy/math/bits.h"
#include "syntropy/math/interpolations.h"
#include "syntropy/math/vector.h"
#include "syntropy/math/quaternion.h"
#include "syntropy/math/rotation.h"
#include "syntropy/math/hash.h"
#include "syntropy/math/codes.h"

#include "syntropy/math/random.h"
#include "syntropy/math/random_engine.h"
#include "syntropy/math/random_context.h"
#include "syntropy/math/pcg_random_engine.h"

#include "syntropy/time/timer.h"
#include "syntropy/time/date.h"
#include "syntropy/time/time_of_day.h"

#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_fixture.h"
#include "syntropy/unit_test/test_case.h"

#include <cmath>
#include <iostream>

class MyFixture : public syntropy::TestFixture
{
public:

    void Foo()
    {
        SYNTROPY_UNIT_ASSERT(1 == 1);
    }

};

int main(int argc, char **argv)
{
    auto my_fixture = MyFixture();

    auto test_case = syntropy::MakeTestCase("foo", &MyFixture::Foo);

    std::cout << test_case.Run(my_fixture);

    SYNTROPY_BREAK;

    system("pause");

    return 0;
}