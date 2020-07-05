/// \file unit_test_cli.h
/// \brief This header is part of Syntropy unit test. Contains definitions for unit test CLI.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

#include <iostream>
#include <iomanip>
#include <functional>

#include "syntropy/application/command_line.h"

#include "syntropy/core/string_stream.h"
#include "syntropy/core/types.h"
#include "syntropy/language/listener.h"
#include "syntropy/unit_test/test_runner.h"

#include "syntropy/application/console/console_output.h"
#include "syntropy/application/console/console_output_sections.h"
#include "syntropy/application/console/console_output_section_scope.h"

// #TODO Remove

#include "unit_tests/unit_tests.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST APPLICATION                                                */
    /************************************************************************/

    /// \brief Represents the Syntropy Unit Test application.
    /// \author Raffaele D. Facendola - June 2020.
    class UnitTestApplication
    {
    public:

        /// \brief No default constructor.
        UnitTestApplication() = delete;

        /// \brief No copy constructor.
        UnitTestApplication(const UnitTestApplication&) = delete;

        /// \brief No move constructor.
        UnitTestApplication(UnitTestApplication&&) = delete;

        /// \brief Create a new application.
        UnitTestApplication(const CommandLine& command_line);

        /// \brief No copy-assignment operator.
        UnitTestApplication& operator=(const UnitTestApplication&) = delete;

        /// \brief No move-assignment operator.
        UnitTestApplication& operator=(UnitTestApplication&&) = delete;

        /// \brief Destructor.
        ~UnitTestApplication();

        /// \brief Application entry point.
        Int Run();

    private:

        /// \brief Called whenever a test suite starts.
        void OnSuiteStarted(const TestRunner& sender, const syntropy::OnTestRunnerSuiteStartedEventArgs& e);

        /// \brief Called whenever a test case starts.
        void OnCaseStarted(const TestRunner& sender, const syntropy::OnTestRunnerCaseStartedEventArgs& e);

        /// \brief Called whenever a test case result is notified.
        void OnCaseFailure(const TestRunner& sender, const syntropy::OnTestRunnerCaseFailureEventArgs& e);

        /// \brief Called whenever a test case result is notified.
        void OnCaseSkipped(const TestRunner& sender, const syntropy::OnTestRunnerCaseSkippedEventArgs& e);

        /// \brief Called whenever a message is notified.
        void OnCaseMessage(const TestRunner& sender, const syntropy::OnTestRunnerCaseMessageEventArgs& e);

        /// \brief Called whenever a test case finishes.
        void OnCaseFinished(const TestRunner& sender, const syntropy::OnTestRunnerCaseFinishedEventArgs& e);

        /// \brief Called whenever a test suite finishes.
        void OnSuiteFinished(const TestRunner& sender, const syntropy::OnTestRunnerSuiteFinishedEventArgs& e);

        /// \brief Push a test suite to the console output.
        void ConditionalTestSuiteTrigger(const Label& test_suite);

        /// \brief Push a test case to the console output.
        void ConditionalTestCaseTrigger(const Label& test_case);

        /// \brief Console output stream.
        ConsoleOutput& out = ConsoleOutput::GetSingleton();

        /// \brief Command line arguments.
        const CommandLine command_line_;

        /// \brief Test runner.
        TestRunner test_runner;

        /// \brief Listener for the test runner events.
        Listener test_listener_;

        /// \brief Whether the current test suite was already triggered.
        /// A test suite gets triggered when a test case fails or is skipped.
        /// Untriggered test suites are hidden from the report to avoid clutter.
        bool test_suite_triggered_{ false };

        /// \brief Whether the current test case was already triggered.
        /// A test case gets triggered when it gets skipped or a fail is reported.
        /// Untriggered test cases are hidden from the report to avoid clutter.
        bool test_case_triggered_{ false };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTestApplication.

    inline UnitTestApplication::UnitTestApplication(const CommandLine& command_line)
        : command_line_(command_line)
    {
        auto BindMemberFunction = [this](auto function)
        {
            return std::bind(function, this, std::placeholders::_1, std::placeholders::_2);
        };

        test_listener_ += test_runner.OnSuiteStarted(BindMemberFunction(&UnitTestApplication::OnSuiteStarted));
        test_listener_ += test_runner.OnCaseStarted(BindMemberFunction(&UnitTestApplication::OnCaseStarted));
        test_listener_ += test_runner.OnCaseFailure(BindMemberFunction(&UnitTestApplication::OnCaseFailure));
        test_listener_ += test_runner.OnCaseSkipped(BindMemberFunction(&UnitTestApplication::OnCaseSkipped));
        test_listener_ += test_runner.OnCaseMessage(BindMemberFunction(&UnitTestApplication::OnCaseMessage));
        test_listener_ += test_runner.OnCaseFinished(BindMemberFunction(&UnitTestApplication::OnCaseFinished));
        test_listener_ += test_runner.OnSuiteFinished(BindMemberFunction(&UnitTestApplication::OnSuiteFinished));
    }

    inline UnitTestApplication::~UnitTestApplication()
    {
        out.PopSection();
    }

    inline Int UnitTestApplication::Run()
    {
        out.PushSection<ConsoleTitleSection>("Syntropy Unit Test Application\n(version 0.0.1)");

        // Run.

        auto test_report = test_runner.Run({});

        auto success = (test_report.fail_count_ == 0);

        // Final report.

        auto final_report_section = MakeConsoleOutputSectionScope<ConsoleHeading1Section>("Final report");

        out.Print("Test cases: ", UnitTest::GetTestCaseCount(test_report));

        out.LineFeed();

        out.Print(" - Success: ", test_report.success_count_);
        out.Print(" - Failed: ", test_report.fail_count_);
        out.Print(" - Skipped: ", test_report.skipped_count_);

        out.LineFeed();

        out.Print("Result: ", success ? "SUCCESS" : "FAIL");

        return (success ? 0 : 1);
    }

    inline void UnitTestApplication::OnSuiteStarted(const TestRunner& sender, const syntropy::OnTestRunnerSuiteStartedEventArgs& e)
    {
        test_suite_triggered_ = false;
    }

    inline void UnitTestApplication::OnCaseStarted(const TestRunner& sender, const syntropy::OnTestRunnerCaseStartedEventArgs& e)
    {
        test_case_triggered_ = false;
    }

    inline void UnitTestApplication::OnCaseFailure(const TestRunner& sender, const syntropy::OnTestRunnerCaseFailureEventArgs& e)
    {
        ConditionalTestSuiteTrigger(e.test_suite_);
        ConditionalTestCaseTrigger(e.test_case_);

        out.Print("FAILURE - ", e.expression_, " returned '", e.result_, "' but '", e.expected_, "' was expected.");
    }

    inline void UnitTestApplication::OnCaseSkipped(const TestRunner& sender, const syntropy::OnTestRunnerCaseSkippedEventArgs& e)
    {
        ConditionalTestSuiteTrigger(e.test_suite_);
        ConditionalTestCaseTrigger(e.test_case_);

        out.Print("SKIP - ", e.reason_);
    }

    inline void UnitTestApplication::OnCaseMessage(const TestRunner& sender, const syntropy::OnTestRunnerCaseMessageEventArgs& e)
    {
        ConditionalTestSuiteTrigger(e.test_suite_);
        ConditionalTestCaseTrigger(e.test_case_);

        out.Print(e.message_);
    }

    inline void UnitTestApplication::OnCaseFinished(const TestRunner& sender, const syntropy::OnTestRunnerCaseFinishedEventArgs& e)
    {
        if (test_suite_triggered_)
        {
            out.PopSection();
        }
    }

    inline void UnitTestApplication::OnSuiteFinished(const TestRunner& sender, const syntropy::OnTestRunnerSuiteFinishedEventArgs& e)
    {
        if (test_case_triggered_)
        {
            out.PopSection();
        }
    }

    inline void UnitTestApplication::ConditionalTestSuiteTrigger(const Label& test_suite)
    {
        if (!test_suite_triggered_)
        {
            test_suite_triggered_ = true;

            out.PushSection<ConsoleHeading1Section>(test_suite);
        }
    }

    inline void UnitTestApplication::ConditionalTestCaseTrigger(const Label& test_case)
    {
        if (!test_case_triggered_)
        {
            test_case_triggered_ = true;

            out.PushSection<ConsoleHeading4Section>(test_case);
        }
    }

}