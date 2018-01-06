#include "unit_test/test.h"

#include <unordered_map>
#include <algorithm>

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RESULT                                                          */
    /************************************************************************/

    static const std::unordered_map<TestResult, const char*> test_result_map =
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
