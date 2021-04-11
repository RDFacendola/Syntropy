
/// \file test_suite.h
///
/// \brief This header is part of the Syntropy diagnostics module.
///        It contains classes used to define test suites.
///
/// \author Raffaele D. Facendola - 2018

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* ON TEST SUITE CASE STARTED EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the test case start event.
    struct OnTestSuiteCaseStartedEventArgs
    {
        /// \brief Test case name.
        String test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE FINISHED EVENT ARGS                               */
    /************************************************************************/

    /// \brief Arguments for the test case finish event.
    struct OnTestSuiteCaseFinishedEventArgs
    {
        /// \brief Test case name.
        String test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE SUCCESS EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the test case success event.
    struct OnTestSuiteCaseSuccessEventArgs : OnTestCaseSuccessEventArgs
    {
        /// \brief Test case name.
        String test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE FAILURE EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the test case failure event.
    struct OnTestSuiteCaseFailureEventArgs : OnTestCaseFailureEventArgs
    {
        /// \brief Test case name.
        String test_case_;
    };

    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    /// \brief Base class for test suites.
    ///
    /// A test suite is used to execute all test cases in a given test fixture.
    ///
    /// \author Raffaele D. Facendola - May 2020
    class TestSuite
    {
    public:

        /// \brief Create a new named test suite.
        TestSuite(Immutable<String> name) noexcept;

        /// \brief Default copy-constructor.
        TestSuite(Immutable<TestSuite> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestSuite(Movable<TestSuite>) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestSuite>
        operator=(Immutable<TestSuite> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestSuite>
        operator=(Movable<TestSuite> rhs) noexcept;

        /// \brief Default virtual destructor.
        virtual
        ~TestSuite() noexcept = default;

        /// \brief Get the test suite name.
        Immutable<String>
        GetName() const noexcept;

        /// \brief Run all test cases in the suite.
        virtual void
        Run() const noexcept = 0;

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

    protected:

        /// \brief Notify the start of a test case.
        void
        NotifyCaseStarted(
            Immutable<OnTestSuiteCaseStartedEventArgs> event_args) const;

        /// \brief Notify the end of a test case.
        void
        NotifyCaseFinished(
            Immutable<OnTestSuiteCaseFinishedEventArgs> event_args) const;

        /// \brief Notify a success within a test case.
        void
        NotifyCaseSuccess(
            Immutable<OnTestSuiteCaseSuccessEventArgs> event_args) const;

        /// \brief Notify a failure within a test case.
        void
        NotifyCaseFailure(
            Immutable<OnTestSuiteCaseFailureEventArgs> event_args) const;

    private:

        /// \brief Type of an event in a test suite.
        template <typename TEventArgs>
        using EventType = Event<Immutable<TestSuite>, TEventArgs>;

        /// \brief Test suite name.
        String name_;

        /// \brief Event notified whenever a test case starts.
        EventType<OnTestSuiteCaseStartedEventArgs> case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        EventType<OnTestSuiteCaseFinishedEventArgs> case_finished_event_;

        /// \brief Event notified whenever a success is reported.
        EventType<OnTestSuiteCaseSuccessEventArgs> case_success_event_;

        /// \brief Event notified whenever a failure is reported.
        EventType<OnTestSuiteCaseFailureEventArgs> case_failure_event_;

    };

    /// \brief Test suite for a test fixture.
    /// \author Raffaele D. Facendola - May 2020
    template <typename TTestFixture>
    class TestSuiteT : public TestSuite
    {
    public:

        /// \brief Create a new test suite.
        template <typename... TArguments>
        TestSuiteT(Immutable<String> name,
                   Forwarding<TArguments>... arguments);

        /// \brief Default copy-constructor.
        TestSuiteT(Immutable<TestSuiteT> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestSuiteT(Movable<TestSuiteT> rhs) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestSuiteT>
        operator=(Immutable<TestSuiteT> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestSuiteT>
        operator=(Movable<TestSuiteT> rhs) noexcept = default;

        /// \brief Default virtual destructor.
        virtual
        ~TestSuiteT() noexcept = default;

        virtual
        void Run() const noexcept override;

    private:

        /// \brief Run a test case.
        void Run(Immutable<TestCase<TTestFixture>> test_case) const;

        /// \brief Underlying test fixture.
        /// The fixture is not considered part of the external interface:
        /// test cases are either const or have to preserve the immutable state
        /// of the fixture via After and Before methods (which are required to
        /// be non-const).
        mutable TTestFixture test_fixture_;

    };

}
