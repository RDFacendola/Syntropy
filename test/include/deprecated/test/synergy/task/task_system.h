
/// \file task_system.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/unit_test/test_fixture.h"
#include "syntropy/unit_test/test_case.h"

#include "syntropy/containers/vector.h"

/************************************************************************/
/* TEST SYNERGY TASK SYSTEM                                             */
/************************************************************************/

/// \brief Test suite used to test Synergy task system.
class TestSynergyTaskSystem : public syntropy::TestFixture
{
public:

    static syntropy::Vector<syntropy::TestCase> GetTestCases();

    /// \brief Test Synergy task graph.
    void TestTaskGraph();

private:

};
