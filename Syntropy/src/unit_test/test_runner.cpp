#include "unit_test/test_runner.h"

#include "time/timer.h"

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

    void TestRunner::Run(const Context& context) const
    {
        for (auto&& test_suite : test_suites_)
        {
            on_test_suite_started_.Notify(*this, OnTestSuiteStartedEventArgs{ &test_suite });

            OnTestSuiteFinishedEventArgs result;

            HighResolutionTimer<std::chrono::milliseconds> timer(true);

            auto test_result = test_suite.Run(context);

            on_test_suite_finished_.Notify(*this, OnTestSuiteFinishedEventArgs{ &test_suite, std::move(test_result), timer.Stop() });
        }
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