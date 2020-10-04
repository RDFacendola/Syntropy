#/// \file math_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Templates::UnitTest
{
    /************************************************************************/
    /* RATIONAL TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Rational test fixture.
    struct MathTestFixture {};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kMathTest = MakeAutoUnitTest<MathTestFixture>("math.templates.language.syntropy")

    .TestCase("The absolute value of a positive or zero number is the number itself.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::Abs<10>), 10);
        SYNTROPY_UNIT_EQUAL((Templates::Abs<0>), 0);
    })

    .TestCase("The absolute value of a negative number is the opposite of the number.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::Abs<-10>), 10);
    })

    .TestCase("GCD returns the largest integer which divides both numbers.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<45, 25>), 5);
    })

    .TestCase("GCD is commutative.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<45, 25>), 5);
        SYNTROPY_UNIT_EQUAL((Templates::GCD<25, 45>), 5);
    })

    .TestCase("The GCD of a number and itself is equal to that number.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<47, 47>), 47);
    })

    .TestCase("The GCD of a number and 1 is equal to 1.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<45, 1>), 1);
        SYNTROPY_UNIT_EQUAL((Templates::GCD<1, 42>), 1);
    })

    .TestCase("The GCD of a number and 0 is equal to the number itself.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<45, 0>), 45);
        SYNTROPY_UNIT_EQUAL(45, (Templates::GCD<45, 0>));
    })

    .TestCase("The GCD of two prime numbers is 1.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<17, 23>), 1);
    })

    .TestCase("The GCD of two coprime numbers is 1.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::GCD<49, 81>), 1);
    })

    .TestCase("LCM returns the smallest integer number which is divisible by both numbers.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<105, 84>), 420);
    })

    .TestCase("LCM of 0 is 0.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<0, 0>), 0);
    })

    .TestCase("LCM is commutative.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<105, 84>), 420);
        SYNTROPY_UNIT_EQUAL((Templates::LCM<84, 105>), 420);
    })

    .TestCase("The LCM of a number and 1 is equal to the former.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<45, 1>), 45);
        SYNTROPY_UNIT_EQUAL((Templates::LCM<1, 42>), 42);
    })

    .TestCase("The LCM of a number and itself is equal to that number.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<45, 45>), 45);
    })

    .TestCase("The LCM of two prime numbers is the product of both.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::LCM<17, 23>), 391);
    });

}