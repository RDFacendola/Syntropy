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

    const Context& TestSuite::GetName() const
    {
        return fixture_->GetName();
    }

    const std::vector<TestCase>& TestSuite::GetTestCases() const
    {
        return fixture_->GetTestCases();
    }

    std::vector<TestCase>& TestSuite::GetTestCases()
    {
        return fixture_->GetTestCases();
    }

    TestSuiteResult TestSuite::Run(const Context& context)
    {
        TestSuiteResult result;

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

                    result += fixture_->GetLastResult().result_;
                }

                fixture_->AfterAll();
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
            result.result_ = TestResult::kSkipped;
        }

        return result;
    }

    void TestSuite::RunTestCase(TestCase& test_case)
    {
        fixture_->Before();

        on_test_case_started_.Notify(*this, OnTestCaseStartedEventArgs{ &test_case });

        OnTestCaseFinishedEventArgs result;

        HighResolutionTimer<std::chrono::milliseconds> timer(true);

        result.test_case_ = &test_case;
        
        // Guarded run: unhandled exception cause the test case to fail. The rest of the test suite is executed normally.
        // Note: won't handle against undefined behaviors (such as division by zero, access violation, etc.)

        try
        {
            fixture_->ClearLastResult();

            test_case.Run();

            result.result_ = fixture_->GetLastResult();

        }
        catch (const std::exception& exception)
        {
            result.result_ = { TestResult::kError , exception.what(), SYNTROPY_HERE };
        }
        catch (...)
        {
            result.result_ = { TestResult::kError , "Unhandled exception while running the test case.", SYNTROPY_HERE };
        }

        result.duration_ = timer.Stop();

        on_test_case_finished_.Notify(*this, result);

        fixture_->After();
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