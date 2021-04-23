
/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/toolset/swap.h"

// ===========================================================================

namespace UnitTest
{
    namespace Sy = Syntropy;

    /************************************************************************/
    /* SWAP TEST FIXTURE                                                    */
    /************************************************************************/

    /// \brief Swap test fixture.
    struct SwapTestFixture
    {

    };

    struct Banane
    {
        void Swap(Sy::Mutable<Banane>)
        {

        }
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
        auto a = Banane{};
        auto b = Banane{};

        Sy::Toolset::Swap(a, b);
    });

}

// ===========================================================================
