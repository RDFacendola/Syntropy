

#include <iostream>
#include <iomanip>

#include "syntropy/application/command_line.h"

#include "syntropy/unit_test/test_runner.h"

#include "syntropy/time/timer.h"

#include "syntropy/math/vector.h"
#include "syntropy/math/random.h"


#include "syntropy/memory/bytes.h"
#include "syntropy/math/rotation.h"

#include "syntropy/containers/stream_vector.h"
#include "syntropy/containers/stream_vector_view.h"

#include "syntropy/containers/string.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/array.h"
#include "syntropy/containers/set.h"
#include "syntropy/containers/map.h"

#include "syntropy/allocators/memory_resource.h"
#include "syntropy/allocators/memory_context.h"
#include "syntropy/allocators/scope_allocator.h"

// Tier 0
#include "syntropy/allocators/null_memory_resource.h"
#include "syntropy/allocators/virtual_memory_resource.h"
#include "syntropy/allocators/stack_memory_resource.h"

// Tier 1
#include "syntropy/allocators/fixed_memory_resource.h"
#include "syntropy/allocators/linear_memory_resource.h"
#include "syntropy/allocators/pool_memory_resource.h"

// Tier 2
#include "syntropy/allocators/tlsf_memory_resource.h"

// Tier Omega
#include "syntropy/allocators/passthrough_memory_resource.h"
#include "syntropy/allocators/counting_memory_resource.h"
#include "syntropy/allocators/chain_memory_resource.h"

#include "syntropy/types/range.h"

struct MyObject
{
    int foo_;
};

int main(int argc, char **argv)
 {
    char  b[] = "Hello, World!";

    auto v = syntropy::Vector<int>{1,2,3};
    //auto m = syntropy::Map<float, float>{ {1.f,2.f}, {3.f, 4.f} };

    auto range = syntropy::MakeRange(v);

    std::cout << range.GetBack();

    for(auto r : range)
    {
        //std::cout << r;
    }

    return 0;

    syntropy::CommandLine command_line(argc, argv);
 
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
 