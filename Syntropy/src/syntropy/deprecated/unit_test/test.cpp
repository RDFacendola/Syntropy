#include "syntropy/unit_test/test.h"

#include "syntropy/containers/map.h"
#include <algorithm>

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RESULT                                                          */
    /************************************************************************/

    static const Map<TestResult, const char*> test_result_map =
    { 
        { TestResult::kSuccess, "Success" },
        { TestResult::kFailure, "Failure" },
        { TestResult::kError, "Error" },
        { TestResult::kSkipped, "Skipped" } 
    };

    std::ostream& operator<<(std::ostream& out, TestResult test_result)
    {
        auto it = test_result_map.find(test_result);

        SYNTROPY_ASSERT(it != test_result_map.end());

        out << it->second;

        return out;
    }
}
