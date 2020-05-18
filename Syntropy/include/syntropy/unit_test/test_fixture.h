
/// \file test_fixture.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test fixtures and unit test macros.
/// A test fixture is a stateful environment for multiple test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/language/event.h"
#include "syntropy/unit_test/test_result.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure and return.
    /// \usage SYNTROPY_UNIT_ASSERT(1 + 2 == 3);                // Inside a TestFixture.
    #define SYNTROPY_UNIT_ASSERT(expression) \
        if (bool result = (expression); !result) \
        { \
            NotifyResult({syntropy::TestResult::kFailure, "ASSERT (" #expression ")", SYNTROPY_HERE}); \
            return; \
        } \
        else \
        { \
            NotifyResult({ syntropy::TestResult::kSuccess, "ASSERT (" #expression ")", SYNTROPY_HERE }); \
        }

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure.
    /// Similar to SYNTROPY_UNIT_ASSERT except it doesn't return on failure.
    /// \usage SYNTROPY_UNIT_TEST(1 + 2 == 3);                  // Inside a TestFixture.
    #define SYNTROPY_UNIT_TEST(expression) \
        if (bool result = (expression); !result) \
        { \
            NotifyResult({syntropy::TestResult::kFailure, "TEST (" #expression ")", SYNTROPY_HERE}); \
        } \
        else \
        { \
            NotifyResult({ syntropy::TestResult::kSuccess, "TEST (" #expression ")", SYNTROPY_HERE }); \
        }

    /// \brief Unit test macro: the test is executed if "expression" is true, otherwise the test is skipped. If used, it must precede any other test.
    /// \usage SYNTROPY_UNIT_EXPECT(!IsServer());                   // Inside a TestFixture
    #define SYNTROPY_UNIT_EXPECT(expression) \
        if(bool result = (expression); !result) \
        { \
            NotifyResult({ syntropy::TestResult::kSkipped, "EXPECT (" #expression ")", SYNTROPY_HERE }); \
            return; \
        } \
        else \
        { \
            NotifyResult({ syntropy::TestResult::kSuccess, "EXPECT (" #expression ")", SYNTROPY_HERE }); \
        }

    /// \brief Unit test macro: macro used to manually skip a test case.
    /// \usage SYNTROPY_UNIT_SKIP("Work in progress");              // Inside a TestFixture.
    #define SYNTROPY_UNIT_SKIP(reason) \
        { \
            NotifyResult({syntropy::TestResult::kSkipped, "SKIP (" #reason ")", SYNTROPY_HERE }); \
            return; \
        }

    /// \brief Unit test macro: notify a message for the current test case being ran.
    /// \usage SYNTROPY_UNIT_MESSAGE("This is a message ", 2 + 3);   // Inside a TestFixture will output "This is a message 5".
    #define SYNTROPY_UNIT_MESSAGE(...) \
        { \
            NotifyMessage(__VA_ARGS__) \
        }

    /// \brief Unit test macro: execute "expression" and trace it as a message.
    /// \usage SYNTROPY_UNIT_TRACE(auto x = y + z)                  // Inside a TestFixture, will output "auto x = y + z".
    #define SYNTROPY_UNIT_TRACE(expression) \
        { \
            (expression); \
            NotifyMessage(#expression) \
        }

    /************************************************************************/
    /* ON TEST RESULT EVENT ARGS                                            */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case result is reported.
    struct OnTestResultEventArgs
    {
        /// \brief Test result.
        TestResult result_;

        /// \brief Message.
        String message_;

        /// \brief Code location that issued the result.
        StackTraceElement location_;
    };

    /************************************************************************/
    /* ON TEST MESSAGE EVENT ARGS                                           */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported.
    struct OnTestMessageEventArgs
    {
        /// \brief Reported message.
        String message_;
    };

    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    /// \brief Base class for test fixtures. Represents a stateful environment for multiple test cases.
    /// \author Raffaele D. Facendola - January 2018
    class TestFixture
    {
    public:

        /// \brief Create a new test fixture.
        /// Constructor is used to setup an fixture state before all test cases being run.
        TestFixture() = default;

        /// \brief Default virtual destructor.
        /// Destructor is used to tear-down any fixture state after all test cases being run.
        virtual ~TestFixture() = default;

        /// \brief Used to setup fixture state before each test case.
        virtual void Before();

        /// \brief Used to tear-down fixture state after each test case.
        virtual void After();

        /// \brief Bind to the event notified whenever a test result is reported.
        template <typename TDelegate>
        Listener OnTestResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test message is reported.
        template <typename TDelegate>
        Listener OnTestMessage(TDelegate&& delegate);

    protected:

        /// \brief Report a test case result.
        /// \brief This method is not expected to be called directly, only via unit test macros.
        void ReportResult(const OnTestResultEventArgs& result);

        /// \brief Report a test case message.
        /// \brief This method is not expected to be called directly, only via unit test macros.
        template <typename... TMessage>
        void ReportMessage(TMessage&&... message);

    private:

        /// \brief Event notified whenever a test result is reported.
        Event<TestFixture*, OnTestResultEventArgs> test_result_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<TestFixture*, OnTestMessageEventArgs> test_message_event_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestFixture.

    inline void TestFixture::Before()
    {

    }

    inline void TestFixture::After()
    {

    }

    template <typename TDelegate>
    inline Listener TestFixture::OnTestResult(TDelegate&& delegate)
    {
        return test_result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestFixture::OnTestMessage(TDelegate&& delegate)
    {
        return test_message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestFixture::ReportResult(const OnTestResultEventArgs& result)
    {
        test_result_event_.Notify(this, result);
    }

    template <typename... TMessage>
    inline void TestFixture::ReportMessage(TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        test_message_event_.Notify(this, OnTestMessageEventArgs{ builder.str() });
    }

}
