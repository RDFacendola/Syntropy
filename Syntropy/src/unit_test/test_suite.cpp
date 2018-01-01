#include "unit_test/test_suite.h"

#include "time/timer.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    TestSuite::OnFinishedEventArgs& TestSuite::OnFinishedEventArgs::operator+=(TestResult result)
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

    const Context& TestSuite::GetName() const
    {
        return fixture_->GetName();
    }

    const std::vector<TestCase>& TestSuite::GetTestCases() const
    {
        return fixture_->GetTestCases();
    }

    void TestSuite::Run(const Context& context)
    {
        on_started_.Notify(*this);

        OnFinishedEventArgs report;

        HighResolutionTimer<std::chrono::milliseconds> timer(true);

        // Skip the suite if its context is not contained in the provided one.

        if (context.Contains(GetName()))
        {
            // Guarded run: exception thrown below cause the rest of the suite to be aborted since the test fixture is left in an undefined state.
            // Note: won't handle undefined behaviors (such as division by zero, access violation, etc.)

            try
            {
                fixture_->BeforeAll();

                for (auto&& test_case : GetTestCases())
                {
                    RunTestCase(test_case);

                    report += fixture_->GetLastResult().result_;
                }

                fixture_->AfterAll();
            }
            catch (const std::exception& exception)
            {
                report.result_ = TestResult::kError;
                report.message_ = exception.what();
            }
            catch (...)
            {
                report.result_ = TestResult::kError;
                report.message_ = "Unhandled exception while running the test suite.";
            }
        }
        else
        {
            report.result_ = TestResult::kSkipped;
        }

        report.duration_ = timer.Stop();

        on_finished_.Notify(*this, report);
    }

    void TestSuite::RunTestCase(const TestCase& test_case)
    {
        fixture_->Before();

        on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ &test_case });

        OnTestCaseFinishedEventArgs report;

        HighResolutionTimer<std::chrono::milliseconds> timer(true);

        report.test_case_ = &test_case;
        
        // Guarded run: unhandled exception cause the test case to fail. The rest of the test suite is executed normally.
        // Note: won't handle against undefined behaviors (such as division by zero, access violation, etc.)

        try
        {
            fixture_->ClearLastResult();

            test_case.Run();

            report.result_ = fixture_->GetLastResult();

        }
        catch (const std::exception& exception)
        {
            report.result_ = { TestResult::kError , exception.what(), SYNTROPY_HERE };
        }
        catch (...)
        {
            report.result_ = { TestResult::kError , "Unhandled exception while running the test case.", SYNTROPY_HERE };
        }

        report.duration_ = timer.Stop();

        on_test_case_finished_.Notify(*this, report);

        fixture_->After();
    }

    Observable<TestSuite&>& TestSuite::OnStarted()
    {
        return on_started_;
    }

    Observable<TestSuite&, const TestSuite::OnFinishedEventArgs&>& TestSuite::OnFinished()
    {
        return on_finished_;
    }

    Observable<TestSuite&, const TestSuite::OnTestCaseStartedEventArgs&>& TestSuite::OnTestCaseStarted()
    {
        return on_test_case_started_;
    }

    Observable<TestSuite&, const TestSuite::OnTestCaseFinishedEventArgs&>& TestSuite::OnTestCaseFinished()
    {
        return on_test_case_finished_;
    }

}