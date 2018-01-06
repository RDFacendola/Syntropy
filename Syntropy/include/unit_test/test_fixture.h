
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

/// \brief Unit test macro. Report a success if "expression" is true, otherwise report a failure and return.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_ASSERT(1 + 2 == 3);
#define SYNTROPY_UNIT_ASSERT(expression) \
    if (bool result = (expression); !result) \
    { \
        NotifyResult({syntropy::TestResult::kFailure, "SYNTROPY_UNIT_ASSERT(" #expression ")", SYNTROPY_HERE}); \
        return; \
    } \
    else \
    { \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_ASSERT(" #expression ")", SYNTROPY_HERE }); \
    }

/// \brief Unit test macro. Report a success if "expression" is true, otherwise report a failure.
/// Similar to SYNTROPY_UNIT_ASSERT but doesn't return on failure.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_CHECK(1 + 2 == 3);
#define SYNTROPY_UNIT_CHECK(expression) \
    if (bool result = (expression); !result) \
    { \
        NotifyResult({syntropy::TestResult::kFailure, "SYNTROPY_UNIT_CHECK(" #expression ")", SYNTROPY_HERE}); \
    } \
    else \
    { \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_CHECK(" #expression ")", SYNTROPY_HERE }); \
    }

/// \brief Unit test macro. The test is executed if "expression" is true, otherwise the test is skipped. If used, it must precede any other test.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_EXPECT(!IsServer());
#define SYNTROPY_UNIT_EXPECT(expression) \
    if(bool result = (expression); !result) \
    { \
        NotifyResult({ syntropy::TestResult::kSkipped, "SYNTROPY_UNIT_EXPECT(" #expression ")", SYNTROPY_HERE }); \
        return; \
    } \
    else \
    { \
        NotifyResult({ syntropy::TestResult::kSuccess, "SYNTROPY_UNIT_EXPECT(" #expression ")", SYNTROPY_HERE }); \
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