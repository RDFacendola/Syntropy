#include "unit_test/test_case.h"

#include <unordered_map>

namespace syntropy
{
    /************************************************************************/
    /* TEST RESULT                                                          */
    /************************************************************************/

    static const std::unordered_map<TestResult, const char*> test_result_map = { { TestResult::kSuccess, "Success" },
                                                                                 { TestResult::kFailure, "Failure" },
                                                                                 { TestResult::kError, "Error" },
                                                                                 { TestResult::kSkipped, "Skipped" } };

    std::ostream& operator<<(std::ostream& out, TestResult test_result)
    {
        auto it = test_result_map.find(test_result);

        if (it != test_result_map.end())
        {
            out << it->second;
        }
        else
        {
            out << "<Unknown test result>";
        }

        return out;
    }

    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    const HashedString& TestCase::GetName() const
    {
        return name_;
    }

    void TestCase::Run() const
    {
        test_case_();
    }
}