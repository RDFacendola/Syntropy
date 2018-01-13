#include "unit_test/test_case.h"

#include <optional>
#include <algorithm>

#include "diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    const HashedString& TestCase::GetName() const
    {
        return name_;
    }

    TestResult TestCase::Run(TestFixture& fixture) const
    {
        on_started_.Notify(*this, OnStartedEventArgs{});

        std::optional<TestResult> test_result;

        auto on_result_notified = fixture.OnResultNotified().Subscribe([this, &test_result](auto& /*sender*/, auto& args)
        {
            if (!test_result)
            {
                test_result = args.result_;
            }
            else
            {
                SYNTROPY_ASSERT(*test_result != TestResult::kSkipped);                  // A skip message must not be followed by any other message.
                SYNTROPY_ASSERT(args.result_ != TestResult::kSkipped);                  // A skip message must not be preceded by any other message.

                test_result = std::max(*test_result, args.result_);
            }

            // Relay the event as if it was originated within the test case.

            on_result_notified_.Notify(*this, OnResultNotifiedEventArgs{ args.result_, args.message_, args.location_ });
        });

        auto on_message_notified = fixture.OnMessageNotified().Subscribe([this](auto& /*sender*/, auto& args)
        {
            // Relay the event as if if was originated within the test case.

            on_message_notified_.Notify(*this, OnMessageNotifiedEventArgs{ args.message_ });
        });

        fixture.Before();                                                               // Setup the fixture.

        try
        {
            test_case_(fixture);                                                        // Will notify all sorts of test results.
        }
        catch (...)
        {
            // Unhandled exceptions must be notified as if they where generated inside the test case. #TODO Trace the error?

            SYNTROPY_ASSERT(!test_result || *test_result != TestResult::kSkipped);      // A test case must safely return after being skipped!

            test_result = TestResult::kError;

            on_result_notified_.Notify(*this, OnResultNotifiedEventArgs{ *test_result, "Unhandled exception", SYNTROPY_HERE });
        }

        fixture.After();                                                                // Tear down the fixture.

        auto result = test_result ? *test_result : TestResult::kSuccess;

        on_finished_.Notify(*this, OnFinishedEventArgs{ result });

        return result;
    }

    const Observable<const TestCase&, const TestCase::OnStartedEventArgs&>& TestCase::OnStarted() const
    {
        return on_started_;
    }

    const Observable<const TestCase&, const TestCase::OnFinishedEventArgs&>& TestCase::OnFinished() const
    {
        return on_finished_;
    }

    const Observable<const TestCase&, const TestCase::OnResultNotifiedEventArgs&>& TestCase::OnResultNotified() const
    {
        return on_result_notified_;
    }

    const Observable<const TestCase&, const TestCase::OnMessageNotifiedEventArgs&>& TestCase::OnMessageNotified() const
    {
        return on_message_notified_;
    }

}