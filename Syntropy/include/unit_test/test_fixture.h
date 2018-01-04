
/// \file test_fixture.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test fixtures and unit test macros.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <vector>

#include "patterns/observable.h"

#include "unit_test/test_case.h"

#include "diagnostics/diagnostics.h"

#include "containers/context.h"
#include "containers/hashed_string.h"

/************************************************************************/
/* UNIT TEST MACROS                                                     */
/************************************************************************/

/// \brief Unit test macro. The test is successful if "expression" is true, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_EXPECT(1 + 2 == 3);
#define SYNTROPY_UNIT_EXPECT(expression) \
    if (bool result = (expression); !result) \
    { \
        NotifyResult({syntropy::TestResult::kFailure, "SYNTROPY_UNIT_EXPECT(" #expression ")", SYNTROPY_HERE}); \
        return; \
    }

/// \brief Unit test macro. The test is executed if "expression" is true, otherwise the test is skipped.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_PRECONDITION(!IsServer());
#define SYNTROPY_UNIT_PRECONDITION(expression) \
    if(bool result = (expression); !result) \
    { \
        NotifyResult({ syntropy::TestResult::kSkipped, "SYNTROPY_UNIT_PRECONDITION(" #expression ")", SYNTROPY_HERE }); \
        return; \
    }

/// \brief Unit test macro. The test is successful if "expression" doesn't throw, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \remarks This will only check exceptions thrown manually via "throw" (access violation is *not* an exception, for instance).
#define SYNTROPY_UNIT_NO_THROW(expression) \
    try \
    { \
        (expression); \
    } \
    catch (...) \
    { \
        NotifyResult({ syntropy::TestResult::kFailure, "SYNTROPY_UNIT_NO_THROW(" #expression ")", SYNTROPY_HERE }); \
        return; \
    }

/// \brief Unit test macro. The test is successful if "expression" throws any exception, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \remarks This will only check exceptions thrown manually via "throw" (access violation is *not* an exception, for instance).
#define SYNTROPY_UNIT_THROW_ANY(expression) \
    try \
    { \
        (expression); \
        NotifyResult({ syntropy::TestResult::kFailure, "SYNTROPY_UNIT_THROW_ANY(" #expression ")", SYNTROPY_HERE }); \
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

    /// \brief Base class for test fixtures. Represents a stateful environment for multiple test cases.
    /// \author Raffaele D. Facendola - January 2018
    class TestFixture
    {
    public:

        /// \brief Arguments for the OnResult event.
        struct OnResultEventArgs
        {
            TestCaseResult result_;                                             ///< \brief Result of the test case.
        };

        /// \brief Create a new test fixture.
        /// Use this method to setup any fixture state before all test cases.
        TestFixture() = default;

        /// \brief Default virtual destructor.
        /// Use this method to tear down any fixture state after all test cases.
        virtual ~TestFixture() = default;

        /// \brief Get the test cases.
        /// \return Returns the test cases.
        const std::vector<TestCase>& GetTestCases() const;

        /// \brief Used to setup fixture state before each test case.
        virtual void Before();

        /// \brief Used to tear-down fixture state after each test case.
        virtual void After();

        /// \brief Event called whenever a test result is being notified.
        Observable<TestFixture&, const OnResultEventArgs&>& OnResult();

    protected:

        /// \brief Declare a new test case.
        /// \param name Name of the test case.
        /// \param test_case Test case function.
        template <typename TTestCase>
        void DeclareTestCase(const HashedString& name, TTestCase test_case)
        {
            test_cases_.push_back({ name, test_case });
        }

        /// \brief Notify a test case result.
        /// \param result Result to notify.
        void NotifyResult(TestCaseResult result);

    private:

        std::vector<TestCase> test_cases_;                                  ///< \brief List of test cases.

        Event<TestFixture&, const OnResultEventArgs&> on_result_;           ///< \brief Event whenever a test result is being notified.
    };

}