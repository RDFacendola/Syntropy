#include "syntropy/unit_test/test_runner.h"

#include "syntropy/time/timer.h"

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

    const Vector<TestSuite>& TestRunner::GetTestSuites() const
    {
        return test_suites_;
    }

    TestResult TestRunner::Run(const Context& context) const
    {
        // Run each suite and return the result with the highest severity.

        on_started_.Notify(*this, OnStartedEventArgs{});

        TestResult result{ TestResult::kSuccess };

        for (auto&& test_suite : test_suites_)
        {
            // Setup listeners for both the current test suite and its test cases.

            auto on_test_case_started_listener = test_suite.OnTestCaseStarted().Subscribe([this](auto& sender, auto& args)
            {
                on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ sender, args.test_case_ });
            });

            auto on_test_case_finished_listener = test_suite.OnTestCaseFinished().Subscribe([this](auto& sender, auto& args)
            {
                on_test_case_finished_.Notify(*this, OnTestCaseFinishedEventArgs{ sender, args.test_case_, args.result_ });
            });

            auto on_result_notified_listener_ = test_suite.OnTestCaseResultNotified().Subscribe([this](auto& sender, auto& args)
            {
                on_test_case_result_notified_.Notify(*this, OnTestCaseResultNotifiedEventArgs{ sender, args.test_case_, args.result_, args.message_, args.location_ });
            });

            auto on_message_notified_listener = test_suite.OnTestCaseMessageNotified().Subscribe([this](auto& sender, auto& args)
            {
                on_test_case_message_notified_.Notify(*this, OnTestCaseMessageNotifiedEventArgs{ sender, args.test_case_, args.message_ });
            });

            // Run the next test suite.

            on_test_suite_started_.Notify(*this, OnTestSuiteStartedEventArgs{ test_suite });

            auto test_result = test_suite.Run(context);

            result = std::max(result, test_result);

            on_test_suite_finished_.Notify(*this, OnTestSuiteFinishedEventArgs{ test_suite, result });
        }

        on_finished_.Notify(*this, OnFinishedEventArgs{ result });

        return result;
    }

    const Observable<const TestRunner&, const TestRunner::OnStartedEventArgs&>& TestRunner::OnStarted() const
    {
        return on_started_;
    }

    const Observable<const TestRunner&, const TestRunner::OnFinishedEventArgs&>& TestRunner::OnFinished() const
    {
        return on_finished_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestSuiteStartedEventArgs&>& TestRunner::OnTestSuiteStarted() const
    {
        return on_test_suite_started_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestSuiteFinishedEventArgs&>& TestRunner::OnTestSuiteFinished() const
    {
        return on_test_suite_finished_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestCaseStartedEventArgs&>& TestRunner::OnTestCaseStarted() const
    {
        return on_test_case_started_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestCaseFinishedEventArgs&>& TestRunner::OnTestCaseFinished() const
    {
        return on_test_case_finished_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestCaseResultNotifiedEventArgs&>& TestRunner::OnTestCaseResultNotified() const
    {
        return on_test_case_result_notified_;
    }

    const Observable<const TestRunner&, const TestRunner::OnTestCaseMessageNotifiedEventArgs&>& TestRunner::OnTestCaseMessageNotified() const
    {
        return on_test_case_message_notified_;
    }

}