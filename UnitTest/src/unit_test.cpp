
// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

#include "memory/memory.h"
#include "memory/memory_units.h"
#include "memory/memory_manager.h"

#include "memory/allocators/std_allocator.h"
#include "memory/allocators/segregated_allocator.h"
#include "memory/allocators/stack_allocator.h"
#include "memory/allocators/layered_allocator.h"

#include "memory/memory_meta.h"

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

#include "algorithms/search/astar.h"

#include "application/command_line.h"

#include "unit_test/test_runner.h"
#include "unit_test/test_fixture.h"

syntropy::Context Root;

struct FooSmall
{
    char buffer[256];
};

struct FooMedium
{
    char buffer[257];
};

struct FooLarge
{
    char buffer[258];
};

void Initialize()
{
    // Initialize log

    using syntropy::diagnostics::FileLogChannel;

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::diagnostics::FileLogChannel>());

    syntropy::diagnostics::ImportLogConfigurationFromJSON("log.cfg");

    // Initialization of the memory manager

    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LinearSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::ExponentialSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::TwoLevelSegregatedFitAllocator>());
    SYNTROPY_UNUSED(syntropy::reflection::ClassOf<syntropy::LayeredAllocator>());

    syntropy::ImportMemoryConfigurationFromJSON("memory.cfg");
}

void ReflectionAndSerializationTest()
{
    Tester t;
    t.Do();
}

