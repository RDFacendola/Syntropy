
/// \file test_fixture.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define test fixtures and unit test macros.
/// A test fixture is a stateful environment for multiple test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/language/event.h"
#include "syntropy/language/macro.h"
#include "syntropy/unit_test/test_result.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure and return. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_ASSERT(1 + 2 == 3);
    #define SYNTROPY_UNIT_ASSERT(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure. Expected usage within a TestFixture.
    /// Similar to SYNTROPY_UNIT_ASSERT except it doesn't return on failure.
    /// \usage SYNTROPY_UNIT_TEST(1 + 2 == 3);
    #define SYNTROPY_UNIT_TEST(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: the test is executed if "expression" is true, otherwise the test is skipped. If used, it must precede any other test. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_EXPECT(!IsServer());
    #define SYNTROPY_UNIT_EXPECT(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: macro used to manually skip a test case. Expected usage within a TestFixture before any other test.
    /// \usage SYNTROPY_UNIT_SKIP("Work in progress");
    #define SYNTROPY_UNIT_SKIP(reason) \
        SYNTROPY_MACRO_DECLARATION(reason)

    /// \brief Unit test macro: notify a message for the current test case being ran. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_MESSAGE("This is a message ", 2 + 3);
    #define SYNTROPY_UNIT_MESSAGE(...) \
        SYNTROPY_MACRO_DECLARATION(__VA_ARGS__)

    /// \brief Unit test macro: execute "expression" and trace it as a message. Expected usage within a TestFixture.
    #define SYNTROPY_UNIT_TRACE(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /************************************************************************/
    /* ON TEST FIXTURE RESULT EVENT ARGS                                    */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test result is reported in the fixture.
    struct OnTestFixtureResultEventArgs
    {
        /// \brief Test result.
        TestResult result_;

        /// \brief Message.
        String message_;

        /// \brief Code location that issued the result.
        StackTrace location_;
    };

    /************************************************************************/
    /* ON TEST FIXTURE MESSAGE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported in the fixture.
    struct OnTestFixtureMessageEventArgs
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

        /// \brief Bind to the event notified whenever a result is reported.
        template <typename TDelegate>
        Listener OnResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a message is reported.
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate);

    protected:

        /// \brief Report a result.
        /// \brief This method is not expected to be called directly, only via unit test macros.
        void ReportResult(const OnTestFixtureResultEventArgs& result);

        /// \brief Report a message.
        /// \brief This method is not expected to be called directly, only via unit test macros.
        template <typename... TMessage>
        void ReportMessage(TMessage&&... message);

    private:

        /// \brief Event notified whenever a test result is reported.
        Event<TestFixture*, OnTestFixtureResultEventArgs> result_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<TestFixture*, OnTestFixtureMessageEventArgs> message_event_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Unit-test macros.

    #undef SYNTROPY_UNIT_ASSERT
    #define SYNTROPY_UNIT_ASSERT(expression) \
        if (bool result = (expression); !result) \
        { \
            ReportResult({syntropy::TestResult::kFailure, "ASSERT (" #expression ")", SYNTROPY_HERE}); \
            return; \
        } \
        else \
        { \
            ReportResult({ syntropy::TestResult::kSuccess, "ASSERT (" #expression ")", SYNTROPY_HERE }); \
        }

    #undef SYNTROPY_UNIT_TEST
    #define SYNTROPY_UNIT_TEST(expression) \
        if (bool result = (expression); !result) \
        { \
            ReportResult({syntropy::TestResult::kFailure, "TEST (" #expression ")", SYNTROPY_HERE}); \
        } \
        else \
        { \
            ReportResult({ syntropy::TestResult::kSuccess, "TEST (" #expression ")", SYNTROPY_HERE }); \
        }

    #undef SYNTROPY_UNIT_EXPECT
    #define SYNTROPY_UNIT_EXPECT(expression) \
        if(bool result = (expression); !result) \
        { \
            ReportResult({ syntropy::TestResult::kSkipped, "EXPECT (" #expression ")", SYNTROPY_HERE }); \
            return; \
        } \
        else \
        { \
            ReportResult({ syntropy::TestResult::kSuccess, "EXPECT (" #expression ")", SYNTROPY_HERE }); \
        }

    #undef SYNTROPY_UNIT_SKIP
    #define SYNTROPY_UNIT_SKIP(reason) \
        { \
            ReportResult({syntropy::TestResult::kSkipped, "SKIP (" #reason ")", SYNTROPY_HERE }); \
            return; \
        }

    #undef SYNTROPY_UNIT_MESSAGE
    #define SYNTROPY_UNIT_MESSAGE(...) \
        { \
            ReportMessage(__VA_ARGS__); \
        }

    #undef SYNTROPY_UNIT_TRACE
    #define SYNTROPY_UNIT_TRACE(expression) \
        { \
            (expression); \
            ReportMessage(#expression); \
        }

    // TestFixture.

    inline void TestFixture::Before()
    {

    }

    inline void TestFixture::After()
    {

    }

    template <typename TDelegate>
    inline Listener TestFixture::OnResult(TDelegate&& delegate)
    {
        return result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestFixture::OnMessage(TDelegate&& delegate)
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestFixture::ReportResult(const OnTestFixtureResultEventArgs& result)
    {
        result_event_.Notify(this, result);
    }

    template <typename... TMessage>
    inline void TestFixture::ReportMessage(TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        message_event_.Notify(this, { builder.str() });
    }

}
