
#include <iostream>
#include <iomanip>

#include "syntropy/core/types.h"

#include "syntropy/application/command_line.h"
#include "syntropy/application/command_line_argument.h"

#include "syntropy/unit_test/test_runner.h"
#include "syntropy/unit_test/test_report.h"

#include "syntropy/unit_test/test_macros.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/auto_test_case.h"

// #TODO Remove
#include "test/unit_tests.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

using syntropy::Int;
using syntropy::Float;

int main(int argc, char **argv)
{
    auto command_line = syntropy::CommandLine{ argc, argv };

    auto context_argument = command_line.GetArgument("ctx");

    auto test_runner = syntropy::TestRunner{};

    // Setup runner listeners.

    auto test_runner_listener = syntropy::Listener{};

    test_runner_listener += test_runner.OnCaseFinished([](const auto& sender, const auto& event_args)
    {
        std::cout << "   " << std::setw(8) << syntropy::UnitTest::GetResult(event_args.test_report_) << " : " << (syntropy::Context(event_args.test_case_) + event_args.test_suite_) << "\n";
    });

    test_runner_listener += test_runner.OnSuiteFinished([](const auto& sender, const auto& event_args)
    {
        std::cout << "\n";
    });

    // Either run the specified suites or run everything.

    std::cout << "Running unit tests:\n\n";

    auto context = context_argument ? syntropy::Context{ context_argument->GetValue() } : syntropy::Context{};

    auto test_report = test_runner.Run(context);

    std::cout << "Result: " << syntropy::UnitTest::GetResult(test_report) << "\n";

    system("pause");

    return 0;
}