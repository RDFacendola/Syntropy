
/// \file test_context.h
///
/// \brief This header is part of the Syntropy diagnostics module.
///        It contains classes used to define test contexts.
///
/// \author Raffaele D. Facendola - 2018.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/strings/string.h"
#include "syntropy/core/toolset/event.h"
#include "syntropy/diagnostics/foundation/source_location.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if expression is equal to
    ///        expected, otherwise report a failure and continue.
    ///
    /// \usage SYNTROPY_UNIT_EQUAL(1 + 2, 3);
    #define SYNTROPY_UNIT_EQUAL(expression, expected) \
        SYNTROPY_MACRO_DECLARATION(expression, expected)

    /// \brief Unit test macro: report a success if type is the same type as
    ///        expected, otherwise report a failure and continue.
    #define SYNTROPY_UNIT_SAME(type, expected) \
       SYNTROPY_MACRO_DECLARATION(type, expected)

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Report a test case success in the active test context.
    ///
    /// \remarks Undefined behavior if no active context.
    template <typename TExpression>
    void ReportSuccess(Immutable<SourceLocation> location,
                       Forwarding<TExpression> expression) noexcept;

    /// \brief Report a test case failure in the active test context.
    ///
    /// \remarks Undefined behavior if no active context.
    template <typename TExpression, typename TResult, typename TExpected>
    void ReportFailure(Immutable<SourceLocation> location,
                       Forwarding<TExpression> expression,
                       Forwarding<TResult> result,
                       Forwarding<TExpected> expected) noexcept;

    /************************************************************************/
    /* ON TEST CONTEXT SUCCESS EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the test success event.
    struct OnTestContextSuccessEventArgs
    {
        /// \brief Code location that issued the result.
        SourceLocation location_;

        /// \brief Tested expression.
        String expression_;
    };

    /************************************************************************/
    /* ON TEST CONTEXT FAILURE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the test failure event.
    struct OnTestContextFailureEventArgs
    {
        /// \brief Code location that issued the result.
        SourceLocation location_;

        /// \brief Tested expression.
        String expression_;

        /// \brief Expression result.
        String result_;

        /// \brief Expected result.
        String expected_;
    };

    /************************************************************************/
    /* TEST CONTEXT                                                         */
    /************************************************************************/

    /// \brief A stateless context for multiple test cases.
    ///        When a context is created it becomes active in the current
    ///        scope. Contexts can be nested but overlapping results in
    ///        undefined behavior.
    /// \author Raffaele D. Facendola - January 2018
    class TestContext
    {
        template <typename TExpression>
        friend void
        ReportSuccess(Immutable<SourceLocation> location,
                      Forwarding<TExpression> expression) noexcept;

        template <typename TExpression, typename TResult, typename TExpected>
        friend void
        ReportFailure(Immutable<SourceLocation> location,
                      Forwarding<TExpression> expression,
                      Forwarding<TResult> result,
                      Forwarding<TExpected> expected) noexcept;

    public:

        /// \brief Create a new test context.
        TestContext() noexcept;

        /// \brief Default copy-constructor.
        TestContext(Immutable<TestContext> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestContext(Movable<TestContext> rhs) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestContext>
        operator=(Immutable<TestContext> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestContext>
        operator=(Movable<TestContext> rhs) noexcept /*= default*/;

        /// \brief Destructor.
        ~TestContext() noexcept;

        /// \brief Bind to the success event.
        template <typename TDelegate>
        Listener
        OnSuccess(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the failure event.
        template <typename TDelegate>
        Listener
        OnFailure(Forwarding<TDelegate> delegate) const noexcept;

    private:

        /// \brief Type of an event in a context.
        template <typename TEventArgs>
        using EventType = Event<Immutable<TestContext>, TEventArgs>;

        /// \brief Report a test case success.
        void ReportSuccess(Immutable<SourceLocation> location,
                           Immutable<String> expression) const noexcept;

        /// \brief Report a test case failure.
        void ReportFailure(Immutable<SourceLocation> location,
                           Immutable<String> expression,
                           Immutable<String> result,
                           Immutable<String> expected) const noexcept;

        /// \brief Active test context.
        static thread_local inline
        RWPtr<TestContext> context_{ nullptr };

        /// \brief Previous test context to restore upon destruction.
        RWPtr<TestContext> previous_context_{ nullptr };

        /// \brief Event notified whenever a test success is reported.
        EventType<OnTestContextSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a test failure is reported.
        EventType<OnTestContextFailureEventArgs> failure_event_;

    };

}

// ===========================================================================

#include "details/test_context.inl"

// ===========================================================================
