

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

#include "vm/virtual_machine.h"
#include "vm/intrinsics.h"



#include "diagnostics/log.h"
#include "diagnostics/log_channels.h"
#include "diagnostics/diagnostics_meta.h"

#include "serialization/json/json.h"

#include "platform/system.h"
#include "platform/threading.h"

#include "patterns/observable.h"

#include "time/calendar.h"
#include "time/timer.h"

#include "math/random.h"

#include "synergy.h"
#include "task/scheduler.h"
#include "patterns/sync_counter.h"

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

void ReflectionAndSerializationTest()
{
    Tester t;
    t.Do();
}

#define COUNT 1 << 16

struct Maxer
{
    Maxer(std::vector<int>& numbers, size_t begin, size_t end, int* max)
        : numbers_(&numbers)
        , begin_(begin)
        , end_(end)
        , max_(max)
    {

    }

    void operator()()
    {
        if (end_ - begin_ < 2)
        {
            *max_ = (*numbers_)[begin_];
            return;
        }

        size_t mid = (end_ + begin_) >> 1;

        auto max_l = new int;
        auto max_r = new int;

        auto l = syntropy::synergy::EmplaceTask<Maxer>({}, *numbers_, begin_, mid, max_l);
        auto r = syntropy::synergy::EmplaceTask<Maxer>({}, *numbers_, mid, end_, max_r);

        syntropy::synergy::CreateTaskContinuation({l, r},
            [max_l, max_r, max_lr = max_, be = begin_, en = end_]()
        {
            *max_lr = std::max(*max_l, *max_r);

            delete max_l;
            delete max_r;

            if (be == 0 && en == COUNT)
            {
                std::cout << "Maximum value found: " << *max_lr;
            }
        });
    }

    std::vector<int>* numbers_;
    size_t begin_ = 0;
    size_t end_ = 0;
    int* max_ = 0;
};

void MultithreadTest()
{
    using namespace std::literals::chrono_literals;

    const size_t count = COUNT;

    std::vector<int> numbers(count);

    srand(0);       // Be sure the random sequence stays the same for different runs.

    for (auto&& number : numbers)
    {
        number = rand() % 65536;
    }

    int max;

    auto& scheduler = syntropy::synergy::GetScheduler();

    syntropy::synergy::GetScheduler().Initialize();

    syntropy::synergy::DetachTask(
    [&numbers, &max]()
    {
        std::cout << "Starting: " << std::this_thread::get_id() << "\n";

        syntropy::synergy::EmplaceTask<Maxer>({}, numbers, 0, numbers.size(), &max);
    });

    system("pause");

    SYNTROPY_UNUSED(scheduler);

    system("pause");
}

int main(int argc, char **argv)
{
    syntropy::CommandLine command_line(argc, argv);

    Initialize();

    if (command_line.HasArgument("test_reflection"))
    {
        ReflectionAndSerializationTest();
    }

    if (command_line.HasArgument("test_synergy"))
    {
        MultithreadTest();
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