void AllocTest()
{
    void* p;
    void* q;
    void* r;

    {
        syntropy::MemoryContext ctx1("MasterAllocator2");

        p = SYNTROPY_MM_ALLOC(23_Bytes);

        {
            syntropy::MemoryContext ctx2("MasterAllocator1");

            q = SYNTROPY_MM_ALLOC(24_KiBytes);
            r = SYNTROPY_MM_ALLOC(2_MiBytes);
        }

        SYNTROPY_MM_FREE(p);        // TODO: quirk! p was allocated by "small", the actual allocator who handled the call was "master2". Incidentally also "master1" references "small", so it will take care of its deallocation. (that's correct but a little bit obscure)
        SYNTROPY_MM_FREE(q);
        SYNTROPY_MM_FREE(r);
    }
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
/// \brief Testing simple pathfinding problem from Paradox
/// https://paradox.kattis.com/problems/paradoxpath

enum KFindPathError
{
    kNodeIsUnreachable = -1,
    kNotEnoughSpace = -2
};

int FindPath(const int nStartX, const int nStartY,
    const int nTargetX, const int nTargetY,
    const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
    int* pOutBuffer, const int nOutBufferSize)
{	
    SYNTROPY_ASSERT(nOutBufferSize > 0 && nMapWidth > 0 && nMapHeight > 0);

    std::vector<int> graph_vector;

    const size_t map_size = nMapWidth*nMapHeight;
    graph_vector.reserve(map_size);
    for (size_t i = 0; i < map_size; i++)
    {
        graph_vector.emplace_back(*(pMap + i));
    }

    Graph graph(graph_vector, nMapWidth, nMapHeight);

    auto& start = graph.GetNodeAt(nStartX, nStartY);
    auto& end = graph.GetNodeAt(nTargetX, nTargetY);

    auto node_position = [&graph](const Node& node)
    {
        return (graph.GetWidth() * node.GetY()) + node.GetX();
    };

    auto adjacency_func =
    [&graph](const Node& node)
    {
        return graph.GetNeighbours(node);
    };

    auto cost_func_distance = [](const Node& _start, const Node& _end) -> float
    {
        return static_cast<float>(std::abs(_start.GetX() - _end.GetY()) + std::abs(_start.GetX() - _end.GetY()));
    };

    auto heuristic_func_distance = [](const Node& _start, const Node& _end) -> float
    {
        return std::sqrtf((std::powf(float(_end.GetX()) - float(_start.GetX()), 2) + std::powf(float(_end.GetY()) - float(_start.GetY()), 2)));
    };

    auto path = syntropy::synapse::AStar(
        start,
        end,
        adjacency_func,
        cost_func_distance,
        heuristic_func_distance);
    
    if (!path.size())
    {
        return KFindPathError::kNodeIsUnreachable;
    }

    if (path.size() > nOutBufferSize)
    {
        return KFindPathError::kNotEnoughSpace;
    }

    for (size_t i = 0; i < path.size(); i++)
    {
        auto Element = pOutBuffer + i;
        *Element = node_position(*path.at(i));
    }

    return static_cast<int>(path.size());
}

void SynapseTest()
{		
    unsigned char pMap[] = 
    { 
        1,1,1,0,
        1,0,1,0,
        1,1,0,0
    };

    constexpr size_t nOutBufferSize = 12;
    int pOutBuffer[nOutBufferSize];

    std::fill(std::begin(pOutBuffer), std::end(pOutBuffer), -1);

    int count = 0;

    syntropy::HighResolutionTimer<std::chrono::microseconds> timer(true);

    for (int i = 0; i < 10000; ++i)
    {
        count += FindPath(1, 2, 2, 1, pMap, 4, 3, pOutBuffer, nOutBufferSize);
    }
    
    float time = timer.Stop().count() / 10000.0f;
    count /= 10000;

    std::cout << "A* duration: " << time << " us\n";
    
    std::cout << "FindPath Output: " << count << std::endl;

    std::cout << "Path: " << std::endl;

    while (count-- > 0)
    {
        std::cout << pOutBuffer[count] << " -> ";
    }
    std::cout << std::endl;

}

class MyTestSuite : public syntropy::TestFixture
{
public:

    MyTestSuite()
    {
        DeclareTestCase("TestCaseA", std::bind(&MyTestSuite::TestCaseA, this));
        DeclareTestCase("TestCaseB", std::bind(&MyTestSuite::TestCaseB, this));
        DeclareTestCase("TestCaseC", std::bind(&MyTestSuite::TestCaseC, this));
    }

    void TestCaseA()
    {
        SYNTROPY_UNIT_EXPECT(1 + 2 == 3);
    }

    void TestCaseB()
    {
        SYNTROPY_UNIT_PRECONDITION(1 == 2);

        // Never called!
    }

    void TestCaseC()
    {
        SYNTROPY_UNIT_EXPECT(1 + 1 == 4);
    }
};

syntropy::AutoTestSuite<MyTestSuite> ut_my_test_suite("MyTestSuite");

int main(int argc, char **argv)
{
    Initialize();

    syntropy::CommandLine command_line(argc, argv);

    if (command_line.HasArgument("test_synapse"))
    {
        SynapseTest();
    }

    if (command_line.HasArgument("test_reflection"))
    {
        ReflectionAndSerializationTest();
    }

    if (command_line.HasArgument("test_alloc"))
    {
        AllocTest();
    }

    if (command_line.HasArgument("test_synergy"))
    {
        MultithreadTest();
    }

    std::cout << "\n\n";

    auto&& tr = syntropy::TestRunner::GetInstance();

    std::shared_ptr<syntropy::Listener> test_case_s;
    std::shared_ptr<syntropy::Listener> test_case_f;

    auto a = tr.OnTestSuiteStarted().Subscribe([&test_case_s, &test_case_f](const syntropy::TestRunner& /*sender*/, const syntropy::TestRunner::OnTestSuiteStartedEventArgs& e)
    {
        std::cout << "Running test suite '" << e.test_suite_->GetName() << "'\n";

        test_case_s = e.test_suite_->OnTestCaseStarted().Subscribe([](const syntropy::TestSuite& /*sender*/, const syntropy::TestSuite::OnTestCaseStartedEventArgs& e)
        {
            std::cout << "   Test case '" << e.test_case_->GetName() << "': ";
        });

        test_case_f = e.test_suite_->OnTestCaseFinished().Subscribe([](const syntropy::TestSuite& /*sender*/, const syntropy::TestSuite::OnTestCaseFinishedEventArgs& e)
        {
            std::cout << e.result_.result_ << "\n";

            // Display test failures and errors only.

            if (e.result_.result_ == syntropy::TestResult::kFailure || e.result_.result_ == syntropy::TestResult::kError)
            {
                std::cout << "      " << e.result_.message_ << "\n";
                    //<< "         " << e.result_.location_ << "\n\n";
            }
        });

    });

    auto b = tr.OnTestSuiteFinished().Subscribe([&test_case_s, &test_case_f](const syntropy::TestRunner& /*sender*/, const syntropy::TestRunner::OnTestSuiteFinishedEventArgs& e)
    {
        std::cout << e.result_ << "\n";

//         std::cout << "\n" << std::string(32, '-') << "\n"
//             << "Ran " << e.result_.count_ << " tests in "<< e.duration_.count() << "ms:\n"
//             << std::setw(4) << e.result_.success_count_ << " test(s) succeeded\n"
//             << std::setw(4) << e.result_.failure_count_ << " test(s) failed\n"
//             << std::setw(4) << e.result_.skip_count_ << " test(s) skipped\n"
//             << std::setw(4) << e.result_.error_count_ << " test(s) aborted\n"
//             << "\n"
//             << "result: " << e.result_.result_ << "\n"
//             << "message: " << e.result_.message_ << "\n"
//             << std::string(32, '-') << "\n";

        test_case_s.reset();
        test_case_f.reset();

    });

    tr.Run("");

    system("pause");

}

