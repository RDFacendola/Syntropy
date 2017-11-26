
// ProtoDW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <exception>
#include <chrono>

#include "unit1.h"

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

#include "patterns/observable.h"

#include "time/calendar.h"
#include "time/timer.h"

#include "synergy.h"
#include "task/scheduler.h"

#include "algorithms/search/astar.h"

syntropy::diagnostics::Context Root;

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

void MultithreadTest()
{
    using namespace std::literals::chrono_literals;

    syntropy::synergy::DetachTask(
    []()
    {
        std::cout << "Root " << std::this_thread::get_id() << "\n";

        std::this_thread::sleep_for(2s);

        auto a = syntropy::synergy::CreateTask(
        std::initializer_list<std::shared_ptr<syntropy::synergy::Task>>{},
        []()
        {
            std::cout << "Left " << std::this_thread::get_id() << "\n";
        });

        auto b =syntropy::synergy::CreateTask(
        std::initializer_list<std::shared_ptr<syntropy::synergy::Task>>{},
        []()
        {
            std::cout << "Right " << std::this_thread::get_id() << "\n";
        });

        auto c = syntropy::synergy::CreateTask(
        std::initializer_list<std::shared_ptr<syntropy::synergy::Task>>{a,b},
            []()
        {
            std::cout << "Leaf " << std::this_thread::get_id() << "\n";
        });
    });

    std::cout << "zzz... " << std::this_thread::get_id() << "\n";

    std::this_thread::sleep_for(10s);

    std::cout << "...zzz " << std::this_thread::get_id() << "\n";

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
    
    float time = timer.Stop() / 10000.0f;
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

int main()
{
    std::cout << syntropy::Calendar::GetDate() << " - " << syntropy::Calendar::GetTimeOfDay() << "\n";

    SynapseTest();

 //   Initialize();

 //   ReflectionAndSerializationTest();

 //   AllocTest();

	//MultithreadTest();

    system("pause");

}

