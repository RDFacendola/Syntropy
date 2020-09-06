/// \file rational_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/rational.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::unit_test
{
    /************************************************************************/
    /* RATIONAL TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Rational test fixture.
    struct RationalTemplatesLanguageSyntropyFixture{};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& rational_templates_language_syntropy_unit_test = MakeAutoUnitTest<RationalTemplatesLanguageSyntropyFixture>("rational.templates.language.syntropy")

    .TestCase("Adding two rationals together returns the unreduced sum of the twos.", [](auto& fixture)
    {
        constexpr auto templates_sum_3over6_2over5_numerator = Templates::RationalSum<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::kNumerator;
        constexpr auto templates_sum_3over6_2over5_denominator = Templates::RationalSum<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_sum_3over6_2over5_numerator, 27);
        SYNTROPY_UNIT_EQUAL(templates_sum_3over6_2over5_denominator, 30);
    })

    .TestCase("Adding zero to a rational returns a rational is equivalent to the latter.", [](auto& fixture)
    {
        auto templates_sum_3over6_0over1_numerator = Templates::RationalSum<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::kNumerator;
        auto templates_sum_3over6_0over1_denominator = Templates::RationalSum<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_sum_3over6_0over1_numerator, 3);
        SYNTROPY_UNIT_EQUAL(templates_sum_3over6_0over1_denominator, 6);

        auto templates_sum_0over6_3over1_numerator = Templates::RationalSum<Templates::Rational<0, 6>, Templates::Rational<3, 1>>::kNumerator;
        auto templates_sum_0over6_3over1_denominator = Templates::RationalSum<Templates::Rational<0, 6>, Templates::Rational<3, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_sum_0over6_3over1_numerator, 18);
        SYNTROPY_UNIT_EQUAL(templates_sum_0over6_3over1_denominator, 6);
    })

    .TestCase("Subtracting a rational from another rational returns the unreduced difference of the twos.", [](auto& fixture)
    {
        auto templates_difference_3over6_2over5_numerator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::kNumerator;
        auto templates_difference_3over6_2over5_denominator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_2over5_numerator, 3);
        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_2over5_denominator, 30);
    })

    .TestCase("Subtracting zero from a rational returns a rational equivalent to the latter.", [](auto& fixture)
    {
        auto templates_difference_3over6_0over1_numerator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::kNumerator;
        auto templates_difference_3over6_0over1_denominator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_0over1_numerator, 3);
        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_0over1_denominator, 6);

        auto templates_difference_3over6_0over5_numerator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<0, 5>>::kNumerator;
        auto templates_difference_3over6_0over5_denominator = Templates::RationalDifference<Templates::Rational<3, 6>, Templates::Rational<0, 5>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_0over5_numerator, 15);
        SYNTROPY_UNIT_EQUAL(templates_difference_3over6_0over5_denominator, 30);
    })

    .TestCase("Subtracting a rational from zero returns the negated rational.", [](auto& fixture)
    {
        auto templates_difference_0over1_3over1_numerator = Templates::RationalDifference<Templates::Rational<0, 1>, Templates::Rational<3, 1>>::kNumerator;
        auto templates_difference_0over1_3over1_denominator = Templates::RationalDifference<Templates::Rational<0, 1>, Templates::Rational<3, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_difference_0over1_3over1_numerator, -3);
        SYNTROPY_UNIT_EQUAL(templates_difference_0over1_3over1_denominator, 1);

        auto templates_difference_0over2_3over5_numerator = Templates::RationalDifference<Templates::Rational<0, 2>, Templates::Rational<3, 5>>::kNumerator;
        auto templates_difference_0over2_3over5_denominator = Templates::RationalDifference<Templates::Rational<0, 2>, Templates::Rational<3, 5>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_difference_0over2_3over5_numerator, -6);
        SYNTROPY_UNIT_EQUAL(templates_difference_0over2_3over5_denominator, 10);
    })

    .TestCase("Multiplying two rationals returns the unreduced product of both numerators and both denominators.", [](auto& fixture)
    {
        auto templates_product_6over8_9over15_numerator = Templates::RationalProduct<Templates::Rational<6, 8>, Templates::Rational<9, 15>>::kNumerator;
        auto templates_product_6over8_9over15_denominator = Templates::RationalProduct<Templates::Rational<6, 8>, Templates::Rational<9, 15>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_product_6over8_9over15_numerator, 54);
        SYNTROPY_UNIT_EQUAL(templates_product_6over8_9over15_denominator, 120);

        auto templates_product_minus6over2_7over11_numerator = Templates::RationalProduct<Templates::Rational<-6, 2>, Templates::Rational<7, 11>>::kNumerator;
        auto templates_product_minus6over2_7over11_denominator = Templates::RationalProduct<Templates::Rational<-6, 2>, Templates::Rational<7, 11>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_product_minus6over2_7over11_numerator, -42);
        SYNTROPY_UNIT_EQUAL(templates_product_minus6over2_7over11_denominator, 22);
    })

    .TestCase("Multiplying a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        auto templates_product_6over8_1over1_numerator = Templates::RationalProduct<Templates::Rational<6, 8>, Templates::Rational<1, 1>>::kNumerator;
        auto templates_product_6over8_1over1_denominator = Templates::RationalProduct<Templates::Rational<6, 8>, Templates::Rational<1, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_product_6over8_1over1_numerator, 6);
        SYNTROPY_UNIT_EQUAL(templates_product_6over8_1over1_denominator, 8);

        auto templates_product_minus4over4_13over17_numerator = Templates::RationalProduct<Templates::Rational<-4, 4>, Templates::Rational<13, 17>>::kNumerator;
        auto templates_product_minus4over4_13over17_denominator = Templates::RationalProduct<Templates::Rational<-4, 4>, Templates::Rational<13, 17>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_product_minus4over4_13over17_numerator, -52);
        SYNTROPY_UNIT_EQUAL(templates_product_minus4over4_13over17_denominator, 68);
    })

    .TestCase("Dividing a rational by another one returns the unreduced rational equivalent their.", [](auto& fixture)
    {
        auto templates_quotient_6over8_9over15_numerator = Templates::RationalQuotient<Templates::Rational<6, 8>, Templates::Rational<9, 15>>::kNumerator;
        auto templates_quotient_6over8_9over15_denominator = Templates::RationalQuotient<Templates::Rational<6, 8>, Templates::Rational<9, 15>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_quotient_6over8_9over15_numerator, 90);
        SYNTROPY_UNIT_EQUAL(templates_quotient_6over8_9over15_denominator, 72);

        auto templates_quotient_minus6over2_7over11_numerator = Templates::RationalQuotient<Templates::Rational<-6, 2>, Templates::Rational<7, 11>>::kNumerator;
        auto templates_quotient_minus6over2_7over11_denominator = Templates::RationalQuotient<Templates::Rational<-6, 2>, Templates::Rational<7, 11>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_quotient_minus6over2_7over11_numerator, -66);
        SYNTROPY_UNIT_EQUAL(templates_quotient_minus6over2_7over11_denominator, 14);
    })

    .TestCase("Dividing a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        auto templates_quotient_6over8_1over1_numerator = Templates::RationalQuotient<Templates::Rational<6, 8>, Templates::Rational<1, 1>>::kNumerator;
        auto templates_quotient_6over8_1over1_denominator = Templates::RationalQuotient<Templates::Rational<6, 8>, Templates::Rational<1, 1>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_quotient_6over8_1over1_numerator, 6);
        SYNTROPY_UNIT_EQUAL(templates_quotient_6over8_1over1_denominator, 8);
    })
 
    .TestCase("Dividing one by a rational returns the reciprocal of the latter.", [](auto& fixture)
    {
        auto templates_quotient_1over1_6over7_numerator = Templates::RationalQuotient<Templates::Rational<1, 1>, Templates::Rational<6, 7>>::kNumerator;
        auto templates_quotient_1over1_6over7_denominator = Templates::RationalQuotient<Templates::Rational<1, 1>, Templates::Rational<6, 7>>::kDenominator;

        SYNTROPY_UNIT_EQUAL(templates_quotient_1over1_6over7_numerator, 7);
        SYNTROPY_UNIT_EQUAL(templates_quotient_1over1_6over7_denominator, 6);
    });

}