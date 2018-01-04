#include "unit_test/test_suite.h"

#include "time/timer.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST SUITE RESULT                                                    */
    /************************************************************************/

    TestSuiteResult& TestSuiteResult::operator+=(TestResult result)
    {
        // Update counters.

        ++count_;

        if (result == TestResult::kSuccess)
        {
            ++success_count_;
        }
        else if (result == TestResult::kFailure)
        {
            ++failure_count_;
        }
        else if (result == TestResult::kError)
        {
            ++error_count_;
        }
        else if (result == TestResult::kSkipped)
        {
            ++skip_count_;
        }

        // Errors and failures cause the suite to fail.

        if (result_ == TestResult::kSuccess && (result == TestResult::kFailure || result == TestResult::kError))
        {
            result_ = TestResult::kFailure;
        }

        return *this;
    }

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

    TestSuiteResult TestSuite::Run(const Context& context) const
    {
        TestSuiteResult result;

        if (context.Contains(GetName()))
        {
            // Unhandled exception cause the test suite to be aborted since the fixture may be left in an undefined state. This won't guard against undefined behaviors (access violation, division by zero, etc.)

            try
            {
                auto fixture = generate_fixture_();

                for (auto&& test_case : fixture->GetTestCases())
                {
                    result += Run(*fixture, test_case);
                }
            }
            catch (const std::exception& exception)
            {
                result.result_ = TestResult::kError;
                result.message_ = exception.what();
            }
            catch (...)
            {
                result.result_ = TestResult::kError;
                result.message_ = "Unhandled exception while running the test suite.";
            }
        }
        else
        {
            // Skip the suite if its context doesn't match.

            result.result_ = TestResult::kSkipped;
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