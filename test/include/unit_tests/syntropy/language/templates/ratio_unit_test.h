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
        using Sum_3o6_2o5 = RatioSum<Ratio<3, 6>, Ratio<2, 5>>;

        SYNTROPY_UNIT_EQUAL(Sum_3o6_2o5::kNumerator, 9);
        SYNTROPY_UNIT_EQUAL(Sum_3o6_2o5::kDenominator, 10);
    })

    .TestCase("Adding zero to a ratio returns a ratio is equivalent to the latter.", [](auto& fixture)
    {
        using Sum_3o6_0o1 = RatioSum<Ratio<3, 6>, Ratio<0, 1>>;

        SYNTROPY_UNIT_EQUAL(Sum_3o6_0o1::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Sum_3o6_0o1::kDenominator, 2);

        using Sum_0o6_3o1 = RatioSum<Ratio<0, 6>, Ratio<3, 1>>;

        SYNTROPY_UNIT_EQUAL(Sum_0o6_3o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Sum_0o6_3o1::kDenominator, 1);
    })

    .TestCase("Subtracting a ratio from another ratio returns the unreduced difference of the twos.", [](auto& fixture)
    {
        using Difference_3o6_2o5 = RatioDifference<Ratio<3, 6>, Ratio<2, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_2o5::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_2o5::kDenominator, 10);
    })

    .TestCase("Subtracting zero from a ratio returns a ratio equivalent to the latter.", [](auto& fixture)
    {
        using Difference_3o6_0o1 = RatioDifference<Ratio<3, 6>, Ratio<0, 1>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o1::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o1::kDenominator, 2);

        using Difference_3o6_0o5 = RatioDifference<Ratio<3, 6>, Ratio<0, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o5::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o5::kDenominator, 2);
    })

    .TestCase("Subtracting a ratio from zero returns the negated ratio.", [](auto& fixture)
    {
        using Difference_0o1_3o1 = RatioDifference<Ratio<0, 1>, Ratio<3, 1>>;

        SYNTROPY_UNIT_EQUAL(Difference_0o1_3o1::kNumerator, -3);
        SYNTROPY_UNIT_EQUAL(Difference_0o1_3o1::kDenominator, 1);

        using Difference_0o2_3o5 = RatioDifference<Ratio<0, 2>, Ratio<3, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_0o2_3o5::kNumerator, -3);
        SYNTROPY_UNIT_EQUAL(Difference_0o2_3o5::kDenominator, 5);
    })

    .TestCase("Multiplying two ratios returns the unreduced product of both numerators and both denominators.", [](auto& fixture)
    {
        using Product_6o8_9o15 = RatioProduct<Ratio<6, 8>, Ratio<9, 15>>;

        SYNTROPY_UNIT_EQUAL(Product_6o8_9o15::kNumerator, 9);
        SYNTROPY_UNIT_EQUAL(Product_6o8_9o15::kDenominator, 20);

        using Product_Minus6o2_7o11 = RatioProduct<Ratio<-6, 2>, Ratio<7, 11>>;

        SYNTROPY_UNIT_EQUAL(Product_Minus6o2_7o11::kNumerator, -21);
        SYNTROPY_UNIT_EQUAL(Product_Minus6o2_7o11::kDenominator, 11);
    })

    .TestCase("Multiplying a ratio by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using Product_6o8_1o1 = RatioProduct<Ratio<6, 8>, Ratio<1, 1>>;

        SYNTROPY_UNIT_EQUAL(Product_6o8_1o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Product_6o8_1o1::kDenominator, 4);

        using Product_Minus4o4_13o17 = RatioProduct<Ratio<-4, 4>, Ratio<13, 17>>;

        SYNTROPY_UNIT_EQUAL(Product_Minus4o4_13o17::kNumerator, -13);
        SYNTROPY_UNIT_EQUAL(Product_Minus4o4_13o17::kDenominator, 17);
    })

    .TestCase("Dividing a ratio by another one returns the unreduced ratio equivalent their.", [](auto& fixture)
    {
        using Quotient_6o8_9o15 = RatioQuotient<Ratio<6, 8>, Ratio<9, 15>>;

        SYNTROPY_UNIT_EQUAL(Quotient_6o8_9o15::kNumerator, 5);
        SYNTROPY_UNIT_EQUAL(Quotient_6o8_9o15::kDenominator, 4);

        using Quotient_Minus6o2_7o11 = RatioQuotient<Ratio<-6, 2>, Ratio<7, 11>>;

        SYNTROPY_UNIT_EQUAL(Quotient_Minus6o2_7o11::kNumerator, -33);
        SYNTROPY_UNIT_EQUAL(Quotient_Minus6o2_7o11::kDenominator, 7);
    })

    .TestCase("Dividing a ratio by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using Quotient_6o8_1o1 = RatioQuotient<Ratio<6, 8>, Ratio<1, 1>>;

        SYNTROPY_UNIT_EQUAL(Quotient_6o8_1o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Quotient_6o8_1o1::kDenominator, 4);
    })
 
    .TestCase("Dividing one by a ratio returns the reciprocal of the latter.", [](auto& fixture)
    {
        using Quotient_1o1_6o7 = RatioQuotient<Ratio<1, 1>, Ratio<6, 7>>;

        SYNTROPY_UNIT_EQUAL(Quotient_1o1_6o7::kNumerator, 7);
        SYNTROPY_UNIT_EQUAL(Quotient_1o1_6o7::kDenominator, 6);
    });

}