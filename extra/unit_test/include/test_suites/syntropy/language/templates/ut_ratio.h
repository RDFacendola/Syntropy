
/// \file ut_math.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/math.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* RATIO TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Templates test fixture.
    struct RatioTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_ratio
        = Syntropy::UnitTest::MakeAutoUnitTest<RatioTestFixture>(
            u8"math.templates.language.syntropy")

    .TestCase(u8"The greatest common divisor of a number and 1 is 1.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 1>), 1);
        SYNTROPY_UNIT_EQUAL((GCD<1, 42>), 1);
    });

}

// ===========================================================================
