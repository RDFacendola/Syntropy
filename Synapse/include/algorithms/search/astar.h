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
        using TCostType = decltype(heuristic_func(start, end));

        using TFrontierNode = std::tuple<const TNode*, TCostType>;

        struct FrontierNodeComparator
        {
            bool operator()(const TFrontierNode& a, const TFrontierNode& b)
            {
                return std::get<1>(a) > std::get<1>(b);
            }
        };

        std::priority_queue<TFrontierNode, std::vector<TFrontierNode>, FrontierNodeComparator> frontier;    // Nodes to be explored yet.
        std::unordered_map<const TNode*, const TNode*> came_from;                                           // Links each node with the predecessor having the lowest cost.
        std::unordered_map<const TNode*, TCostType> cost_to_node;                                           // Cost to reach each node from start.

        frontier.emplace(&start, TCostType(0));
        came_from.emplace(&start, &start);
        cost_to_node.emplace(&start, TCostType(0));

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

            // Add neighbours to the frontier.

            auto&& neighbours = adjacency_func(*current_node);                                              // Get the neighbours of current node.

            auto cost_to_current_node = cost_to_node.at(current_node);                                      // Cost to reach the current node from start.

            for (auto&& neighbour : neighbours)
            {
                auto new_cost = cost_to_current_node + cost_func(*current_node, *neighbour);                // Cost to node = Cost from start + cost from current to next = g(x)

                if ((cost_to_node.find(neighbour) == std::end(cost_to_node)) ||                             // The node hasn't been explored yet.
                    new_cost < cost_to_node.at(neighbour))                                                 // A better path to the node was found.
                {
                    cost_to_node.emplace(neighbour, new_cost);

                    auto priority = new_cost + heuristic_func(*neighbour, end);                             //  f(x) = g(x) + h(x)

                    frontier.emplace(neighbour, priority);

                    came_from.emplace(neighbour, current_node);
                }
            }
        }

        // Recreate the path.

        std::vector<const TNode*> path;

        if (came_from.find(&end) != std::end(came_from))
        {
            for (auto previous_node = &end; previous_node != &start; previous_node = came_from.at(previous_node))
            {
                path.emplace_back(previous_node);
            }
        }

        return path;
    }

}
