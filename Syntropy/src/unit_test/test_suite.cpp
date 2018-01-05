#include "unit_test/test_suite.h"

#include "time/timer.h"

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
        return fixture_->GetTestCases();
    }

    TestResult TestSuite::Run(const Context& context) const
    {
        TestResult result{ TestResult::kSkipped };

        if (context.Contains(GetName()))
        {
            try
            {
                auto fixture = generate_fixture_();

                for (auto&& test_case : fixture->GetTestCases())
                {
                    auto test_result = Run(*fixture, test_case);

                    result = std::max(result, test_result);
                }
            }
            catch (...)
            {
                result = std::max(result, TestResult::kError);          // Abort the rest since the fixture may be left in an undefined state.
            }
        }

        return result;
    }

    TestResult TestSuite::Run(TestFixture& fixture, const TestCase& test_case) const
    {
        on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ &test_case });

        fixture.Before();

        // Unhandled exceptions cause this test case to fail. This won't guard against undefined behaviors (access violation, division by zero, etc.)

        HighResolutionTimer<std::chrono::milliseconds> timer(true);

        OnTestCaseFinishedEventArgs result;

        auto on_result = fixture.OnResult().Subscribe([&result](const TestFixture& /*sender*/, const TestFixture::OnResultEventArgs& args)
        {
            result.result_ = args.result_;
        });

        try
        {
            test_case.Run();
        }
        catch (const std::exception& exception)
        {
            result.result_ = { TestResult::kError , exception.what(), SYNTROPY_HERE };
        }
        catch (...)
        {
            result.result_ = { TestResult::kError , "Unhandled exception while running the test case.", SYNTROPY_HERE };
        }

        result.test_case_ = &test_case;
        result.duration_ = timer.Stop();

        fixture.After();

        on_test_case_finished_.Notify(*this, result);

        return result.result_.result_;      // Yep, I got it.
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