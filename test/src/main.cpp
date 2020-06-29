/// \file main.cpp
/// \brief Defines the entry point of the unit test application.
///
/// \author Raffaele D. Facendola - June 2020.

#include "syntropy/application/command_line.h"
#include "syntropy/application/command_line_argument.h"

#include "cli/unit_test_cli.h"

// #TODO Remove
#include "unit_tests/unit_tests.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    auto command_line = syntropy::CommandLine{ argc, argv };

    auto context_argument = command_line.GetArgument("ctx");

    auto context = context_argument ? syntropy::Context{ context_argument->GetValue() } : syntropy::Context{};

    auto test_runner = syntropy::TestRunner{};

    {
        auto CLI = syntropy::UnitTestCLI{ test_runner };

        auto test_report = test_runner.Run(context);

        CLI.Report(test_report);
    }
    
    system("pause");

    return 0;
}


