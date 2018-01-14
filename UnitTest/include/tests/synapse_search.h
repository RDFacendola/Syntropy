
/// \file synapse_search.h
///
/// \author Raffaele D. Facendola - 2018

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include <vector>
#include <unordered_map>
#include <memory>

/************************************************************************/
/* TEST SYNAPSE SEARCH                                                  */
/************************************************************************/

/// \brief Test suite used to test search functionalities within Synapse.
class TestSynapseSearch : public syntropy::TestFixture
{
public:

    static std::vector<syntropy::TestCase> GetTestCases();

    /// \brief Setup the graph to search in.
    void Before();

    /// \brief Tear down the graph to search in.
    void After();

    /// \brief Test A* implementation.
    void TestAStar();

private:

    /// \brief A node in 2D space.
    struct GraphNode
    {
        int32_t x_;                                             ///< \brief X coordinate.
        int32_t y_;                                             ///< \brief Y coordinate.

        /// \brief Create a new node.
        GraphNode(int32_t x, int32_t y);

        /// \brief Link this node to another one.
        /// \param destination Node to link this node to.
        /// \param cost Cost needed to get to the destination node from this one.
        void LinkTo(GraphNode& destination, float cost);

        /// \brief Get the list of neighbors this node is linked to.
        const std::vector<GraphNode*>& GetNeighbors() const;

        /// \brief Get the cost needed to get from this node to destination.
        /// \param destination Destination to reach.
        /// \return Returns the cost to get from this node to destination.
        float GetLinkCost(const GraphNode& destination) const;

        /// \brief Get the euclidean distance from this node to another one.
        /// \param destination Node to get the distance from.
        float GetDistance(const GraphNode& destination) const;

    private:

        std::vector<GraphNode*> neighbors_;                     ///< \brief List of this node's neighbors.
        std::vector<float> costs_;                              ///< \brief Cost to get to each neighbor node. Must have the same size as neighbors_.
    };

    /// \brief A graph in 2D space.
    class Graph
    {
    public:

        /// \brief Add a new node to the graph.
        /// \param x X coordinate.
        /// \param y Y coordinate.
        /// \return Returns the added node.
        GraphNode& AddNode(int32_t x, int32_t y);

        /// \brief Get a node at given coordinates.
        /// \param x X coordinate.
        /// \param y Y coordinate.
        /// \return Returns the node at given coordinates.
        const GraphNode& GetNode(int32_t x, int32_t y) const;

    private:

        std::vector<std::unique_ptr<GraphNode>> nodes_;     ///< \brief Nodes in the graph.
    };

    /// \brief Create a path on a graph.
    template <typename... TNode>
    std::vector<const GraphNode*> MakePath(TNode&... ts)
    {
        return { &ts... };
    }

    std::unique_ptr<Graph> graph_;                          ///< \brief Graph used to test search algorithms on.

};

