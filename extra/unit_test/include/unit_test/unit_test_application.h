
/// \file unit_test_application.h
///
/// \brief This header is part of the Syntropy unit test application.
///        It contains base definitions to run unit tests.
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/support/event.h"

#include "syntropy/diagnostics/unit_test/test_runner.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* UNIT TEST APPLICATION                                                */
    /************************************************************************/

    /// \brief Represents the Syntropy Unit Test application.
    /// \author Raffaele D. Facendola - June 2020.
    class UnitTestApplication
    {
    public:

        /// \brief Create a new application.
        UnitTestApplication() noexcept;

        /// \brief No copy constructor.
        UnitTestApplication(Syntropy::Immutable<UnitTestApplication> rhs)
            = delete;

        /// \brief No move constructor.
        UnitTestApplication(Syntropy::Movable<UnitTestApplication> rhs)
            = delete;

        /// \brief No copy-assignment operator.
        Syntropy::Mutable<UnitTestApplication>
        operator=(Syntropy::Immutable<UnitTestApplication> rhs) = delete;

        /// \brief No move-assignment operator.
        Syntropy::Mutable<UnitTestApplication>
        operator=(Syntropy::Movable<UnitTestApplication> rhs) = delete;

        /// \brief Destructor.
        ~UnitTestApplication() noexcept = default;

        /// \brief Application entry point.
        Syntropy::Int Run() noexcept;

    private:

        /// \brief Called whenever a test suite starts.
        void
        OnSuiteStarted(Syntropy::Immutable<Syntropy::TestRunner> sender,
                       Syntropy::Immutable<Syntropy::OnTestRunnerSuiteStartedEventArgs> event_args)
        noexcept;

        /// \brief Called whenever a test case starts.
        void OnCaseStarted(Syntropy::Immutable<Syntropy::TestRunner> sender,
                           Syntropy::Immutable<Syntropy::OnTestRunnerCaseStartedEventArgs> event_args)
        noexcept;

        /// \brief Called whenever a test case failure is reported.
        void OnCaseFailure(Syntropy::Immutable<Syntropy::TestRunner> sender,
                           Syntropy::Immutable<Syntropy::OnTestRunnerCaseFailureEventArgs> event_args)
        noexcept;

        /// \brief Called whenever a test case finishes.
        void OnCaseFinished(Syntropy::Immutable<Syntropy::TestRunner> sender,
                            Syntropy::Immutable<Syntropy::OnTestRunnerCaseFinishedEventArgs> event_args)
        noexcept;

        /// \brief Called whenever a test suite finishes.
        void OnSuiteFinished(Syntropy::Immutable<Syntropy::TestRunner> sender,
                             Syntropy::Immutable<Syntropy::OnTestRunnerSuiteFinishedEventArgs> event_args)
        noexcept;

        /// \brief Test runner.
        Syntropy::TestRunner test_runner_;

        /// \brief Listener for the test runner events.
        Syntropy::Listener test_listener_;

        /// \brief Total number of test suites tested.
        Syntropy::Int total_test_suites_{ 0 };

        /// \brief Number of test suites containing zero failed test cases.
        Syntropy::Int passed_test_suites_{ 0 };

        /// \brief Number of test suites containing at least one failed test case.
        Syntropy::Int failed_test_suites_{ 0 };

        /// \brief Total test cases tested.
        Syntropy::Int total_test_cases_{ 0 };

        /// \brief Number of passed test cases.
        Syntropy::Int passed_test_cases_{ 0 };

        /// \brief Number of failed test cases.
        Syntropy::Int failed_test_cases_{ 0 };

        /// \brief Whether the current test suite contains at least one failed test case.
        Syntropy::Bool test_suite_failed_{ false };

        /// \brief Whether the current test case contains at least one failure.
        Syntropy::Bool test_case_failed_{ false };

    };

}


// ===========================================================================

#include "details/unit_test_application.inl"

// ===========================================================================
