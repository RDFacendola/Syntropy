
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

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if "expression" is equal to expected, otherwise report a failure and continue.
    /// \usage SYNTROPY_UNIT_EQUAL(1 + 2, 3);
    #define SYNTROPY_UNIT_EQUAL(expression, expected) \
        SYNTROPY_MACRO_DECLARATION(expression, expected)

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// \brief Exposes test context-related functionalities used to notify results and messages from within a test context.
    /// \author Raffaele D. Facendola - May 2020
    namespace UnitTest
    {
        /// \brief Report a test case success in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename TExpression>
        void ReportSuccess(const StackTrace& test_location, TExpression&& expression);

        /// \brief Report a test case failure in the current active test context.
        /// If no context is active, the behavior of this method is undefined.
        template <typename TExpression, typename TResult, typename TExpected>
        void ReportFailure(const StackTrace& test_location, TExpression&& expression, TResult&& result, TExpected&& expected);
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

        /// \brief Expression result.
        String result_;

        /// \brief Expected result.
        String expected_;
    };

    /************************************************************************/
    /* TEST ENVIRONMENT                                                     */
    /************************************************************************/

    /// \brief Represents a stateless context for multiple test cases.
    /// When a context is created it becomes active in the current scope. Contexts can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - January 2018
    class TestContext
    {
        template <typename TExpression>
        friend void UnitTest::ReportSuccess(const StackTrace& test_location, TExpression&& expression);

        template <typename TExpression, typename TResult, typename TExpected>
        friend void UnitTest::ReportFailure(const StackTrace& test_location, TExpression&& expression, TResult&& result, TExpected&& expected);

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

    private:

        /// \brief Report a test case success.
        void ReportSuccess(const StackTrace& location, const String& expression) const;

        /// \brief Report a test case failure.
        void ReportFailure(const StackTrace& location, const String& expression, const String& result, const String& expected) const;

        /// \brief Active test context.
        static thread_local inline ObserverPtr<TestContext> context_{ nullptr };

        /// \brief Previous test context to restore upon destruction.
        ObserverPtr<TestContext> previous_context_{ nullptr };

        /// \brief Event notified whenever a test success is reported.
        Event<const TestContext&, OnTestContextSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a test failure is reported.
        Event<const TestContext&, OnTestContextFailureEventArgs> failure_event_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Unit-test macros.

    #undef SYNTROPY_UNIT_EQUAL
    #define SYNTROPY_UNIT_EQUAL(expression, expected) \
        if (auto&& result = (expression); result != expected) \
        { \
            syntropy::UnitTest::ReportFailure( SYNTROPY_HERE, #expression, result, expected ); \
        } \
        else \
        { \
            syntropy::UnitTest::ReportSuccess( SYNTROPY_HERE, #expression ); \
        }

    // UnitTest.

    template <typename TExpression>
    void UnitTest::ReportSuccess(const StackTrace& test_location, TExpression&& expression)
    {
        auto expression_stream = OStringStream{};

        expression_stream << expression;

        TestContext::context_->ReportSuccess(test_location, expression_stream.str());
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

    inline void TestContext::ReportSuccess(const StackTrace& location, const String& expression) const
    {
        success_event_.Notify(*this, { location, expression });
    }

    inline void TestContext::ReportFailure(const StackTrace& location, const String& expression, const String& result, const String& expected) const
    {
        failure_event_.Notify(*this, { location, expression, result, expected });
    }

}
