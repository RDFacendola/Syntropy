/// \file rational_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Templates::UnitTest
{
    /************************************************************************/
    /* RATIO TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Ratio test fixture.
    struct RatioTestFixture{};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kRatioTest = MakeAutoUnitTest<RatioTestFixture>("ratio.templates.language.syntropy")

    .TestCase("Adding two ratios together returns the unreduced sum of the twos.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<3, 6>, Ratio<2, 5>>::kNumerator), 9);
        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<3, 6>, Ratio<2, 5>>::kDenominator), 10);
    })

    .TestCase("Adding zero to a ratio returns a ratio is equivalent to the latter.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<3, 6>, Ratio<0, 1>>::kNumerator), 1);
        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<3, 6>, Ratio<0, 1>>::kDenominator), 2);

        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<0, 6>, Ratio<3, 1>>::kNumerator), 3);
        SYNTROPY_UNIT_EQUAL((RatioSum<Ratio<0, 6>, Ratio<3, 1>>::kDenominator), 1);
    })

    .TestCase("Subtracting a ratio from another ratio returns the unreduced difference of the twos.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<2, 5>>::kNumerator), 1);
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<2, 5>>::kDenominator), 10);
    })

    .TestCase("Subtracting zero from a ratio returns a ratio equivalent to the latter.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<0, 1>>::kNumerator), 1);
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<0, 1>>::kDenominator), 2);

        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<0, 5>>::kNumerator), 1);
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<3, 6>, Ratio<0, 5>>::kDenominator), 2);
    })

    .TestCase("Subtracting a ratio from zero returns the negated ratio.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<0, 1>, Ratio<3, 1>>::kNumerator), -3);
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<0, 1>, Ratio<3, 1>>::kDenominator), 1);

        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<0, 2>, Ratio<3, 5>>::kNumerator), -3);
        SYNTROPY_UNIT_EQUAL((RatioDifference<Ratio<0, 2>, Ratio<3, 5>>::kDenominator), 5);
    })

    .TestCase("Multiplying two ratios returns the unreduced product of both numerators and both denominators.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<6, 8>, Ratio<9, 15>>::kNumerator), 9);
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<6, 8>, Ratio<9, 15>>::kDenominator), 20);

        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<-6, 2>, Ratio<7, 11>>::kNumerator), -21);
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<-6, 2>, Ratio<7, 11>>::kDenominator), 11);
    })

    .TestCase("Multiplying a ratio by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<6, 8>, Ratio<1, 1>>::kNumerator), 3);
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<6, 8>, Ratio<1, 1>>::kDenominator), 4);

        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<-4, 4>, Ratio<13, 17>>::kNumerator), -13);
        SYNTROPY_UNIT_EQUAL((RatioProduct<Ratio<-4, 4>, Ratio<13, 17>>::kDenominator), 17);
    })

    .TestCase("Dividing a ratio by another one returns the unreduced ratio equivalent their.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<6, 8>, Ratio<9, 15>>::kNumerator), 5);
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<6, 8>, Ratio<9, 15>>::kDenominator), 4);

        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<-6, 2>, Ratio<7, 11>>::kNumerator), -33);
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<-6, 2>, Ratio<7, 11>>::kDenominator), 7);
    })

    .TestCase("Dividing a ratio by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<6, 8>, Ratio<1, 1>>::kNumerator), 3);
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<6, 8>, Ratio<1, 1>>::kDenominator), 4);
    })
 
    .TestCase("Dividing one by a ratio returns the reciprocal of the latter.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<1, 1>, Ratio<6, 7>>::kNumerator), 7);
        SYNTROPY_UNIT_EQUAL((RatioQuotient<Ratio<1, 1>, Ratio<6, 7>>::kDenominator), 6);
    });

}