
/// \file allocators.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include "memory/bytes.h"
#include "memory/memory_manager.h"
#include "memory/memory_meta.h"

#include "memory/allocators/linear_allocator.h"
#include "memory/allocators/scope_allocator.h"

#include "memory/allocators/segregated_allocator.h"

#include <vector>

/************************************************************************/
/* TEST SYNTROPY MEMORY ALLOCATORS                                      */
/************************************************************************/

/// \brief Test suite used to test Syntropy allocators.
class TestSyntropyMemoryAllocators : public syntropy::TestFixture
{
public:

    static std::vector<syntropy::TestCase> GetTestCases();

    TestSyntropyMemoryAllocators();

    /// \brief Test Syntropy memory context.
    void TestMemoryContext();

private:


};
