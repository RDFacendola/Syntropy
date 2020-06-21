
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
        /// \brief Report a test case result in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename... TMessage>
        void ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message);

        /// \brief Report a test case message in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename... TMessage>
        void ReportTestCaseMessage(TMessage&&... message);
    }

    /************************************************************************/
    /* ON TEST CONTEXT RESULT EVENT ARGS                                    */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test result is reported in a test context.
    struct OnTestContextResultEventArgs
    {
        /// \brief Test result.
        TestResult result_;

        /// \brief Message.
        String message_;

        /// \brief Code location that issued the result.
        StackTrace location_;
    };

    /************************************************************************/
    /* ON TEST CONTEXT MESSAGE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported in a test context.
    struct OnTestContextMessageEventArgs
    {
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
        template <typename... TMessage>
        friend void UnitTest::ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message);

        template <typename... TMessage>
        friend void UnitTest::ReportTestCaseMessage(TMessage&&... message);

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

        /// \brief Bind to the event notified whenever a result is reported.
        template <typename TDelegate>
        Listener OnResult(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a message is reported.
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate) const;

    private:

        /// \brief Report a result.
        void ReportResult(TestResult test_result, const String& test_message, const StackTrace& test_location) const;

        /// \brief Report a message.
        void ReportMessage(const String& test_message) const;

        /// \brief Active test context.
        static thread_local inline ObserverPtr<TestContext> context_{ nullptr };

        /// \brief Previous test context to restore upon destruction.
        ObserverPtr<TestContext> previous_context_{ nullptr };

        /// \brief Event notified whenever a test result is reported.
        Event<const TestContext&, OnTestContextResultEventArgs> result_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<const TestContext&, OnTestContextMessageEventArgs> message_event_;

        /// \brief Test 
        Label name_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTest.

    template <typename... TMessage>
    inline void UnitTest::ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        TestContext::context_->ReportResult(test_result, builder.str(), test_location);
    }

    template <typename... TMessage>
    inline void UnitTest::ReportTestCaseMessage(TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        TestContext::context_->ReportMessage(builder.str());
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
    inline Listener TestContext::OnResult(TDelegate&& delegate) const
    {
        return result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestContext::OnMessage(TDelegate&& delegate) const
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestContext::ReportResult(TestResult test_result, const String& test_message, const StackTrace& test_location) const
    {
        result_event_.Notify(*this, { test_result, test_message, test_location });
    }

    inline void TestContext::ReportMessage(const String& message) const
    {
        message_event_.Notify(*this, { message });
    }

}
