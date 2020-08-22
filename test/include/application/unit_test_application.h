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
#include "syntropy/language/language.h"
#include "syntropy/language/listener.h"
#include "syntropy/unit_test/test_runner.h"

#include "syntropy/application/console/console_output.h"
#include "syntropy/application/console/console_output_sections.h"
#include "syntropy/application/console/console_output_section_scope.h"

// #TODO Remove

#include "unit_tests/unit_tests.h"

namespace Syntropy
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
        void OnSuiteStarted(const TestRunner& sender, const Syntropy::OnTestRunnerSuiteStartedEventArgs& e);

        /// \brief Called whenever a test case starts.
        void OnCaseStarted(const TestRunner& sender, const Syntropy::OnTestRunnerCaseStartedEventArgs& e);

        /// \brief Called whenever a test case failure is reported.
        void OnCaseFailure(const TestRunner& sender, const Syntropy::OnTestRunnerCaseFailureEventArgs& e);

        /// \brief Called whenever a test case finishes.
        void OnCaseFinished(const TestRunner& sender, const Syntropy::OnTestRunnerCaseFinishedEventArgs& e);

        /// \brief Called whenever a test suite finishes.
        void OnSuiteFinished(const TestRunner& sender, const Syntropy::OnTestRunnerSuiteFinishedEventArgs& e);

        /// \brief Console output stream.
        ConsoleOutput& out = ConsoleOutput::GetSingleton();

        /// \brief Command line arguments.
        const CommandLine command_line_;

        /// \brief Test runner.
        TestRunner test_runner;

        /// \brief Listener for the test runner events.
        Listener test_listener_;

        /// \brief Total number of test suites tested.
        Int total_test_suites_{ 0 };

        /// \brief Number of test suites containing zero failed test cases.
        Int passed_test_suites_{ 0 };

        /// \brief Number of test suites containing at least one failed test case.
        Int failed_test_suites_{ 0 };

        /// \brief Total test cases tested.
        Int total_test_cases_{ 0 };

        /// \brief Number of passed test cases.
        Int passed_test_cases_{ 0 };

        /// \brief Number of failed test cases.
        Int failed_test_cases_{ 0 };

        /// \brief Whether the current test suite contains at least one failed test case.
        bool test_suite_failed_{ false };

        /// \brief Whether the current test case contains at least one failure.
        bool test_case_failed_{ false };

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTestApplication.

    inline UnitTestApplication::UnitTestApplication(const CommandLine& command_line)
        : command_line_(command_line)
    {
        out.PushSection<ConsoleTitleSection>("Syntropy Unit Test Application\n(version 0.0.1)");

        auto BindMemberFunction = [this](auto function)
        {
            return std::bind(function, this, std::placeholders::_1, std::placeholders::_2);
        };

        test_listener_ += test_runner.OnSuiteStarted(BindMemberFunction(&UnitTestApplication::OnSuiteStarted));
        test_listener_ += test_runner.OnCaseStarted(BindMemberFunction(&UnitTestApplication::OnCaseStarted));
        test_listener_ += test_runner.OnCaseFailure(BindMemberFunction(&UnitTestApplication::OnCaseFailure));
        test_listener_ += test_runner.OnCaseFinished(BindMemberFunction(&UnitTestApplication::OnCaseFinished));
        test_listener_ += test_runner.OnSuiteFinished(BindMemberFunction(&UnitTestApplication::OnSuiteFinished));
    }

    inline UnitTestApplication::~UnitTestApplication()
    {
        out.PopSection();
    }

    inline Int UnitTestApplication::Run()
    {
        // Run.

        test_runner.Run({});

        // Final report.

        auto final_report_section = MakeConsoleOutputSectionScope<ConsoleHeading1Section>("Final report");

        out
            .Print("Test suites tested: ", total_test_suites_)
            .LineFeed()
            .Print(" > Success: ", passed_test_suites_)
            .Print(" > Failed: ", failed_test_suites_)
            .LineFeed()
            .Print("Test cases tested: ", total_test_cases_)
            .LineFeed()
            .Print(" > Success: ", passed_test_cases_)
            .Print(" > Failed: ", failed_test_cases_)
            .LineFeed();

        return ((failed_test_cases_ == 0) ? 0 : 1);
    }

    inline void UnitTestApplication::OnSuiteStarted(const TestRunner& sender, const Syntropy::OnTestRunnerSuiteStartedEventArgs& e)
    {
        test_suite_failed_ = false;

        ++total_test_suites_;
    }

    inline void UnitTestApplication::OnCaseStarted(const TestRunner& sender, const Syntropy::OnTestRunnerCaseStartedEventArgs& e)
    {
        test_case_failed_ = false;

        ++total_test_cases_;
    }

    inline void UnitTestApplication::OnCaseFailure(const TestRunner& sender, const Syntropy::OnTestRunnerCaseFailureEventArgs& e)
    {
        if (!test_suite_failed_)
        {
            test_suite_failed_ = true;

            out.PushSection<ConsoleHeading1Section>(e.test_suite_);         // Output the test suite header on the first reported error.
        }

        if (!test_case_failed_)
        {
            test_case_failed_ = true;

            out.PushSection<ConsoleHeading3Section>(e.test_case_);          // Output the test case header on the first reported error.

            auto& location = e.location_.GetFunction();

            out
                .Print("(", location.file_, "@", location.line_, ")")
                .LineFeed();
        }

        out.Print(" > ", e.expression_, " returned '", e.result_, "' but '", e.expected_, "' was expected.");
    }

    inline void UnitTestApplication::OnCaseFinished(const TestRunner& sender, const Syntropy::OnTestRunnerCaseFinishedEventArgs& e)
    {
        if (test_case_failed_)
        {
            out.PopSection();                                               // Pop test case header.

            ++failed_test_cases_;
        }
        else
        {
            ++passed_test_cases_;
        }
    }

    inline void UnitTestApplication::OnSuiteFinished(const TestRunner& sender, const Syntropy::OnTestRunnerSuiteFinishedEventArgs& e)
    {
        if (test_suite_failed_)
        {
            out.PopSection();                                               // Pop test suite header.

            ++failed_test_suites_;
        }
        else
        {
            ++passed_test_suites_;
        }
    }

}