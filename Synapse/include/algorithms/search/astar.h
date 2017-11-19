/// \file astar.h
/// \brief This header is part of the synapse AI module. It contains different implementations of A*
///
/// \author Giuseppe Spizzico - 19/11/2017

#pragma once
#include <map>
#include <string>
#include <type_traits>
#include <queue>

namespace syntropy::synapse
{
	/// \brief Classic A* implementation, used for search.
	///
	/// \param start The starting node of the path.
	/// \param end The ending node of the path.
	/// \param graph The graph in which the nodes are contained.
	/// \param adjacency_func The function that provides the set of nodes adjacent to a node in a graph. TAdjacencyFunc(TNode, TGraph) -> IterableContainer
	/// \param cost_func The actual cost ( g(x) )from node A to node B. TCostFunc(TNode A, TNode B) -> TCostType
	/// \param heuristic_func The estimate cost ( h(x) ) from node A to node B. THeuristicFunc(TNode A, TNode B) -> TCostType
	/// \return the vector containing the path.

	template<typename TNode, typename TGraph, typename TAdjacencyFunc, typename TCostFunc, typename THeuristicFunc>
	std::vector<TNode> AStar(
		const TNode& start,
		const TNode& end,
		const TGraph& graph,
		TAdjacencyFunc adjacency_func,
		TCostFunc cost_func,
		THeuristicFunc heuristic_func)
	{
		typedef decltype(heuristic_func(start, end)) TCostType;
		static_assert(std::is_same_v<TCostType, decltype(cost_func(start, start))>, "syntropy::synapse::AStar: THeuristicFunc and TCostFunc must have the same return type.");
		static_assert(std::is_arithmetic_v<TCostType>, "syntropy::synapse::AStar: the return type of TCostFunc and THeuristicFunc must be arithmetic.");

		///  Utility struct used to assign a priority to a node.
		struct FNode
		{
		public:
			FNode(const TNode& node, TCostType priority)
				: node_(node), priority_(priority)
			{
			};

			bool operator>(const FNode& other) const
			{
				return priority_ > other.priority_;
			};

			const TNode& GetNode() const { return node_; };

		private:
			TNode node_;
			TCostType priority_;
		};
		/// List of the nodes to be explored, sorted by priority ( less is best ).
		std::priority_queue<FNode, std::vector<FNode>, std::greater<FNode>> frontier;
		frontier.push(FNode(start, static_cast<TCostType>(0)));

		/// Map of links from node to node with the lowest cost.
		std::unordered_map<TNode, TNode> came_from;
		came_from.emplace(start, start);

		/// Mapping the cost from start for each node.
		std::unordered_map<TNode, TCostType> cost_to_node;
		cost_to_node.emplace(start, static_cast<TCostType>(0));

		while (!frontier.empty())
		{
			/// Get the node with the lowest cost.
			FNode current = frontier.top();
			frontier.pop();
			
			const TNode& current_node = current.GetNode();
			if (current_node == end)
			{
				break; /// Stop exploring if the end node has been found.
			}
			auto neighbours = adjacency_func(current_node, graph); /// Get the neighbours of current node.			
			for (auto&& neighbour : neighbours)
			{
				TCostType new_cost = cost_to_node.at(current_node) + cost_func(current_node, neighbour); /// actual cost to node = Cost from start + cost from current to next = g(x)
				if (!cost_to_node.count(neighbour) || new_cost < (cost_to_node.at(neighbour)))
				{
					cost_to_node.emplace(neighbour, new_cost);
					TCostType priority = new_cost + heuristic_func(neighbour, end); ///  f(x) = g(x) + h(x)
					frontier.push(FNode(neighbour, priority));
					came_from.emplace(neighbour, current_node);
				}
			}
		}

		/// Recreate the path.
		std::vector<TNode> path;
		if (came_from.count(end))
		{
			auto previous_node = came_from.at(end);
			while (previous_node != start)
			{
				path.insert(path.begin(), previous_node);
				previous_node = came_from.at(previous_node);
			}

			path.insert(path.end(), end);

		}
		return path;
	}		
}