
/// \file test_runner.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to run test suites.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/context.h"

#include "syntropy/language/event.h"

#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_suite.h"
#include "syntropy/unit_test/auto_test_suite.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST RUNNER SUITE STARTED EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments of for the event notified whenever a test suite starts.
    struct OnTestRunnerSuiteStartedEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE FINISHED EVENT ARGS                             */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test suite ends.
    struct OnTestRunnerSuiteFinishedEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;

        /// \brief Synthetic test suite report.
        TestReport test_report_;
    };

    /************************************************************************/
    /* ON TEST RUNNER CASE STARTED EVENT ARGS                               */
    /************************************************************************/

    /// \brief Arguments of the event notified whenever a test case starts.
    struct OnTestRunnerCaseStartedEventArgs : OnTestSuiteCaseStartedEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER CASE FINISHED EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments of the event notified whenever a test case ends.
    struct OnTestRunnerCaseFinishedEventArgs : OnTestSuiteCaseFinishedEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE SUCCESS EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case success is reported.
    struct OnTestRunnerCaseSuccessEventArgs : OnTestSuiteCaseSuccessEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE FAILURE EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case failure is reported.
    struct OnTestRunnerCaseFailureEventArgs : OnTestSuiteCaseFailureEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE SKIPPED EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case is skipped.
    struct OnTestRunnerCaseSkippedEventArgs : OnTestSuiteCaseSkippedEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* ON TEST RUNNER SUITE MESSAGE EVENT ARGS                              */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported.
    struct OnTestRunnerCaseMessageEventArgs : OnTestSuiteCaseMessageEventArgs
    {
        /// \brief Test suite name.
        Context test_suite_;
    };

    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    class TestRunner
    {
    public:

        /// \brief Default constructor.
        TestRunner() = default;

        /// \brief Default copy-constructor.
        TestRunner(const TestRunner&) = default;

        /// \brief Default move-constructor.
        TestRunner(TestRunner&&) = default;

        /// \brief Default copy-assignment.
        TestRunner& operator=(const TestRunner&) = default;

        /// \brief Default move-assignment.
        TestRunner& operator=(TestRunner&&) = default;

        /// \brief Default destructor.
        ~TestRunner() = default;

        /// \brief Run all test suites with a matching context.
        TestReport Run(const Context& context) const;

        /// \brief Bind to the event notified whenever a test suite starts. 
        template <typename TDelegate>
        Listener OnSuiteStarted(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test suite starts. 
        template <typename TDelegate>
        Listener OnSuiteFinished(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnCaseStarted(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnCaseFinished(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case success is reported.
        template <typename TDelegate>
        Listener OnCaseSuccess(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case failure is reported.
        template <typename TDelegate>
        Listener OnCaseFailure(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case is skipped.
        template <typename TDelegate>
        Listener OnCaseSkipped(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case message is reported. 
        template <typename TDelegate>
        Listener OnCaseMessage(TDelegate&& delegate) const;

    private:

        /// \brief Run a test suite.
        TestReport Run(const TestSuite& test_suite) const;

        /// \brief Event notified whenever a test suite starts.
        Event<const TestRunner&, OnTestRunnerSuiteStartedEventArgs> suite_started_event_;

        /// \brief Event notified whenever a test suite finishes.
        Event<const TestRunner&, OnTestRunnerSuiteFinishedEventArgs> suite_finished_event_;

        /// \brief Event notified whenever a test case starts.
        Event<const TestRunner&, OnTestRunnerCaseStartedEventArgs> case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        Event<const TestRunner&, OnTestRunnerCaseFinishedEventArgs> case_finished_event_;

        /// \brief Event notified whenever a success is reported.
        Event<const TestRunner&, OnTestRunnerCaseSuccessEventArgs> case_success_event_;

        /// \brief Event notified whenever a failure is reported.
        Event<const TestRunner&, OnTestRunnerCaseFailureEventArgs> case_failure_event_;

        /// \brief Event notified whenever a test case is skipped.
        Event<const TestRunner&, OnTestRunnerCaseSkippedEventArgs> case_skipped_event_;

        /// \brief Event notified whenever a test case message is reported.
        Event<const TestRunner&, OnTestRunnerCaseMessageEventArgs> case_message_event_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestRunner.

    inline TestReport TestRunner::Run(const Context& context) const
    {
        auto test_report = MakeTestReport(context);

        AutoTestSuite::ForEach([this, &test_report, context](const AutoTestSuite& auto_test_suite)
        {
            auto& test_suite = auto_test_suite.GetTestSuite();

            if (context.Contains(test_suite.GetName()))
            {
                test_report += Run(test_suite);
            }
        });

        return test_report;
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnSuiteStarted(TDelegate&& delegate) const
    {
        return suite_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnSuiteFinished(TDelegate&& delegate) const
    {
        return suite_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseStarted(TDelegate&& delegate) const
    {
        return case_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline  Listener TestRunner::OnCaseFinished(TDelegate&& delegate) const
    {
        return case_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseSuccess(TDelegate&& delegate) const
    {
        return case_success_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseFailure(TDelegate&& delegate) const
    {
        return case_failure_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseSkipped(TDelegate&& delegate) const
    {
        return case_skipped_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline  Listener TestRunner::OnCaseMessage(TDelegate&& delegate) const
    {
        return case_message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

}