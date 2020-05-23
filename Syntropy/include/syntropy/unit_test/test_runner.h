
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
    /* ON TEST RUNNER SUITE RESULT EVENT ARGS                               */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test result is reported.
    struct OnTestRunnerCaseResultEventArgs : OnTestSuiteCaseResultEventArgs
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

        /// \brief Run all test suites with a matching context.
        TestReport Run(const Context& context);

        /// \brief Bind to the event notified whenever a test suite starts. 
        template <typename TDelegate>
        Listener OnSuiteStarted(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test suite starts. 
        template <typename TDelegate>
        Listener OnSuiteFinished(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnCaseStarted(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnCaseFinished(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case result is reported. 
        template <typename TDelegate>
        Listener OnCaseResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case message is reported. 
        template <typename TDelegate>
        Listener OnCaseMessage(TDelegate&& delegate);

    private:

        /// \brief Run a test suite.
        TestReport Run(TestSuite& test_suite);

        /// \brief Event notified whenever a test suite starts.
        Event<TestRunner*, OnTestRunnerSuiteStartedEventArgs> suite_started_event_;

        /// \brief Event notified whenever a test suite finishes.
        Event<TestRunner*, OnTestRunnerSuiteFinishedEventArgs> suite_finished_event_;

        /// \brief Event notified whenever a test case starts.
        Event<TestRunner*, OnTestRunnerCaseStartedEventArgs> case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        Event<TestRunner*, OnTestRunnerCaseFinishedEventArgs> case_finished_event_;

        /// \brief Event notified whenever a test case result is reported.
        Event<TestRunner*, OnTestRunnerCaseResultEventArgs> case_result_event_;

        /// \brief Event notified whenever a test case message is reported.
        Event<TestRunner*, OnTestRunnerCaseMessageEventArgs> case_message_event_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestRunner.

    inline TestReport TestRunner::Run(const Context& context)
    {
        auto test_report = MakeTestReport(context);

        AutoTestSuite::ForEach([this, &test_report, context](const AutoTestSuite& auto_test_suite)
        {
            if (context.Contains(auto_test_suite.GetName()))
            {
                auto test_suite = auto_test_suite.CreateTestSuite();

                test_report += Run(*test_suite);
            }
        });

        return test_report;
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnSuiteStarted(TDelegate&& delegate)
    {
        return suite_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnSuiteFinished(TDelegate&& delegate)
    {
        return suite_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseStarted(TDelegate&& delegate)
    {
        return case_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline  Listener TestRunner::OnCaseFinished(TDelegate&& delegate)
    {
        return case_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestRunner::OnCaseResult(TDelegate&& delegate)
    {
        return case_result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline  Listener TestRunner::OnCaseMessage(TDelegate&& delegate)
    {
        return case_message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline TestReport TestRunner::Run(TestSuite& test_suite)
    {
        // Setup listeners for the current test suite.

        auto test_suite_listener = syntropy::Listener{};

        test_suite_listener += test_suite.OnCaseStarted([this](TestSuite* sender, const OnTestSuiteCaseStartedEventArgs& event_args)
        {
            case_started_event_.Notify(this, { event_args.test_case_, sender->GetName() });
        });

        test_suite_listener += test_suite.OnCaseFinished([this](TestSuite* sender, const OnTestSuiteCaseFinishedEventArgs& event_args)
        {
            case_finished_event_.Notify(this, { event_args.test_case_, event_args.test_report_, sender->GetName() });
        });

        test_suite_listener += test_suite.OnCaseResult([this](TestSuite* sender, const OnTestSuiteCaseResultEventArgs& event_args)
        {
            case_result_event_.Notify(this, { event_args.result_, event_args.message_, event_args.location_, event_args.test_case_, sender->GetName() });
        });

        test_suite_listener += test_suite.OnCaseMessage([this](TestSuite* sender, const OnTestSuiteCaseMessageEventArgs& event_args)
        {
            case_message_event_.Notify(this, { event_args.message_, event_args.test_case_, sender->GetName() });
        });

        // Run the suite.

        suite_started_event_.Notify(this, { test_suite.GetName() });

        auto test_report = test_suite.Run();

        suite_finished_event_.Notify(this, { test_suite.GetName(), test_report });

        // Report.

        return test_report;
    }

}