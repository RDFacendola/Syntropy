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
        constexpr auto Abs_10 = Templates::Abs<10>;
        constexpr auto Abs_0 = Templates::Abs<0>;

        SYNTROPY_UNIT_EQUAL(Abs_10, 10);
        SYNTROPY_UNIT_EQUAL(Abs_0, 0);
    })

    .TestCase("The absolute value of a negative number is the opposite of the number.", [](auto& fixture)
    {
        constexpr auto Abs_Minus10 = Templates::Abs<-10>;

        SYNTROPY_UNIT_EQUAL(Abs_Minus10, 10);
    })

    .TestCase("GCD returns the largest integer which divides both numbers.", [](auto& fixture)
    {
        constexpr auto GCD_45_25 = Templates::GCD<45, 25>;

        SYNTROPY_UNIT_EQUAL(GCD_45_25, 5);
    })

    .TestCase("GCD is commutative.", [](auto& fixture)
    {
        constexpr auto GCD_45_25 = Templates::GCD<45, 25>;
        constexpr auto GCD_25_45 = Templates::GCD<25, 45>;

        SYNTROPY_UNIT_EQUAL(GCD_45_25, 5);
        SYNTROPY_UNIT_EQUAL(GCD_25_45, 5);
    })

    .TestCase("The GCD of a number and itself is equal to that number.", [](auto& fixture)
    {
        constexpr auto GCD_47_47 = Templates::GCD<47, 47>;

        SYNTROPY_UNIT_EQUAL(GCD_47_47, 47);
    })

    .TestCase("The GCD of a number and 1 is equal to 1.", [](auto& fixture)
    {
        constexpr auto GCD_45_1 = Templates::GCD<45, 1>;
        constexpr auto GCD_1_42 = Templates::GCD<1, 42>;

        SYNTROPY_UNIT_EQUAL(GCD_45_1, 1);
        SYNTROPY_UNIT_EQUAL(GCD_1_42, 1);
    })

    .TestCase("The GCD of a number and 0 is equal to the number itself.", [](auto& fixture)
    {
        constexpr auto GCD_45_0 = Templates::GCD<45, 0>;

        SYNTROPY_UNIT_EQUAL(GCD_45_0, 45);
        SYNTROPY_UNIT_EQUAL(45, GCD_45_0);
    })

    .TestCase("The GCD of two prime numbers is 1.", [](auto& fixture)
    {
        constexpr auto GCD_17_23 = Templates::GCD<17, 23>;

        SYNTROPY_UNIT_EQUAL(GCD_17_23, 1);
    })

    .TestCase("The GCD of two coprime numbers is 1.", [](auto& fixture)
    {
        constexpr auto GCD_49_81 = Templates::GCD<49, 81>;

        SYNTROPY_UNIT_EQUAL(GCD_49_81, 1);
    })

    .TestCase("LCM returns the smallest integer number which is divisible by both numbers.", [](auto& fixture)
    {
        constexpr auto LCM_105_84 = Templates::LCM<105, 84>;

        SYNTROPY_UNIT_EQUAL(LCM_105_84, 420);
    })

    .TestCase("LCM of 0 is 0.", [](auto& fixture)
    {
        constexpr auto LCM_0_0 = Templates::LCM<0, 0>;

        SYNTROPY_UNIT_EQUAL(LCM_0_0, 0);
    })

    .TestCase("LCM is commutative.", [](auto& fixture)
    {
        constexpr auto LCM_105_84 = Templates::LCM<105, 84>;
        constexpr auto LCM_84_105 = Templates::LCM<84, 105>;

        SYNTROPY_UNIT_EQUAL(LCM_105_84, 420);
        SYNTROPY_UNIT_EQUAL(LCM_84_105, 420);
    })

    .TestCase("The LCM of a number and 1 is equal to the former.", [](auto& fixture)
    {
        constexpr auto LCM_45_1 = Templates::LCM<45, 1>;
        constexpr auto LCM_1_42 = Templates::LCM<1, 42>;

        SYNTROPY_UNIT_EQUAL(LCM_45_1, 45);
        SYNTROPY_UNIT_EQUAL(LCM_1_42, 42);
    })

    .TestCase("The LCM of a number and itself is equal to that number.", [](auto& fixture)
    {
        constexpr auto LCM_45_45 = Templates::LCM<45, 45>;

        SYNTROPY_UNIT_EQUAL(LCM_45_45, 45);
    })

    .TestCase("The LCM of two prime numbers is the product of both.", [](auto& fixture)
    {
        constexpr auto LCM_17_23 = Templates::LCM<17, 23>;

        SYNTROPY_UNIT_EQUAL(LCM_17_23, 391);
    });

}