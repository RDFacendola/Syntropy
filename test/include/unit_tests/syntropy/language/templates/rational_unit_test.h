/// \file rational_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/rational.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Templates::UnitTest
{
    /************************************************************************/
    /* RATIONAL TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Rational test fixture.
    struct RationalTestFixture{};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kRationalTest = MakeAutoUnitTest<RationalTestFixture>("rational.templates.language.syntropy")

    .TestCase("Adding two rationals together returns the unreduced sum of the twos.", [](auto& fixture)
    {
        using sum_3over6_2over5 = RationalSum<Rational<3, 6>, Rational<2, 5>>;

        SYNTROPY_UNIT_EQUAL(sum_3over6_2over5::kNumerator, 27);
        SYNTROPY_UNIT_EQUAL(sum_3over6_2over5::kDenominator, 30);
    })

    .TestCase("Adding zero to a rational returns a rational is equivalent to the latter.", [](auto& fixture)
    {
        using sum_3over6_0over1 = RationalSum<Rational<3, 6>, Rational<0, 1>>;

        SYNTROPY_UNIT_EQUAL(sum_3over6_0over1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(sum_3over6_0over1::kDenominator, 6);

        using sum_0over6_3over1 = RationalSum<Rational<0, 6>, Rational<3, 1>>;

        SYNTROPY_UNIT_EQUAL(sum_0over6_3over1::kNumerator, 18);
        SYNTROPY_UNIT_EQUAL(sum_0over6_3over1::kDenominator, 6);
    })

    .TestCase("Subtracting a rational from another rational returns the unreduced difference of the twos.", [](auto& fixture)
    {
        using difference_3over6_2over5 = RationalDifference<Rational<3, 6>, Rational<2, 5>>;

        SYNTROPY_UNIT_EQUAL(difference_3over6_2over5::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(difference_3over6_2over5::kDenominator, 30);
    })

    .TestCase("Subtracting zero from a rational returns a rational equivalent to the latter.", [](auto& fixture)
    {
        using difference_3over6_0over1 = RationalDifference<Rational<3, 6>, Rational<0, 1>>;

        SYNTROPY_UNIT_EQUAL(difference_3over6_0over1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(difference_3over6_0over1::kDenominator, 6);

        using difference_3over6_0over5 = RationalDifference<Rational<3, 6>, Rational<0, 5>>;

        SYNTROPY_UNIT_EQUAL(difference_3over6_0over5::kNumerator, 15);
        SYNTROPY_UNIT_EQUAL(difference_3over6_0over5::kDenominator, 30);
    })

    .TestCase("Subtracting a rational from zero returns the negated rational.", [](auto& fixture)
    {
        using difference_0over1_3over1 = RationalDifference<Rational<0, 1>, Rational<3, 1>>;

        SYNTROPY_UNIT_EQUAL(difference_0over1_3over1::kNumerator, -3);
        SYNTROPY_UNIT_EQUAL(difference_0over1_3over1::kDenominator, 1);

        using difference_0over2_3over5 = RationalDifference<Rational<0, 2>, Rational<3, 5>>;

        SYNTROPY_UNIT_EQUAL(difference_0over2_3over5::kNumerator, -6);
        SYNTROPY_UNIT_EQUAL(difference_0over2_3over5::kDenominator, 10);
    })

    .TestCase("Multiplying two rationals returns the unreduced product of both numerators and both denominators.", [](auto& fixture)
    {
        using product_6over8_9over15 = RationalProduct<Rational<6, 8>, Rational<9, 15>>;

        SYNTROPY_UNIT_EQUAL(product_6over8_9over15::kNumerator, 54);
        SYNTROPY_UNIT_EQUAL(product_6over8_9over15::kDenominator, 120);

        using product_minus6over2_7over11 = RationalProduct<Rational<-6, 2>, Rational<7, 11>>;

        SYNTROPY_UNIT_EQUAL(product_minus6over2_7over11::kNumerator, -42);
        SYNTROPY_UNIT_EQUAL(product_minus6over2_7over11::kDenominator, 22);
    })

    .TestCase("Multiplying a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using product_6over8_1over1 = RationalProduct<Rational<6, 8>, Rational<1, 1>>;

        SYNTROPY_UNIT_EQUAL(product_6over8_1over1::kNumerator, 6);
        SYNTROPY_UNIT_EQUAL(product_6over8_1over1::kDenominator, 8);

        using product_minus4over4_13over17 = RationalProduct<Rational<-4, 4>, Rational<13, 17>>;

        SYNTROPY_UNIT_EQUAL(product_minus4over4_13over17::kNumerator, -52);
        SYNTROPY_UNIT_EQUAL(product_minus4over4_13over17::kDenominator, 68);
    })

    .TestCase("Dividing a rational by another one returns the unreduced rational equivalent their.", [](auto& fixture)
    {
        using quotient_6over8_9over15 = RationalQuotient<Rational<6, 8>, Rational<9, 15>>;

        SYNTROPY_UNIT_EQUAL(quotient_6over8_9over15::kNumerator, 90);
        SYNTROPY_UNIT_EQUAL(quotient_6over8_9over15::kDenominator, 72);

        using quotient_minus6over2_7over11 = RationalQuotient<Rational<-6, 2>, Rational<7, 11>>;

        SYNTROPY_UNIT_EQUAL(quotient_minus6over2_7over11::kNumerator, -66);
        SYNTROPY_UNIT_EQUAL(quotient_minus6over2_7over11::kDenominator, 14);
    })

    .TestCase("Dividing a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using quotient_6over8_1over1 = RationalQuotient<Rational<6, 8>, Rational<1, 1>>;

        SYNTROPY_UNIT_EQUAL(quotient_6over8_1over1::kNumerator, 6);
        SYNTROPY_UNIT_EQUAL(quotient_6over8_1over1::kDenominator, 8);
    })
 
    .TestCase("Dividing one by a rational returns the reciprocal of the latter.", [](auto& fixture)
    {
        using quotient_1over1_6over7 = RationalQuotient<Rational<1, 1>, Rational<6, 7>>;

        SYNTROPY_UNIT_EQUAL(quotient_1over1_6over7::kNumerator, 7);
        SYNTROPY_UNIT_EQUAL(quotient_1over1_6over7::kDenominator, 6);
    });

}