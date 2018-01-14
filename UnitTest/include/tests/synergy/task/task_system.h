
/// \file task_system.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include <vector>

/************************************************************************/
/* TEST SYNERGY TASK SYSTEM                                             */
/************************************************************************/

/// \brief Test suite used to test Synergy task system.
class TestSynergyTaskSystem : public syntropy::TestFixture
{
public:

    static std::vector<syntropy::TestCase> GetTestCases();

    /// \brief Test Synergy task graph.
    void TestTaskGraph();

private:

};
