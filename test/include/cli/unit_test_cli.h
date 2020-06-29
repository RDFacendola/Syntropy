/// \file unit_test_cli.h
/// \brief This header is part of Syntropy unit test. Contains definitions for unit test CLI.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

#include <iostream>
#include <iomanip>
#include <functional>

#include "syntropy/core/string_stream.h"
#include "syntropy/core/types.h"
#include "syntropy/language/listener.h"
#include "syntropy/unit_test/test_runner.h"

#include "syntropy/application/console/console_output.h"
#include "syntropy/application/console/console_output_section.h"
#include "syntropy/application/console/console_output_section_scope.h"
#include "syntropy/application/console/default_console_style.h"
#include "syntropy/application/console/plain_console_style.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST CLI                                                        */
    /************************************************************************/

    /// \brief Represents a Command Line Interface for Syntropy Unit Test.
    /// \author Raffaele D. Facendola - June 2020.
    class UnitTestCLI
    {
    public:

        /// \brief Create a new CLI bound to a test runner.
        UnitTestCLI(const TestRunner& runner);

        /// \brief Report the final outcome of the test runner.
        void Report(const TestReport& test_report);

    private:

        /// \brief Called whenever a test suite starts.
        void OnSuiteStarted(const TestRunner& sender, const syntropy::OnTestRunnerSuiteStartedEventArgs& e);

        /// \brief Called whenever a test case starts.
        void OnCaseStarted(const TestRunner& sender, const syntropy::OnTestRunnerCaseStartedEventArgs& e);

        /// \brief Called whenever a test case result is notified.
        void OnCaseSuccess(const TestRunner& sender, const syntropy::OnTestRunnerCaseSuccessEventArgs& e);

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

        /// \brief Listener for the test runner events.
        Listener test_runner_listener_;

           /// \brief Maximum line size.
        Int line_size_{ 120 };
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTestCLI.

    inline UnitTestCLI::UnitTestCLI(const syntropy::TestRunner& runner)
    {
        auto& out = ConsoleOutput::GetSingleton();

        {
            auto title_section = MakeConsoleOutputSectionScope<ConsoleTitleSection>("Syntropy Unit Test Application\n(version 0.0.1)");

            {
                auto heading_section = MakeConsoleOutputSectionScope<ConsoleHeading1Section>("Main menu");

                out.Print("What you want to do?");
            }
        }

        using namespace std::placeholders;

        auto BindMemberFunction = [this](auto function)
        {
            return std::bind(function, this, _1, _2);
        };

        test_runner_listener_ += runner.OnSuiteStarted(BindMemberFunction(&UnitTestCLI::OnSuiteStarted));
        test_runner_listener_ += runner.OnCaseStarted(BindMemberFunction(&UnitTestCLI::OnCaseStarted));
        test_runner_listener_ += runner.OnCaseSuccess(BindMemberFunction(&UnitTestCLI::OnCaseSuccess));
        test_runner_listener_ += runner.OnCaseFailure(BindMemberFunction(&UnitTestCLI::OnCaseFailure));
        test_runner_listener_ += runner.OnCaseSkipped(BindMemberFunction(&UnitTestCLI::OnCaseSkipped));
        test_runner_listener_ += runner.OnCaseMessage(BindMemberFunction(&UnitTestCLI::OnCaseMessage));
        test_runner_listener_ += runner.OnCaseFinished(BindMemberFunction(&UnitTestCLI::OnCaseFinished));
        test_runner_listener_ += runner.OnSuiteFinished(BindMemberFunction(&UnitTestCLI::OnSuiteFinished));
    }

    inline void UnitTestCLI::Report(const TestReport& test_report)
    {

    }

    inline void UnitTestCLI::OnSuiteStarted(const TestRunner& sender, const syntropy::OnTestRunnerSuiteStartedEventArgs& e)
    {

    }


    inline void UnitTestCLI::OnCaseStarted(const TestRunner& sender, const syntropy::OnTestRunnerCaseStartedEventArgs& e)
    {

    }

    inline void UnitTestCLI::OnCaseSuccess(const TestRunner& sender, const syntropy::OnTestRunnerCaseSuccessEventArgs& e)
    {
        std::cout << "SUCCESS " << e.test_suite_.GetName() << " " << e.test_case_.GetCharacters() << " : " << e.expression_ << "\n";
    }

    inline void UnitTestCLI::OnCaseFailure(const TestRunner& sender, const syntropy::OnTestRunnerCaseFailureEventArgs& e)
    {
        std::cout << "FAILURE " << e.test_suite_.GetName() << " " << e.test_case_.GetCharacters() << " : " << e.expression_ << " returned " << e.result_ << " where " << e.expected_ << " was expected.\n";
    }

    inline void UnitTestCLI::OnCaseSkipped(const TestRunner& sender, const syntropy::OnTestRunnerCaseSkippedEventArgs& e)
    {
        std::cout << "SKIPPED " << e.test_suite_.GetName() << " " << e.test_case_.GetCharacters() << " : " << e.reason_ << "\n";
    }

    inline void UnitTestCLI::OnCaseMessage(const TestRunner& sender, const syntropy::OnTestRunnerCaseMessageEventArgs& e)
    {

    }

    inline void UnitTestCLI::OnCaseFinished(const TestRunner& sender, const syntropy::OnTestRunnerCaseFinishedEventArgs& e)
    {

    }

    inline void UnitTestCLI::OnSuiteFinished(const TestRunner& sender, const syntropy::OnTestRunnerSuiteFinishedEventArgs& e)
    {
        std::cout << "\n";
    }

}