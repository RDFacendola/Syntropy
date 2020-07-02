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

        /// \brief Argument used to filter test suites by name.
        static constexpr auto kContextArgument = "ctx";

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

        /// \brief Console output stream.
        ConsoleOutput& out = ConsoleOutput::GetSingleton();

        /// \brief Command line arguments.
        const CommandLine command_line_;

        /// \brief Test runner.
        TestRunner test_runner;

        /// \brief Listener for the test runner events.
        Listener test_listener_;
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
        test_listener_ += test_runner.OnCaseSuccess(BindMemberFunction(&UnitTestApplication::OnCaseSuccess));
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

        auto context_argument = command_line_.GetArgument(kContextArgument);

        auto context = context_argument ? syntropy::Context{ context_argument->GetValue() } : syntropy::Context{};

        auto test_report = test_runner.Run(context);

        system("pause");

        return 0;
    }

    inline void UnitTestApplication::OnSuiteStarted(const TestRunner& sender, const syntropy::OnTestRunnerSuiteStartedEventArgs& e)
    {
        out.PushSection<ConsoleHeading1Section>("Testing\n", e.test_suite_.GetName());
    }

    inline void UnitTestApplication::OnCaseStarted(const TestRunner& sender, const syntropy::OnTestRunnerCaseStartedEventArgs& e)
    {
        out.PushSection<ConsoleHeading3Section>(e.test_case_);
    }

    inline void UnitTestApplication::OnCaseSuccess(const TestRunner& sender, const syntropy::OnTestRunnerCaseSuccessEventArgs& e)
    {
        out.Print("SUCCESS - ", e.expression_, " returned ", e.result_);
    }

    inline void UnitTestApplication::OnCaseFailure(const TestRunner& sender, const syntropy::OnTestRunnerCaseFailureEventArgs& e)
    {
        out.Print("FAILURE - ", e.expression_);
        out.Print(" Result: ", e.result_);
        out.Print(" Expected: ", e.expected_);
    }

    inline void UnitTestApplication::OnCaseSkipped(const TestRunner& sender, const syntropy::OnTestRunnerCaseSkippedEventArgs& e)
    {
        out.Print("SKIP - ", e.reason_);
    }

    inline void UnitTestApplication::OnCaseMessage(const TestRunner& sender, const syntropy::OnTestRunnerCaseMessageEventArgs& e)
    {

    }

    inline void UnitTestApplication::OnCaseFinished(const TestRunner& sender, const syntropy::OnTestRunnerCaseFinishedEventArgs& e)
    {
        out.PopSection();
    }

    inline void UnitTestApplication::OnSuiteFinished(const TestRunner& sender, const syntropy::OnTestRunnerSuiteFinishedEventArgs& e)
    {
        out.PopSection();
    }

}