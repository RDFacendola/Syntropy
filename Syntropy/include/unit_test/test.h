
/// \file test.h
/// \brief This header is part of the syntropy unit test system. It contains base classes and enumeration for unit tests.
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
    /// Sorted by severity.
    enum class TestResult : uint8_t
    {
        kSkipped = 0u,                  ///< \brief The test was skipped.
        kSuccess = 1u,                  ///< \brief The test succeeded.
        kFailure = 2u,                  ///< \brief The test failed.
        kError = 3u,                    ///< \brief The test threw unhandled exceptions.
    };

    /// \brief Stream insertion for TestResult.
    std::ostream& operator<<(std::ostream& out, TestResult test_result);
}
