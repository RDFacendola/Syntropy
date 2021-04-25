
/// \file test_runner.h
///
/// \brief This header is part of the Syntropy diagnostics module.
///        It contains classes used to run test suites.
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"
#include "syntropy/core/support/event.h"

#include "syntropy/diagnostics/unit_test/auto_test_suite.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* ON TEST RUNNER SUITE STARTED EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments of for the event notified whenever a test suite starts.
    struct OnTestRunnerSuiteStartedEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE FINISHED EVENT ARGS                             */
    /************************************************************************/

    /// \brief Arguments for the test suite end event.
    struct OnTestRunnerSuiteFinishedEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER CASE STARTED EVENT ARGS                               */
    /************************************************************************/

    /// \brief Arguments of the test suite start event.
    struct OnTestRunnerCaseStartedEventArgs : OnTestSuiteCaseStartedEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER CASE FINISHED EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments of the test case end event.
    struct OnTestRunnerCaseFinishedEventArgs : OnTestSuiteCaseFinishedEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE SUCCESS EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the test case success event.
    struct OnTestRunnerCaseSuccessEventArgs : OnTestSuiteCaseSuccessEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE FAILURE EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the test case failure event.
    struct OnTestRunnerCaseFailureEventArgs : OnTestSuiteCaseFailureEventArgs
    {
        /// \brief Test suite name.
        String test_suite_;
    };

    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    class TestRunner
    {
    public:

        /// \brief Default constructor.
        TestRunner() noexcept = default;

        /// \brief Default copy-constructor.
        TestRunner(Immutable<TestRunner> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestRunner(Movable<TestRunner> rhs) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestRunner>
        operator=(Immutable<TestRunner> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestRunner>
        operator=(Movable<TestRunner> rhs) noexcept;

        /// \brief Default destructor.
        ~TestRunner() noexcept = default;

        /// \brief Run all test suites.
        void
        Run() const noexcept;

        /// \brief Bind to the test suite start event.
        template <typename TDelegate>
        Listener
        OnSuiteStarted(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the test suite finish event.
        template <typename TDelegate>
        Listener
        OnSuiteFinished(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the test case start event.
        template <typename TDelegate>
        Listener
        OnCaseStarted(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the test case finish event.
        template <typename TDelegate>
        Listener
        OnCaseFinished(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the test case success event.
        template <typename TDelegate>
        Listener
        OnCaseSuccess(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the test case failure event.
        template <typename TDelegate>
        Listener
        OnCaseFailure(Forwarding<TDelegate> delegate) const noexcept;

    private:

        /// \brief Type of an event in a test runner.
        template <typename TEventArgs>
        using EventType = Event<Immutable<TestRunner>, Immutable<TEventArgs>>;

        /// \brief Run a test suite.
        void
        Run(Immutable<TestSuite> test_suite) const noexcept;

        /// \brief Event notified whenever a test suite starts.
        EventType<OnTestRunnerSuiteStartedEventArgs> suite_started_event_;

        /// \brief Event notified whenever a test suite finishes.
        EventType<OnTestRunnerSuiteFinishedEventArgs> suite_finished_event_;

        /// \brief Event notified whenever a test case starts.
        EventType<OnTestRunnerCaseStartedEventArgs> case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        EventType<OnTestRunnerCaseFinishedEventArgs> case_finished_event_;

        /// \brief Event notified whenever a success is reported.
        EventType<OnTestRunnerCaseSuccessEventArgs> case_success_event_;

        /// \brief Event notified whenever a failure is reported.
        EventType<OnTestRunnerCaseFailureEventArgs> case_failure_event_;

    };

}

// ===========================================================================

#include "details/test_runner.inl"

// ===========================================================================
