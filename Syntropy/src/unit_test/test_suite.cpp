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
        on_started_.Notify(*this, OnStartedEventArgs{});

        TestResult result;

        if (!context.Contains(GetName()))
        {
            result = TestResult::kSkipped;                          // The provided context doesn't match: skip the entire suite.
        }
        else
        {
            result = TestResult::kSuccess;                          // Even if empty, the suite is not considered to be "skippable" at this point.

            try
            {
                auto fixture = fixture_();                          // Will setup the fixture and destroy it before leaving this method.

                for (auto&& test_case : test_cases_)
                {
                    // Setup listeners for the current test case.

                    auto on_result_notified_listener = test_case.OnResultNotified().Subscribe([this](auto& sender, auto& args)
                    {
                        on_test_case_result_notified_.Notify(*this, OnTestCaseResultNotifiedEventArgs{ sender, args.result_, args.message_, args.location_ });
                    });

                    auto on_message_notified_listener = test_case.OnMessageNotified().Subscribe([this](auto& sender, auto& args)
                    {
                        on_test_case_message_notified_.Notify(*this, OnTestCaseMessageNotifiedEventArgs{ sender, args.message_ });
                    });

                    // Run the next test case.

                    on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ test_case });

                    auto test_result = test_case.Run(*fixture);

                    result = std::max(result, test_result);

                    on_test_case_finished_.Notify(*this, OnTestCaseFinishedEventArgs{ test_case, test_result });
                }
            }
            catch (...)
            {
                result = TestResult::kError;                        // Abort the rest since the fixture may be left in an undefined state. #TODO Trace the error?
            }
        }

        on_finished_.Notify(*this, OnFinishedEventArgs{ result });

        return result;
    }

    const Observable<const TestSuite&, const TestSuite::OnStartedEventArgs&>& TestSuite::OnStarted() const
    {
        return on_started_;
    }

    const Observable<const TestSuite&, const TestSuite::OnFinishedEventArgs&>& TestSuite::OnFinished() const
    {
        return on_finished_;
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseStartedEventArgs&>& TestSuite::OnTestCaseStarted() const
    {
        return on_test_case_started_;
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseFinishedEventArgs&>& TestSuite::OnTestCaseFinished() const
    {
        return on_test_case_finished_;
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseResultNotifiedEventArgs&>& TestSuite::OnTestCaseResultNotified() const
    {
        return on_test_case_result_notified_;
    }

    const Observable<const TestSuite&, const TestSuite::OnTestCaseMessageNotifiedEventArgs&>& TestSuite::OnTestCaseMessageNotified() const
    {
        return on_test_case_message_notified_;
    }
}