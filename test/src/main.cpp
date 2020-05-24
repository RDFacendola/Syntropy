
#include <iostream>
#include <iomanip>
#include <type_traits>
#include "syntropy/application/command_line.h"
#include "syntropy/application/command_line_argument.h"

#include "syntropy/unit_test/test_runner.h"
#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_macros.h"

class MyFixture
{
public:

    void After() { std::cout << "MyFixture::After\n"; }
    void Before() { std::cout << "MyFixture::Before\n"; }

    void Foo()
    {
        SYNTROPY_UNIT_ASSERT((1 + 1) == 2);
    }

    void Bar()
    {
        SYNTROPY_UNIT_EXPECT(1 == 1);
        SYNTROPY_UNIT_EXPECT(1 == 1);
        SYNTROPY_UNIT_EXPECT(1 == 1);
        SYNTROPY_UNIT_EXPECT(1 == 1);
        SYNTROPY_UNIT_EXPECT(1 == 3);
        SYNTROPY_UNIT_EXPECT(1 == 1);
        SYNTROPY_UNIT_MESSAGE("hello!");
    }

    void FooBar()
    {
        SYNTROPY_UNIT_ASSERT(60 * 0 == 0);
    }
};

class YourFixture
{
public:

    void Before() { std::cout << "YourFixture::Before\n"; }

    void Foo()
    {
        SYNTROPY_UNIT_ASSERT((1 + 2) == 3);
    }

    void Bar()
    {
        SYNTROPY_UNIT_ASSERT((1 + 1) == 2);
    }
};

auto MySuite = syntropy::MakeAutoTestSuite<MyFixture>("MyFixture.suite");

const auto MyTestA = syntropy::MakeAutoTestCase("Foo", &MyFixture::Foo);
const auto MyTestB = syntropy::MakeAutoTestCase("Bar", &MyFixture::Bar);
const auto MyTestC = syntropy::MakeAutoTestCase("FooBar", &MyFixture::FooBar);

auto YourSuite = syntropy::MakeAutoTestSuite<YourFixture>("YourFixture.suite");

const auto YourTestA = syntropy::MakeAutoTestCase("Foo", &YourFixture::Foo);
const auto YourTestB = syntropy::MakeAutoTestCase("Bar", &YourFixture::Bar);





int main(int argc, char **argv)
{
    auto command_line = syntropy::CommandLine{ argc, argv };

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

    // Run.

    std::cout << "Running unit tests:\n\n";

    auto test_report = test_runner.Run("");

    std::cout << "Result: " << syntropy::UnitTest::GetResult(test_report) << "\n";

    system("pause");

    return 0;
}