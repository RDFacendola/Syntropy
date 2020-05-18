
/// \file test_result.h
/// \brief This header is part of the syntropy unit test system. It contains base definitions for unit test results.
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
        kSkipped = 0u,

        /// \brief The test succeeded.
        kSuccess = 1u,

        /// \brief The test failed.
        kFailure = 2u,

        /// \brief The test threw an unhandled exception.
        kError = 3u,
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

        return out;
    }

}
