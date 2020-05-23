
/// \file test_report.h
/// \brief This header is part of the Syntropy unit test module. It contains base definitions for unit test reports.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <iostream>
#include <ios>

#include "syntropy/core/label.h"
#include "syntropy/unit_test/test_result.h"
#include "syntropy/diagnostics/stack_trace.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST REPORT                                                          */
    /************************************************************************/

    /// \brief Represents a synthetic test report.
    /// \author Raffaele D. Facendola - May 2020.
    struct TestReport
    {
        /// \brief Synthetic test report name.
        Label name_;

        /// \brief Stack trace at first test location.
        StackTrace start_trace_;

        /// \brief Stack trace at last test location.
        StackTrace end_trace_;

        /// \brief Number of tests that were skipped.
        std::int64_t skipped_count_{ 0 };

        /// \brief Number of tests that succeeded.
        std::int64_t success_count_{ 0 };

        /// \brief Number of tests that failed.
        std::int64_t fail_count_{ 0 };
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// \brief Exposes generic unit test functionalities.
    namespace UnitTest
    {
        /// \brief Determine the overall result of a report.
        TestResult GetResult(const TestReport& test_report);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new named test.
    TestReport MakeTestReport(const Label& name);

    /// \brief Add a result to a test report.
    TestReport& operator+=(TestReport& lhs, TestResult rhs);

    /// \brief Add a stack trace to a test report.
    TestReport& operator+=(TestReport& lhs, const StackTrace& rhs);

    /// \brief Add a test report to another test report.
    TestReport& operator+=(TestReport& lhs, const TestReport& rhs);

    /// \brief Stream insertion for TestResult.
    std::ostream& operator<<(std::ostream& out, const TestReport& test_report);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Unit test.

    inline TestResult UnitTest::GetResult(const TestReport& test_report)
    {
        if (test_report.fail_count_ > 0)
        {
            return TestResult::kFailure;    // At least one test failed.
        }
        else 
        {
            return TestResult::kSuccess;    // Success! Empty and skipped test are implicitly considered successful.
        }
    }

    // Non-member functions.

    inline TestReport MakeTestReport(const Label& name)
    {
        auto test_report = TestReport{};

        test_report.name_ = name;

        return test_report;
    }

    inline TestReport& operator+=(TestReport& lhs, TestResult rhs)
    {
        if (rhs == TestResult::kSkipped)
        {
            ++lhs.skipped_count_;
        }
        else if (rhs == TestResult::kSuccess)
        {
            ++lhs.success_count_;
        }
        else if (rhs == TestResult::kFailure)
        {
            ++lhs.fail_count_;
        }
        return lhs;
    }

    inline TestReport& operator+=(TestReport& lhs, const StackTrace& rhs)
    {
        // Only keep start trace location (if not already set) and the end trace.

        if (!lhs.start_trace_)
        {
            lhs.start_trace_ = rhs;
        }

        lhs.end_trace_ = rhs;

        return lhs;
    }

    inline TestReport& operator+=(TestReport& lhs, const TestReport& rhs)
    {
        lhs.skipped_count_ += rhs.skipped_count_;
        lhs.success_count_ += rhs.success_count_;
        lhs.fail_count_ += rhs.fail_count_;

        if (!lhs.start_trace_)
        {
            lhs.start_trace_ = rhs.start_trace_;
        }

        lhs.end_trace_ = rhs.end_trace_;

        return lhs;
    }

    inline std::ostream& operator<<(std::ostream& out, const TestReport& test_report)
    {
        out << "Test report '" << test_report.name_ << "': \n";
        out << "   Failures: " << test_report.fail_count_ << "\n";
        out << "   Skipped: " << test_report.skipped_count_ << "\n";
        out << "   Success: " << test_report.success_count_ << "\n";
        out << "Result: " << std::uppercase << UnitTest::GetResult(test_report) << "\n";

        return out;
    }

}
