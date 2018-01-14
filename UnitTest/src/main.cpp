

#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <exception>
#include <chrono>
#include <iomanip>

#include "unit1.h"

#include "containers/hashed_string.h"
#include "containers/context.h"

#include "diagnostics/log.h"
#include "diagnostics/log_channels.h"
#include "diagnostics/diagnostics_meta.h"

#include "serialization/json/json.h"

#include "math/random.h"

#include "application/command_line.h"

#include "unit_test/test_runner.h"

syntropy::Context Root;

void Initialize()
{
    // Initialize log

    using syntropy::diagnostics::FileLogChannel;

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::diagnostics::FileLogChannel>());

    syntropy::diagnostics::ImportLogConfigurationFromJSON("log.cfg");
}

int main(int argc, char **argv)
{
    syntropy::CommandLine command_line(argc, argv);

    Initialize();

    if (command_line.HasArgument("test_reflection"))
    {
        Tester t;
        t.Do();
    }

    std::cout << "\n\n";

    auto&& test_runner = syntropy::TestRunner::GetInstance();

    auto on_started_listener = test_runner.OnStarted().Subscribe([](auto& /*sender*/, auto& /*args*/)
    {
        std::cout << "\nRunning unit tests:\n\n";
    });

    auto on_test_suite_started_listener = test_runner.OnTestSuiteStarted().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "   Testing suite '" << args.test_suite_.GetName() << "'\n";
    });

    auto on_test_case_started_listener = test_runner.OnTestCaseStarted().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "      Testing case '" << args.test_case_.GetName() << "'\n";
    });

    auto on_test_case_message_listener = test_runner.OnTestCaseMessageNotified().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "         " << "             " << args.message_ << "\n";
    });

    auto on_test_case_result_listener = test_runner.OnTestCaseResultNotified().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "         " << std::setw(10) << args.result_ << " : " << args.message_ << "\n";
    });

    auto on_test_case_finished_listener = test_runner.OnTestCaseFinished().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "      Test case result: " << args.result_ << "\n";
    });

    auto on_test_suite_finished_listener = test_runner.OnTestSuiteFinished().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "   Test suite result: " << args.result_ << "\n\n";
    });

    auto on_finished_listener = test_runner.OnFinished().Subscribe([](auto& /*sender*/, auto& args)
    {
        std::cout << "Result: " << args.result_ << "\n";
    });

    test_runner.Run("");

    system("pause");

}

