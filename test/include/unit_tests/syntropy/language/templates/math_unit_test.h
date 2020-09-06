/// \file math_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::unit_test
{
    /************************************************************************/
    /* RATIONAL TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Rational test fixture.
    struct MathTemplateLanguageSyntropyFixture {};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& math_template_language_syntropy_unit_test = MakeAutoUnitTest<MathTemplateLanguageSyntropyFixture>("math.templates.language.syntropy")

    .TestCase("GCD returns the largest integer which divides both numbers.", [](auto& fixture)
    {
        constexpr auto templates_GCD_45_25 = Templates::GCD<45, 25>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_45_25, 5);
    })

    .TestCase("GCD is commutative.", [](auto& fixture)
    {
        constexpr auto templates_GCD_45_25 = Templates::GCD<45, 25>;
        constexpr auto templates_GCD_25_45 = Templates::GCD<25, 45>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_45_25, 5);
        SYNTROPY_UNIT_EQUAL(templates_GCD_25_45, 5);
    })

    .TestCase("The GCD of a number and itself is equal to that number.", [](auto& fixture)
    {
        constexpr auto templates_GCD_47_47 = Templates::GCD<47, 47>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_47_47, 47);
    })

    .TestCase("The GCD of a number and 1 is equal to 1.", [](auto& fixture)
    {
        constexpr auto templates_GCD_45_1 = Templates::GCD<45, 1>;
        constexpr auto templates_GCD_1_42 = Templates::GCD<1, 42>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_45_1, 1);
        SYNTROPY_UNIT_EQUAL(templates_GCD_1_42, 1);
    })

    .TestCase("The GCD of a number and 0 is equal to the number itself.", [](auto& fixture)
    {
        constexpr auto templates_GCD_45_0 = Templates::GCD<45, 0>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_45_0, 45);
        SYNTROPY_UNIT_EQUAL(45, templates_GCD_45_0);
    })

    .TestCase("The GCD of two prime numbers is 1.", [](auto& fixture)
    {
        constexpr auto templates_GCD_17_23 = Templates::GCD<17, 23>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_17_23, 1);
    })

    .TestCase("The GCD of two coprime numbers is 1.", [](auto& fixture)
    {
        constexpr auto templates_GCD_49_81 = Templates::GCD<49, 81>;

        SYNTROPY_UNIT_EQUAL(templates_GCD_49_81, 1);
    })

    .TestCase("LCM returns the smallest integer number which is divisible by both numbers.", [](auto& fixture)
    {
        constexpr auto templates_LCM_105_84 = Templates::LCM<105, 84>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_105_84, 420);
    })

    .TestCase("LCM of 0 is 0.", [](auto& fixture)
    {
        constexpr auto templates_LCM_0_0 = Templates::LCM<0, 0>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_0_0, 0);
    })

    .TestCase("LCM is commutative.", [](auto& fixture)
    {
        constexpr auto templates_LCM_105_84 = Templates::LCM<105, 84>;
        constexpr auto templates_LCM_84_105 = Templates::LCM<84, 105>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_105_84, 420);
        SYNTROPY_UNIT_EQUAL(templates_LCM_84_105, 420);
    })

    .TestCase("The LCM of a number and 1 is equal to the former.", [](auto& fixture)
    {
        constexpr auto templates_LCM_45_1 = Templates::LCM<45, 1>;
        constexpr auto templates_LCM_1_42 = Templates::LCM<1, 42>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_45_1, 45);
        SYNTROPY_UNIT_EQUAL(templates_LCM_1_42, 42);
    })

    .TestCase("The LCM of a number and itself is equal to that number.", [](auto& fixture)
    {
        constexpr auto templates_LCM_45_45 = Templates::LCM<45, 45>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_45_45, 45);
    })

    .TestCase("The LCM of two prime numbers is the product of both.", [](auto& fixture)
    {
        constexpr auto templates_LCM_17_23 = Templates::LCM<17, 23>;

        SYNTROPY_UNIT_EQUAL(templates_LCM_17_23, 391);
    });

}