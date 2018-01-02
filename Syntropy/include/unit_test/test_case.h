
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to handle single test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <string>
#include <functional>

#include "containers/hashed_string.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RESULT                                                          */
    /************************************************************************/

    /// \brief Result of a test.
    enum class TestResult : uint8_t
    {
        kSuccess,                       ///< \brief The test succeeded.
        kFailure,                       ///< \brief The test failed.
        kError,                         ///< \brief The test threw unhandled exceptions.
        kSkipped,                       ///< \brief The test was skipped.
    };

    /// \brief Stream insertion for TestResult.
    std::ostream& operator<<(std::ostream& out, TestResult test_result);

    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    /// \brief Represents a named test case.
    /// \author Raffaele D. Facendola - December 2017
    class TestCase
    {
    public:

        /// \brief Create a new test case.
        /// \param name Name of the test case.
        /// \param test_case Function bound to this test case.
        template <typename TTestCase>
        TestCase(HashedString name, TTestCase test_case)
            : name_(std::move(name))
            , test_case_(std::move(test_case))
        {

        }

        /// \brief Get the test case name.
        /// \return Return the test case name.
        const HashedString& GetName() const;

        /// \brief Run the test case.
        void Run() const;

    private:

        HashedString name_;                             ///< \brief Name of the test case.

        std::function<void(void)> test_case_;           ///< \brief Test case function.
    };
}
