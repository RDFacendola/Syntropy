
/// \file test_fixture.h
/// \brief This header is part of the syntropy unit test system. It contains classes and interfaces used to define test fixtures as well as assertion macros.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <vector>

#include "unit_test/test_case.h"

#include "diagnostics/diagnostics.h"

#include "containers/context.h"
#include "containers/hashed_string.h"

/************************************************************************/
/* UNIT TEST MACROS                                                     */
/************************************************************************/

/// \brief Unit test macro. The test is successful if "expression" is true, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_TEST_EXPECT(1 + 2 == 3);
#define SYNTROPY_TEST_EXPECT(expression) \
    if (bool result = (expression); !result) \
    { \
        SetLastResult({syntropy::TestResult::kFailure, "SYNTROPY_TEST_EXPECT(" #expression ")", SYNTROPY_HERE}); \
        return; \
    }

/// \brief Unit test macro. The test is executed if "expression" is true, otherwise the test is skipped.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_TEST_PRECONDITION(!IsServer());
#define SYNTROPY_TEST_PRECONDITION(expression) \
    if(bool result = (expression); !result) \
    { \
        SetLastResult({ syntropy::TestResult::kSkipped, "SYNTROPY_TEST_PRECONDITION(" #expression ")", SYNTROPY_HERE }); \
        return; \
    }

/// \brief Unit test macro. The test is successful if "expression" doesn't throw, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \remarks This will only check exceptions thrown manually via "throw" (access violation is *not* an exception, for instance).
#define SYNTROPY_TEST_NO_THROW(expression) \
    try \
    { \
        (expression); \
    } \
    catch (...) \
    { \
        SetLastResult({ syntropy::TestResult::kFailure, "SYNTROPY_TEST_NO_THROW(" #expression ")", SYNTROPY_HERE }); \
        return; \
    }

/// \brief Unit test macro. The test is successful if "expression" throws any exception, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \remarks This will only check exceptions thrown manually via "throw" (access violation is *not* an exception, for instance).
#define SYNTROPY_TEST_THROW_ANY(expression) \
    try \
    { \
        (expression); \
        SetLastResult({ syntropy::TestResult::kFailure, "SYNTROPY_TEST_THROW_ANY(" #expression ")", SYNTROPY_HERE }); \
        return; \
    } \
    catch (...) \
    { \
        \
    }

namespace syntropy
{

    /************************************************************************/
    /* TEST CASE RESULT                                                     */
    /************************************************************************/

    /// \brief Test case result.
    /// \author Raffaele D. Facendola - January 2018
    struct TestCaseResult
    {
        TestResult result_{ TestResult::kSuccess };         ///< \brief Result of the test case.

        std::string message_;                               ///< \brief Message of the test case.

        diagnostics::StackTraceElement location_;           ///< \brief Code location the result refers to.
    };

    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    /// \brief Base class for test suites. Represents a stateful environment for multiple test cases.
    /// \author Raffaele D. Facendola - January 2018
    class TestFixture
    {
    public:

        /// \brief Create a new test fixture.
        /// \param name Name of the test fixture.
        TestFixture(const Context& name);

        /// \brief Get the test fixture name.
        /// \return Returns the test fixture name.
        const Context& GetName() const;

        /// \brief Get the test cases.
        /// \return Returns the test cases.
        const std::vector<TestCase>& GetTestCases() const;

        /// \brief Get the test cases.
        /// \return Returns the test cases.
        std::vector<TestCase>& GetTestCases();

        /// \brief Get the result of the last test case.
        /// If no test case was run the behavior is undefined.
        /// \return Returns the result of the last test case.
        TestCaseResult GetLastResult() const;

        /// \brief Clear the last result.
        void ClearLastResult();

        /// \brief Used to setup fixture state before all test cases.
        virtual void BeforeAll();

        /// \brief Used to tear-down fixture state after all test cases.
        virtual void AfterAll();

        /// \brief Used to setup fixture state before each test case.
        virtual void Before();

        /// \brief Used to tear-down fixture state after each test case.
        virtual void After();

    protected:

        /// \brief Declare a new test case.
        /// \param name Name of the test case.
        /// \param test_case Test case function.
        template <typename TTestCase>
        void DeclareTestCase(const HashedString& name, TTestCase test_case)
        {
            test_cases_.push_back({ name, test_case });
        }

        /// \brief Set the result of the last test case.
        /// \param result Result to set.
        void SetLastResult(TestCaseResult result);

    private:

        Context name_;                              ///< \brief Fixture name.

        std::vector<TestCase> test_cases_;          ///< \brief List of test cases.

        TestCaseResult test_result_;                ///< \brief Result of the latest test case.
    };

}