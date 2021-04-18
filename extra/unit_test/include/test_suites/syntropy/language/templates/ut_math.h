
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
    /* MATH TEST FIXTURE                                                    */
    /************************************************************************/

    /// \brief Templates test fixture.
    struct MathTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_math
        = Syntropy::UnitTest::MakeAutoUnitTest<MathTestFixture>(
            u8"math.templates.language.syntropy")

    .TestCase(u8"The greatest common divisor of a number and 1 is 1.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 1>), 1);
        SYNTROPY_UNIT_EQUAL((GCD<1, 42>), 1);
    })

    .TestCase(u8"The greatest common divisor function is commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 28>), 14);
        SYNTROPY_UNIT_EQUAL((GCD<28, 42>), 14);
    })

    .TestCase(u8"The least common multiple of a number and 1 is the number "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<42, 1>), 42);
    })

    .TestCase(u8"The least common multiple function is commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<42, 35>), 210);
        SYNTROPY_UNIT_EQUAL((LCM<35, 42>), 210);
    });

}

// ===========================================================================
