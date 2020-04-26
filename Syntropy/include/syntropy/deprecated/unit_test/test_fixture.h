
/// \file test_fixture.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test fixtures and unit test macros.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/types/string.h"
#include "syntropy/types/sstream.h"

#include "syntropy/patterns/observable.h"

#include "syntropy/unit_test/test.h"

#include "syntropy/diagnostics/diagnostics.h"

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

/// \brief Unit test macro. Macro used to manually skip a test case.
/// Must be called within a TestFixture.
/// \usage SYNTROPY_UNIT_SKIP("Work in progress");
#define SYNTROPY_UNIT_SKIP(reason) \
    if (bool result = (reason); result){ \
        NotifyResult({syntropy::TestResult::kSkipped, "SYNTROPY_UNIT_SKIP(" #reason ")", SYNTROPY_HERE }); \
        return; \
    }

/// \brief Unit test macro. Notify a message for the current test case being ran.
#define SYNTROPY_UNIT_MESSAGE(...) \
    NotifyMessage(__VA_ARGS__);

/// \brief Unit test macro. Execute "expression" and trace it as a message.
#define SYNTROPY_UNIT_TRACE(expression) \
    expression; \
    NotifyMessage(#expression)\

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

            String message_;                               ///< \brief Result message.

            diagnostics::StackTraceElement location_;           ///< \brief Code that issued the result.
        };

        /// \brief Arguments of the event called whenever a test case notifies a message.
        struct OnMessageNotifiedEventArgs
        {
            String message_;                               ///< \brief Notified message.
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

        /// \brief Event called whenever a test result is notified.
        Observable<TestFixture&, const OnResultNotifiedEventArgs&>& OnResultNotified();

        /// \brief Event called whenever a message is notified.
        Observable<TestFixture&, const OnMessageNotifiedEventArgs&>& OnMessageNotified();

    protected:

        /// \brief Notify a test case result.
        /// \param result Result to notify.
        void NotifyResult(const OnResultNotifiedEventArgs& result);

        /// \brief Notify a message.
        /// \param message Message to notify.
        template <typename... TMessage>
        void NotifyMessage(TMessage&&... message)
        {
            OStringStream builder;

            (builder << ... << message);

            on_message_notified_.Notify(*this, OnMessageNotifiedEventArgs{ builder.str() });
        }

    private:

        Event<TestFixture&, const OnResultNotifiedEventArgs&> on_result_notified_;          ///< \brief Event triggered whenever a test result is notified.

        Event<TestFixture&, const OnMessageNotifiedEventArgs&> on_message_notified_;        ///< \brief Event triggered whenever a message is notified.
    };

}