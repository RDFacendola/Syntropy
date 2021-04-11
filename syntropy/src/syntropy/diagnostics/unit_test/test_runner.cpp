
/// \file test_runner.cpp
///
/// \author Raffaele D. Facendola - 2020

// ===========================================================================

#include "syntropy/diagnostics/unit_test/test_runner.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    void TestRunner::Run(Immutable<TestSuite> test_suite) const noexcept
    {
        // Setup listeners for the current test suite.

        auto test_suite_listener = Listener{};

        auto on_case_started = [this] (const auto& sender,
                                       const auto& event_args)
        {
            case_started_event_.Notify(*this, { event_args.test_case_,
                                                sender.GetName() });
        };

        auto on_case_finished = [this] (const auto& sender,
                                        const auto& event_args)
        {
            case_finished_event_.Notify(*this, { event_args.test_case_,
                                                 sender.GetName() });
        };

        auto on_case_success = [this](const auto& sender,
                                      const auto& event_args)
        {
            case_success_event_.Notify(*this, { event_args.location_,
                                                event_args.expression_,
                                                event_args.test_case_,
                                                sender.GetName() });
        };

        auto on_case_failure = [this](const auto& sender,
                                      const auto& event_args)
        {
            case_failure_event_.Notify(*this, { event_args.location_,
                                                event_args.expression_,
                                                event_args.result_,
                                                event_args.expected_,
                                                event_args.test_case_,
                                                sender.GetName() });
        };

        test_suite_listener += test_suite.OnCaseStarted(on_case_started);
        test_suite_listener += test_suite.OnCaseFinished(on_case_finished);
        test_suite_listener += test_suite.OnCaseSuccess(on_case_success);
        test_suite_listener += test_suite.OnCaseFailure(on_case_failure);

        // Run the suite.

        suite_started_event_.Notify(*this, { test_suite.GetName() });

        test_suite.Run();

        suite_finished_event_.Notify(*this, { test_suite.GetName() });

    }

}

// ===========================================================================
