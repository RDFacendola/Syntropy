#include "test/synapse/search.h"

#include "syntropy/unit_test/test_runner.h"
#include "syntropy/diagnostics/assert.h"

#include "synapse/algorithms/search/astar.h"

#include <algorithm>
#include <limits>
#include <cmath>

/************************************************************************/
/* GRAPH NODE                                                           */
/************************************************************************/

TestSynapseSearch::GraphNode::GraphNode(int32_t x, int32_t y)
    : x_(x)
    , y_(y)
{

}

void TestSynapseSearch::GraphNode::LinkTo(GraphNode& destination, float cost)
{
    neighbors_.emplace_back(&destination);
    costs_.emplace_back(cost);
}

const syntropy::Vector<TestSynapseSearch::GraphNode*>& TestSynapseSearch::GraphNode::GetNeighbors() const
{
    return neighbors_;
}

float TestSynapseSearch::GraphNode::GetLinkCost(const GraphNode& destination) const
{
    auto it = std::find(std::begin(neighbors_), std::end(neighbors_), &destination);

    if (it != std::end(neighbors_))
    {
        return costs_[std::distance(std::begin(neighbors_), it)];
    }
    else
    {
        return std::numeric_limits<float>::infinity();
    }
}

float TestSynapseSearch::GraphNode::GetDistance(const GraphNode& destination) const
{
    auto diff_x = x_ - destination.x_;
    auto diff_y = y_ - destination.y_;

    return std::sqrtf(float(diff_x * diff_x + diff_y * diff_y));
}

/************************************************************************/
/* GRAPH                                                                */
/************************************************************************/

TestSynapseSearch::GraphNode& TestSynapseSearch::Graph::AddNode(int32_t x, int32_t y)
{
    return *nodes_.emplace_back(std::make_unique<GraphNode>(x, y));
}

const TestSynapseSearch::GraphNode& TestSynapseSearch::Graph::GetNode(int32_t x, int32_t y) const
{
    auto it = std::find_if(std::begin(nodes_), std::end(nodes_), [x, y](const auto& node)
    {
        return node->x_ == x && node->y_ == y;
    });

    SYNTROPY_ASSERT(it != std::end(nodes_));

    return **it;
}

/************************************************************************/
/* TEST SYNAPSE SEARCH                                                  */
/************************************************************************/

syntropy::AutoTestSuite<TestSynapseSearch> suite("synapse.search");

syntropy::Vector<syntropy::TestCase> TestSynapseSearch::GetTestCases()
{
    return
    {
        { "astar", &TestSynapseSearch::TestAStar }
    };
}

void TestSynapseSearch::Before()
{
    graph_ = std::make_unique<Graph>();

    auto& n00 = graph_->AddNode(0, 0);
    auto& n06 = graph_->AddNode(0, 6);
    auto& n30 = graph_->AddNode(3, 0);
    auto& n36 = graph_->AddNode(3, 6);
    auto& n53 = graph_->AddNode(5, 3);
    auto& n59 = graph_->AddNode(5, 9);
    auto& n71 = graph_->AddNode(7, 1);
    auto& n75 = graph_->AddNode(7, 5);
    auto& n83 = graph_->AddNode(8, 3);
    auto& n99 = graph_->AddNode(9, 9);

    // Manhattan distance

    n00.LinkTo(n06, 6);
    n00.LinkTo(n30, 3);
    n06.LinkTo(n59, 8);
    n30.LinkTo(n36, 6);
    n30.LinkTo(n71, 5);
    n36.LinkTo(n06, 3);
    n36.LinkTo(n53, 5);
    n53.LinkTo(n83, 3);
    n59.LinkTo(n99, 4);
    n71.LinkTo(n53, 4);
    n71.LinkTo(n75, 4);
    n71.LinkTo(n83, 3);
    n75.LinkTo(n59, 6);
    n83.LinkTo(n75, 3);
}

void TestSynapseSearch::After()
{
    graph_.reset();
}

void TestSynapseSearch::TestAStar()
{
    auto& n00 = graph_->GetNode(0, 0);
    auto& n06 = graph_->GetNode(0, 6);
    auto& n59 = graph_->GetNode(5, 9);
    auto& n99 = graph_->GetNode(9, 9);

    SYNTROPY_UNIT_TRACE(auto neighbors = [](const auto& node) { return node.GetNeighbors(); });
    SYNTROPY_UNIT_TRACE(auto cost = [](const auto& source, const auto& destination) { return source.GetLinkCost(destination); });
    SYNTROPY_UNIT_TRACE(auto heuristic = [](const auto& source, const auto& destination) { return source.GetDistance(destination); });

    SYNTROPY_UNIT_ASSERT(syntropy::synapse::AStar(n00, n99, neighbors, cost, heuristic) == MakePath(n99, n59, n06, n00));
}
