
/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/toolset/swap.h"
#include "syntropy/core/toolset/swap_extensions.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* SWAP TEST FIXTURE                                                    */
    /************************************************************************/

    /// \brief Swap test fixture.
    struct SwapTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& swap_unit_test
        = Syntropy::UnitTest::MakeAutoUnitTest<SwapTestFixture>(
            u8"swap.algorithms.core.syntropy")

    .TestCase(u8"",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;
    });

}

// ===========================================================================
