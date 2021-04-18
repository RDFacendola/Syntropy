
/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/algorithms/compare.h"
#include "syntropy/core/algorithms/compare_extensions.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* COMPARE TEST FIXTURE                                                 */
    /************************************************************************/

    /// \brief Compare test fixture.
    struct CompareTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& compare_unit_test
        = Syntropy::UnitTest::MakeAutoUnitTest<CompareTestFixture>(
            u8"compare.algorithms.core.syntropy")

    .TestCase(u8"",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;
    });

}

// ===========================================================================
