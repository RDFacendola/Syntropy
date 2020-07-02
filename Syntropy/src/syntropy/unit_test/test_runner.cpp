#include "syntropy/unit_test/test_runner.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    TestReport TestRunner::Run(const TestSuite& test_suite) const
    {
        // Setup listeners for the current test suite.

        auto test_suite_listener = syntropy::Listener{};

        test_suite_listener += test_suite.OnCaseStarted([this](const auto& sender, const auto& event_args)
        {
            case_started_event_.Notify(*this, { event_args.test_case_, sender.GetName() });
        });

        test_suite_listener += test_suite.OnCaseFinished([this](const auto& sender, const auto& event_args)
        {
            case_finished_event_.Notify(*this, { event_args.test_case_, event_args.test_report_, sender.GetName() });
        });

        test_suite_listener += test_suite.OnCaseSuccess([this](const auto& sender, const auto& event_args)
        {
            case_success_event_.Notify(*this, { event_args.location_, event_args.expression_, event_args.result_, event_args.test_case_, sender.GetName() });
        });

        test_suite_listener += test_suite.OnCaseFailure([this](const auto& sender, const auto& event_args)
        {
            case_failure_event_.Notify(*this, { event_args.location_, event_args.expression_, event_args.result_, event_args.expected_, event_args.test_case_, sender.GetName() });
        });

        test_suite_listener += test_suite.OnCaseSkipped([this](const auto& sender, const auto& event_args)
        {
            case_skipped_event_.Notify(*this, { event_args.location_, event_args.reason_, event_args.test_case_, sender.GetName() });
        });

        test_suite_listener += test_suite.OnCaseMessage([this](const auto& sender, const auto& event_args)
        {
            case_message_event_.Notify(*this, { event_args.location_, event_args.message_, event_args.test_case_, sender.GetName() });
        });

        // Run the suite.

        suite_started_event_.Notify(*this, { test_suite.GetName() });

        auto test_report = test_suite.Run();

        suite_finished_event_.Notify(*this, { test_suite.GetName(), test_report });

        // Report.

        return test_report;
    }

}