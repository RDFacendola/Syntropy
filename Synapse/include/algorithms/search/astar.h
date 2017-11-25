/// \file astar.h
/// \brief This header is part of the synapse AI module. It contains different implementations of A*
///
/// \author Giuseppe Spizzico - 19/11/2017

#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <queue>
#include <vector>
#include <tuple>
#include <algorithm>

namespace syntropy::synapse
{
	/// \brief Classic A* implementation, used for search.
	///
	/// \param start The starting node of the path.
	/// \param end The ending node of the path.
	/// \param adjacency_func The function that provides the list of nodes adjacent to a node in a graph. TAdjacencyFunc(TNode) -> Collection<const TNode*>
	/// \param cost_func The actual cost ( g(x) )from node A to node B. TCostFunc(const TNode& A, const TNode& B) -> TCostType
	/// \param heuristic_func The estimate cost ( h(x) ) from node A to node B. THeuristicFunc(const TNode& A, const TNode& B) -> TCostType
	/// \return the vector containing the path, in reverse order (from end to start).
	template<typename TNode, typename TAdjacencyFunc, typename TCostFunc, typename THeuristicFunc>
    std::vector<const TNode*> AStar(const TNode& start, const TNode& end, TAdjacencyFunc adjacency_func, TCostFunc cost_func, THeuristicFunc heuristic_func)
    {
        using TCostType = decltype(cost_func(start, end));

        using TFrontierNode = std::tuple<const TNode*, TCostType>;

        struct FrontierNodeComparator
        {
            bool operator()(const TFrontierNode& a, const TFrontierNode& b) const
            {
                return std::get<1>(a) > std::get<1>(b);
            }
        };

        std::priority_queue<TFrontierNode, std::vector<TFrontierNode>, FrontierNodeComparator> frontier;    // Nodes to be explored yet.

        std::unordered_map<const TNode*, std::tuple<const TNode*, TCostType>> node_map;                     // Associate each node with the predecessor having the lowest cost and the cost needed to reach it from start.

        frontier.emplace(&start, TCostType(0));

        node_map.emplace(&start, std::make_tuple(&start, TCostType(0)));

        while (!frontier.empty())
        {
            // Extract the node with the lowest cost from the frontier.

            auto current_node = std::get<0>(frontier.top());

            frontier.pop();

            // Check if the end node was found.

            if (current_node == &end)
            {
                break;
            }

            // Add neighbors to the frontier.

            auto cost_to_current_node = std::get<1>(node_map.at(current_node));                             // Cost to reach the current node from start.

            for (auto&& neighbour : adjacency_func(*current_node))
            {
                auto new_cost = cost_to_current_node + cost_func(*current_node, *neighbour);                // Cost to node = Cost from start + cost from current to next = g(x)

                auto neighbor_it = node_map.find(neighbour);

                if ((neighbor_it == std::end(node_map)) || new_cost < std::get<1>(neighbor_it->second))     // Either the neighbor was never considered, or a better path to it was found.
                {
                    frontier.emplace(neighbour, new_cost + heuristic_func(*neighbour, end));                //  f(x) = g(x) + h(x)

                    node_map.emplace(neighbour, std::make_tuple(current_node, new_cost));
                }
            }
        }

        // Recreate the path (reversed).

        std::vector<const TNode*> path;

        if (node_map.find(&end) != std::end(node_map))
        {
            for (auto previous_node = &end; previous_node != &start; previous_node = std::get<0>(node_map.at(previous_node)))
            {
                path.emplace_back(previous_node);
            }
        }

        return path;
    }

}
