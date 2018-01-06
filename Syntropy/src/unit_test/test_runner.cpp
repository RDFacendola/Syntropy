#include "unit_test/test_runner.h"

#include "time/timer.h"

#include <algorithm>

namespace syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    TestRunner& TestRunner::GetInstance()
    {
        static TestRunner instance;
        return instance;
    }

    const std::vector<TestSuite>& TestRunner::GetTestSuites() const
    {
        return test_suites_;
    }

    TestResult TestRunner::Run(const Context& context) const
    {
        // Run each suite and return the result with the highest severity.

        TestResult result{ TestResult::kSuccess };

        for (auto&& test_suite : test_suites_)
        {
            on_test_suite_started_.Notify(*this, OnTestSuiteStartedEventArgs{ &test_suite });

            auto test_result = test_suite.Run(context);

            result = std::max(result, test_result);

            on_test_suite_finished_.Notify(*this, OnTestSuiteFinishedEventArgs{ &test_suite, result });
        }

        return result;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestSuiteStartedEventArgs&>& TestRunner::OnTestSuiteStarted() const
    {
        return on_test_suite_started_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestSuiteFinishedEventArgs&>& TestRunner::OnTestSuiteFinished() const
    {
        return on_test_suite_finished_;
    }

}