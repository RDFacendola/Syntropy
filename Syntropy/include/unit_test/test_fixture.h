
/// \file test_fixture.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test fixtures and unit test macros.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <string>

#include "patterns/observable.h"

#include "unit_test/test.h"

#include "diagnostics/diagnostics.h"

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
    } \
    else \
    { \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_EXPECT(" #expression ")", SYNTROPY_HERE }); \
    } \

/// \brief Unit test macro. The test is executed if "expression" is true, otherwise the test is skipped.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_PRECONDITION(!IsServer());
#define SYNTROPY_UNIT_PRECONDITION(expression) \
    if(bool result = (expression); !result) \
    { \
        NotifyResult({ syntropy::TestResult::kSkipped, "SYNTROPY_UNIT_PRECONDITION(" #expression ")", SYNTROPY_HERE }); \
        return; \
    } \
    else \
    { \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_PRECONDITION(" #expression ")", SYNTROPY_HERE }); \
    } \

/// \brief Unit test macro. The test is successful if "expression" doesn't throw, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \remarks This will only check exceptions thrown manually via "throw" (access violation is *not* an exception, for instance).
#define SYNTROPY_UNIT_NO_THROW(expression) \
    try \
    { \
        (expression); \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_NO_THROW(" #expression ")", SYNTROPY_HERE }); \
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
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_THROW_ANY(" #expression ")", SYNTROPY_HERE }); \
    }

namespace syntropy
{

    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    /// \brief Base class for test fixtures. Represents a stateful environment for multiple test cases.
    /// \author Raffaele D. Facendola - January 2018
    class TestFixture
    {
    public:

        /// \brief Arguments of the event called whenever a test case result is notified.
        struct OnResultNotifiedEventArgs
        {
            TestResult result_;                                 ///< \brief Result.

            std::string message_;                               ///< \brief Result message.

            diagnostics::StackTraceElement location_;           ///< \brief Code that issued the result.
        };

        /// \brief Create a new test fixture.
        /// Use this method to setup any fixture state before all test cases.
        TestFixture() = default;

        /// \brief Default virtual destructor.
        /// Use this method to tear down any fixture state after all test cases.
        virtual ~TestFixture() = default;

        /// \brief Used to setup fixture state before each test case.
        virtual void Before();

        /// \brief Used to tear-down fixture state after each test case.
        virtual void After();

        /// \brief Event called whenever a test result is being notified.
        Observable<TestFixture&, const OnResultNotifiedEventArgs&>& OnResultNotified();

    protected:

        /// \brief Notify a test case result.
        /// \param result Result to notify.
        void NotifyResult(const OnResultNotifiedEventArgs& result);

    private:

        Event<TestFixture&, const OnResultNotifiedEventArgs&> on_result_notified_;          ///< \brief Event whenever a test result is being notified.
    };

}