#include "unit_test/test_suite.h"

#include <algorithm>

namespace syntropy
{
    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    TestSuite::TestSuite(Context name)
        : name_(std::move(name))
    {

    }

    const Context& TestSuite::GetName() const
    {
        return name_;
    }

    const std::vector<TestCase>& TestSuite::GetTestCases() const
    {
        return test_cases_;
    }

    TestResult TestSuite::Run(const Context& context) const
    {
        if (!context.Contains(GetName()))
        {
            return TestResult::kSkipped;                            // The provided context doesn't match: skip the entire suite.
        }

        TestResult result{ TestResult::kSuccess };                  // Even if empty, the suite is not considered to be "skippable" at this point.

        try
        {
            auto fixture = fixture_();                              // Will setup the fixture and destroy it before leaving this method.

            for (auto&& test_case : test_cases_)
            {
                on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ &test_case });

                auto test_result = test_case.Run(*fixture);

                result = std::max(result, test_result);

                on_test_case_finished_.Notify(*this, OnTestCaseFinishedEventArgs{ &test_case, test_result });
            }

            return result;
        }
        catch (...)
        {
            return TestResult::kError;                              // Abort the rest since the fixture may be left in an undefined state. #TODO Trace the error?
        }
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseStartedEventArgs&>& TestSuite::OnTestCaseStarted() const
    {
        return on_test_case_started_;
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseFinishedEventArgs&>& TestSuite::OnTestCaseFinished() const
    {
        return on_test_case_finished_;
    }

}