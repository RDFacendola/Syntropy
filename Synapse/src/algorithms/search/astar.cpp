#include "algorithms/search/astar.h"
#include "diagnostics/assert.h"

namespace syntropy::synapse::algo::search
{

	Node::Node(uint id)
		:_id(id)
	{		
	}

	void Node::operator=(const Node& other)
	{
		SYNTROPY_ASSERT(&other != nullptr);
		_id = other._id;
		_adjancencyMap.clear();
		for (auto&& pair : other._adjancencyMap)
		{
			_adjancencyMap.emplace(pair);
		}
	}

	bool Node::operator==(const Node& other) const
	{
		/// Don't bother continue the compare if ids or map size mismatch.
		if (_id == other._id && _adjancencyMap.size() == other._adjancencyMap.size())
		{
			auto iterator = _adjancencyMap.begin();
			auto otherIterator = other._adjancencyMap.begin();
			while (iterator != _adjancencyMap.end())
			{
				/// Compares both id and cost.
				if (*iterator != *otherIterator)
				{
					/// Same id but different cost!
					SYNTROPY_ASSERT(*iterator.first != *otherIterator.first);
					return false;
				}

				iterator++;
				otherIterator++;
			}
			return true;
		}
		return false;
	}


	bool Node::AreAdjacent(const Node& a, const Node& b)
	{
		/// Make that the nodes are valid
		SYNTROPY_ASSERT(&a != nullptr && &b != nullptr);

		return a._adjancencyMap.find(b._id) != a._adjancencyMap.end();
	}

	void Node::AddConnection(Node& other, uint cost)
	{
		SYNTROPY_ASSERT(&other != nullptr);
		/// Try add a connection to both this and other.
		_adjancencyMap.try_emplace(other._id, cost);
		other._adjancencyMap.try_emplace(_id, cost);
	}

}