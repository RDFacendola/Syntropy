
/// \file test_case.h
///
/// \brief This header is part of the Syntropy diagnostics module.
///        It contains classes used to define test cases.
///
/// \author Raffaele D. Facendola - 2018

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"
#include "syntropy/core/support/event.h"

#include "syntropy/diagnostics/unit_test/test_context.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* ON TEST CASE SUCCESS EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the test-case success event.
    struct OnTestCaseSuccessEventArgs : OnTestContextSuccessEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE FAILURE EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the test-case failure event.
    struct OnTestCaseFailureEventArgs : OnTestContextFailureEventArgs
    {

    };

    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    /// \brief Represents an environment for a single test case.
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture>
    class TestCase
    {
    public:

        /// \brief Create a named test case.
        TestCase(Immutable<String> name) noexcept;

        /// \brief Default copy-constructor.
        TestCase(Immutable<TestCase> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestCase(Mutable<TestCase> rhs) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestCase>
        operator=(Immutable<TestCase> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestCase>
        operator=(Mutable<TestCase> rhs) noexcept = default;

        /// \brief Default virtual destructor.
        virtual
        ~TestCase() noexcept = default;

        /// \brief Run the test case within a fixture.
        void
        Run(Mutable<TTestFixture> test_fixture) const noexcept;

        /// \brief Get the test case name.
        Immutable<String>
        GetName() const noexcept;

        /// \brief Bind to the event notified whenever a success is reported.
        template <typename TDelegate>
        Listener
        OnSuccess(Forwarding<TDelegate> delegate) const noexcept;

        /// \brief Bind to the event notified whenever a failure is reported.
        template <typename TDelegate>
        Listener
        OnFailure(Forwarding<TDelegate> delegate) const noexcept;

    private:

        /// \brief Type of an event in a test case.
        template <typename TEventArgs>
        using EventType = Event<Immutable<TestCase>, Immutable<TEventArgs>>;

        /// \brief Run the concrete test case.
        virtual void
        RunTestCase(Mutable<TTestFixture> test_fixture) const noexcept = 0;

        /// \brief Test case name.
        String name_;

        /// \brief Event notified whenever a success is reported.
        EventType<OnTestCaseSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a failure is reported.
        EventType<OnTestCaseFailureEventArgs> failure_event_;

    };

    /// \brief Wraps a concrete test case method.
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture, typename TTestCase>
    class TestCaseT : public TestCase<TTestFixture>
    {
    public:

        /// \brief Create a named test case.
        template <typename UTestCase>
        TestCaseT(Immutable<String> name, Forwarding<UTestCase> test_case)
        noexcept;

        /// \brief Default copy-constructor.
        TestCaseT(Immutable<TestCaseT> rhs) noexcept = default;

        /// \brief Default move-constructor.
        TestCaseT(Movable<TestCaseT> rhs) noexcept = default;

        /// \brief Default copy-assignment.
        Mutable<TestCaseT>
        operator=(Immutable<TestCaseT> rhs) noexcept = default;

        /// \brief Default move-assignment.
        Mutable<TestCaseT>
        operator=(Movable<TestCaseT> rhs) noexcept = default;

        /// \brief Default virtual destructor.
        virtual
        ~TestCaseT() noexcept = default;

    private:

        virtual void
        RunTestCase(Mutable<TTestFixture> test_fixture)
        const noexcept override;

        /// \brief Test case method.
        TTestCase test_case_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new test case by deducing templates from arguments.
    template <typename TTestFixture, typename TTestCase>
    TestCaseT<TTestFixture, TTestCase>
    MakeTestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
    noexcept;

}

// ===========================================================================

#include "details/test_case.inl"

// ===========================================================================
