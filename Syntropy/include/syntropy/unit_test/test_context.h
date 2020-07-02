
/// \file test_context.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define test contexts.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/language/event.h"
#include "syntropy/language/type_traits.h"

#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// \brief Exposes test context-related functionalities used to notify results and messages from within a test context.
    /// \author Raffaele D. Facendola - May 2020
    namespace UnitTest
    {
        /// \brief Report a test case success in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename TExpression, typename TResult>
        void ReportSuccess(const StackTrace& test_location, TExpression&& expression, TResult&& result);

        /// \brief Report a test case failure in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename TExpression, typename TResult, typename TExpected>
        void ReportFailure(const StackTrace& test_location, TExpression&& expression, TResult&& result, TExpected&& expected);

        /// \brief Report a test case skip in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename TReason>
        void ReportSkipped(const StackTrace& test_location, TReason&& reason);

        /// \brief Report a test case message in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename... TMessage>
        void ReportMessage(const StackTrace& test_location, TMessage&&... message);
    }

    /************************************************************************/
    /* ON TEST CONTEXT SUCCESS EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test success is reported in a test context.
    struct OnTestContextSuccessEventArgs
    {
        /// \brief Code location that issued the result.
        StackTrace location_;

        /// \brief Tested expression.
        String expression_;

        /// \brief Actual result.
        String result_;
    };

    /************************************************************************/
    /* ON TEST CONTEXT FAILURE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test failure is reported in a test context.
    struct OnTestContextFailureEventArgs
    {
        /// \brief Code location that issued the result.
        StackTrace location_;

        /// \brief Tested expression.
        String expression_;

        /// \brief Actual result.
        String result_;

        /// \brief Expected result.
        String expected_;
    };

    /************************************************************************/
    /* ON TEST CONTEXT SKIPPED EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test is skipped in a test context.
    struct OnTestContextSkippedEventArgs
    {
        /// \brief Code location that issued the result.
        StackTrace location_;

        /// \brief Skip reason.
        String reason_;
    };


    /************************************************************************/
    /* ON TEST CONTEXT MESSAGE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported in a test context.
    struct OnTestContextMessageEventArgs
    {
        /// \brief Code location generated the message.
        StackTrace location_;

        /// \brief Reported message.
        String message_;
    };

    /************************************************************************/
    /* TEST ENVIRONMENT                                                     */
    /************************************************************************/

    /// \brief Represents a stateless context for multiple test cases.
    /// When a context is created it becomes active in the current scope. Contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - January 2018
    class TestContext
    {
        template <typename TExpression, typename TResult>
        friend void UnitTest::ReportSuccess(const StackTrace& test_location, TExpression&& expression, TResult&& result);

        template <typename TExpression, typename TResult, typename TExpected>
        friend void UnitTest::ReportFailure(const StackTrace& test_location, TExpression&& expression, TResult&& result, TExpected&& expected);

        template <typename TReason>
        friend void UnitTest::ReportSkipped(const StackTrace& test_location, TReason&& reason);

        template <typename... TMessage>
        friend void UnitTest::ReportMessage(const StackTrace& test_location, TMessage&&... message);

    public:

        /// \brief Create a new test context.
        TestContext();

        /// \brief Default copy-constructor.
        TestContext(const TestContext&) = default;

        /// \brief Default move-constructor.
        TestContext(TestContext&&) = default;

        /// \brief Default copy-assignment.
        TestContext& operator=(const TestContext&) = default;

        /// \brief Default move-assignment.
        TestContext& operator=(TestContext&&) = default;

        /// \brief Default virtual destructor.
        ~TestContext();

        /// \brief Bind to the event notified whenever a success is reported.
        template <typename TDelegate>
        Listener OnSuccess(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a failure is reported.
        template <typename TDelegate>
        Listener OnFailure(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a skip is reported.
        template <typename TDelegate>
        Listener OnSkipped(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a message is reported.
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate) const;

    private:

        /// \brief Report a test case success.
        void ReportSuccess(const StackTrace& location, const String& expression, const String& result) const;

        /// \brief Report a test case failure.
        void ReportFailure(const StackTrace& location, const String& expression, const String& result, const String& expected) const;

        /// \brief Report a skipped test case.
        void ReportSkipped(const StackTrace& location, const String& reason) const;

        /// \brief Report a message.
        void ReportMessage(const StackTrace& location, const String& message) const;

        /// \brief Active test context.
        static thread_local inline ObserverPtr<TestContext> context_{ nullptr };

        /// \brief Previous test context to restore upon destruction.
        ObserverPtr<TestContext> previous_context_{ nullptr };

        /// \brief Event notified whenever a test success is reported.
        Event<const TestContext&, OnTestContextSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a test failure is reported.
        Event<const TestContext&, OnTestContextFailureEventArgs> failure_event_;

        /// \brief Event notified whenever a test skip is reported.
        Event<const TestContext&, OnTestContextSkippedEventArgs> skipped_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<const TestContext&, OnTestContextMessageEventArgs> message_event_;

        /// \brief Test 
        Label name_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTest.

    template <typename TExpression, typename TResult>
    void UnitTest::ReportSuccess(const StackTrace& test_location, TExpression&& expression, TResult&& result)
    {
        auto expression_stream = OStringStream{};
        auto result_stream = OStringStream{};

        expression_stream << expression;
        result_stream << result;

        TestContext::context_->ReportSuccess(test_location, expression_stream.str(), result_stream.str());
    }

    template <typename TExpression, typename TResult, typename TExpected>
    void UnitTest::ReportFailure(const StackTrace& test_location, TExpression&& expression, TResult&& result, TExpected&& expected)
    {
        auto expression_stream = OStringStream{};
        auto result_stream = OStringStream{};
        auto expected_stream = OStringStream{};

        expression_stream << expression;
        result_stream << result;
        expected_stream << expected;

        TestContext::context_->ReportFailure(test_location, expression_stream.str(), result_stream.str(), expected_stream.str());
    }

    template <typename TReason>
    inline void UnitTest::ReportSkipped(const StackTrace& test_location, TReason&& reason)
    {
        auto reason_stream = OStringStream{};

        reason_stream << reason;

        TestContext::context_->ReportSkipped(test_location, reason_stream.str());
    }

    template <typename... TMessage>
    inline void UnitTest::ReportMessage(const StackTrace& test_location, TMessage&&... message)
    {
        auto stream = OStringStream{};

        (stream << ... << message);

        TestContext::context_->ReportMessage(test_location, stream.str());
    }

    // TestContext.

    inline TestContext::TestContext()
        : previous_context_(context_)
    {
        context_ = this;
    }

    inline TestContext::~TestContext()
    {
        context_ = previous_context_;
    }

    template <typename TDelegate>
    inline Listener TestContext::OnSuccess(TDelegate&& delegate) const
    {
        return success_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestContext::OnFailure(TDelegate&& delegate) const
    {
        return failure_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestContext::OnSkipped(TDelegate&& delegate) const
    {
        return skipped_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestContext::OnMessage(TDelegate&& delegate) const
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestContext::ReportSuccess(const StackTrace& location, const String& expression, const String& result) const
    {
        success_event_.Notify(*this, { location, expression, result });
    }

    inline void TestContext::ReportFailure(const StackTrace& location, const String& expression, const String& result, const String& expected) const
    {
        failure_event_.Notify(*this, { location, expression, result, expected });
    }

    inline void TestContext::ReportSkipped(const StackTrace& location, const String& reason) const
    {
        skipped_event_.Notify(*this, { location, reason });
    }

    inline void TestContext::ReportMessage(const StackTrace& location, const String& message) const
    {
        message_event_.Notify(*this, { location, message });
    }

}
