
/// \file math_unit_test.h
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

    inline auto& math_unit_test
        = Syntropy::MakeAutoUnitTest<MathTestFixture>(
            u8"math.templates.language.syntropy")

    .TestCase(u8"The greatest common divisor of a number and 1 is 1.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 1>), 1);
        SYNTROPY_UNIT_EQUAL((GCD<1, 42>), 1);
    })

    .TestCase(u8"The greatest common divisor of a number and 0 is the number "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 0>), 42);
    })

    .TestCase(u8"The greatest common divisor of a number and itself is the "
              u8"number itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 42>), 42);
    })

    .TestCase(u8"The greatest common divisor function is commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<42, 28>), 14);
        SYNTROPY_UNIT_EQUAL((GCD<28, 42>), 14);
    })

    .TestCase(u8"The greatest common divisor of two prime numbers is 1.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<17, 23>), 1);
    })

    .TestCase(u8"The greatest common divisor of two coprime numbers is 1.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((GCD<49, 81>), 1);
    })

    .TestCase(u8"The least common multiple of a number and 1 is the number "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<42, 1>), 42);
    })

    .TestCase(u8"The least common multiple of a number and itself is the "
              u8"number itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<42, 42>), 42);
    })

    .TestCase(u8"The least common multiple function is commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<42, 35>), 210);
        SYNTROPY_UNIT_EQUAL((LCM<35, 42>), 210);
    })

    .TestCase(u8"The least common multiple of two prime numbers is their "
              u8"product.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((LCM<17, 23>), 391);
    });

}

// ===========================================================================
