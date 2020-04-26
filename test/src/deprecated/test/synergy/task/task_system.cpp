#include "test/synergy/task/task_system.h"

#include "syntropy/unit_test/test_runner.h"

#include "syntropy/platform/system.h"
#include "syntropy/platform/threading.h"

#include "synergy/task/scheduler.h"

#define COUNT 1 << 16

struct Maxer
{
    Maxer(syntropy::Vector<int>& numbers, size_t begin, size_t end, int* max)
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

        syntropy::synergy::CreateTaskContinuation({ l, r },
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

    syntropy::Vector<int>* numbers_;
    size_t begin_ = 0;
    size_t end_ = 0;
    int* max_ = 0;
};

/************************************************************************/
/* TEST SYNERGY TASK SYSTEM                                             */
/************************************************************************/

syntropy::AutoTestSuite<TestSynergyTaskSystem> suite("synergy.task.tasksystem");

syntropy::Vector<syntropy::TestCase> TestSynergyTaskSystem::GetTestCases()
{
    return
    {
        { "task graph", &TestSynergyTaskSystem::TestTaskGraph }
    };
}

void TestSynergyTaskSystem::TestTaskGraph()
{
    SYNTROPY_UNIT_SKIP("Work in progress");

    using namespace std::literals::chrono_literals;

    const size_t count = COUNT;

    syntropy::Vector<int> numbers(count);

    srand(0);       // Be sure the random sequence stays the same for different runs.

    for (auto&& number : numbers)
    {
        number = rand() % 65536;
    }

    int max;

    syntropy::synergy::GetScheduler().Initialize();

    syntropy::synergy::DetachTask(
        [&numbers, &max]()
    {
        std::cout << "Starting: " << std::this_thread::get_id() << "\n";

        syntropy::synergy::EmplaceTask<Maxer>({}, numbers, 0, numbers.size(), &max);
    });

    system("pause");
}