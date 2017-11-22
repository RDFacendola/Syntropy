
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

	const Graph graph(graph_vector, nMapWidth, nMapHeight);
	Node start(nStartX, nStartY);
	Node end(nTargetX, nTargetY);

	auto node_position = [](Node node, const Graph* graph) ->int
	{
		return (graph->GetWidth() * node.Y()) + node.X();
	};

	auto adjacency_func =
		[node_position](Node node, const Graph* graph) -> std::unordered_set<Node>
	{
		const int pos = node_position(node, graph);
		const auto size = graph->GetGraph().size();
		std::unordered_set<Node> neighbours;

		auto is_valid_node = [graph, size](int position) -> bool
		{
			return position >= 0 && position < size
				&& graph->GetGraph().at(position) != 0;
		};

		if (is_valid_node(pos))
		{
			const int down = pos + graph->GetWidth();
			if (down < size && is_valid_node(down))
			{
				neighbours.emplace(Node(down, graph));
			}

			const int top = pos - graph->GetWidth();
			if (top >= 0 && is_valid_node(top))
			{
				neighbours.emplace(Node(top, graph));
			}

			const int right = pos + 1;
			if (right % graph->GetWidth() > 0 && is_valid_node(right))
			{
				neighbours.emplace(Node(right, graph));
			}

			const int left = pos - 1;
			if (left % graph->GetWidth() >= 0 && is_valid_node(left))
			{
				neighbours.emplace(Node(left, graph));
			}			
		}

		return neighbours;
	};

	auto cost_func_distance = [](Node _start, Node _end) -> float
	{
		return static_cast<float>(std::abs(_start.X() - _end.X()) + std::abs(_start.Y() - _end.Y()));
	};

	auto heuristic_func_distance = [](Node _start, Node _end) -> float
	{
		return std::sqrtf((std::powf(float(_end.X()) - float(_start.X()), 2) + std::powf(float(_end.Y()) - float(_start.Y()), 2)));
	};

	auto path = syntropy::synapse::AStar(
		start,
		end,
		&graph,
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
		*Element = node_position(path.at(i), &graph);
	}

	auto print = [node_position, graph](Node n) { std::cout << "->" << node_position(n, const_cast<Graph*>(&graph)); };
	std::cout << "Path: " << std::endl;
	std::for_each(path.begin(), path.end(), [print, graph](const Node& n) {print(n); });
	std::cout << std::endl;

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

	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;

	auto t0 = Time::now();	
	auto result = FindPath(1, 2, 2, 1, pMap, 4, 3, pOutBuffer, nOutBufferSize);
	std::cout << "FindPath Output: " << result << std::endl;

	auto t1 = Time::now();
	ms d = std::chrono::duration_cast<ms>(t1 - t0);
	std::cout << "A* duration: " <<d.count() << "ms\n";
}

int main()
{

	SynapseTest();

    Initialize();

    ReflectionAndSerializationTest();

    AllocTest();

	MultithreadTest();

    system("pause");

}

