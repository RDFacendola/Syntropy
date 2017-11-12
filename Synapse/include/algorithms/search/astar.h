/// \file astar.h
/// \brief This header is part of the synapse AI module. It contains different implementations of A*
///
/// \author Giuseppe Spizzico - 2017

#pragma once
#include <map>
#include <functional>
#include <string>
#include <type_traits>

namespace syntropy::synapse
{
	namespace algo
	{
		namespace search
		{
			using uint = unsigned int;

			/// \brief base class for a Node that can be used for A* search.			
			class Node
			{
			public:

				/// \brief Default constructor accepts unique identifier for a node.
				Node(uint id);

				virtual ~Node() = default;
								
				uint _id;

				/// \brief compares _id and adjacency map
				bool operator==(const Node& other) const;
				
				void operator=(const Node& other);

				/// \brief returns the name of this node as unique _id.
				virtual std::string GetName() const { return std::string(""+_id);}

				/// \brief virtual heuristic function to be implemented in derived classes. h(x)
				/// \param goal: the node to which you want to evaluate the cost.
				/// \return the heuristic cost.
				virtual uint GetEstimatedCost(const Node& goal) const = 0;

				/// \brief add a Node to the adjacency map with the appropriate cost
				/// \param other: the Node it is connected to
				/// \param cost: the actual cost to get to the node. f(x)
				void AddConnection(Node& other, uint cost);

				/// \brief add a Node to the adjacency map with the appropriate cost
				/// \param other: the Node it is connected to
				/// \param costFunction: the function that gets the cost of the node. f(x)
				/// \param arguments: additional arguments for costFunction
				template<typename ...TArgs>
				void AddConnection(Node& other, std::function<uint(TArgs...)> costFunction, TArgs&&... arguments);
				
				/// \brief return whether a and b are adjacent
				static bool AreAdjacent(const Node& a, const Node& b);

			protected:
				
				/// \brief adjancency map. std::pair<uint, uint> -> <Node._id, f(x)>
				std::map<uint, uint> _adjancencyMap;
			};

		}
	}
}

namespace syntropy::synapse::algo::search
{
	template<typename... TArgs>
	void Node::AddConnection(Node& other, std::function<uint(TArgs...)>  costFunction, TArgs&&... arguments)
	{
		AddConnection(other, costFunction(std::forward<TArgs>(arguments)...));
	}

}