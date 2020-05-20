
/// \file test_result.h
/// \brief This header is part of the Syntropy unit test module. It contains base definitions for unit test results.
///
/// \author Raffaele D. Facendola - 2018

#include <iostream>

#pragma once

namespace syntropy
{
    /************************************************************************/
    /* TEST RESULT                                                          */
    /************************************************************************/

    /// \brief Result of a test.
    enum class TestResult : int8_t
    {
        /// \brief The test was skipped.
        kSkipped = 0,

        /// \brief The test succeeded.
        kSuccess = 1,

        /// \brief The test failed.
        kFailure = 2,

        /// \brief The test threw an unhandled exception.
        kError = 3,

        /// \brief The test is invalid.
        kInvalid = 4,
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Stream insertion for TestResult.
    std::ostream& operator<<(std::ostream& out, TestResult test_result);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    inline std::ostream& operator<<(std::ostream& out, TestResult test_result)
    {
        if (test_result == TestResult::kSkipped)
        {
            out << "Skipped";
        }
        else if (test_result == TestResult::kSuccess)
        {
            out << "Success";
        }
        else if (test_result == TestResult::kFailure)
        {
            out << "Failure";
        }
        else if (test_result == TestResult::kError)
        {
            out << "Error";
        }
        else if (test_result == TestResult::kInvalid)
        {
            out << "Invalid";
        }

        return out;
    }

}
