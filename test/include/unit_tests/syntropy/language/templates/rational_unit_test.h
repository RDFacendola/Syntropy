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
        using Sum_3o6_2o5 = RationalSum<Rational<3, 6>, Rational<2, 5>>;

        SYNTROPY_UNIT_EQUAL(Sum_3o6_2o5::kNumerator, 9);
        SYNTROPY_UNIT_EQUAL(Sum_3o6_2o5::kDenominator, 10);
    })

    .TestCase("Adding zero to a rational returns a rational is equivalent to the latter.", [](auto& fixture)
    {
        using Sum_3o6_0o1 = RationalSum<Rational<3, 6>, Rational<0, 1>>;

        SYNTROPY_UNIT_EQUAL(Sum_3o6_0o1::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Sum_3o6_0o1::kDenominator, 2);

        using Sum_0o6_3o1 = RationalSum<Rational<0, 6>, Rational<3, 1>>;

        SYNTROPY_UNIT_EQUAL(Sum_0o6_3o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Sum_0o6_3o1::kDenominator, 1);
    })

    .TestCase("Subtracting a rational from another rational returns the unreduced difference of the twos.", [](auto& fixture)
    {
        using Difference_3o6_2o5 = RationalDifference<Rational<3, 6>, Rational<2, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_2o5::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_2o5::kDenominator, 10);
    })

    .TestCase("Subtracting zero from a rational returns a rational equivalent to the latter.", [](auto& fixture)
    {
        using Difference_3o6_0o1 = RationalDifference<Rational<3, 6>, Rational<0, 1>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o1::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o1::kDenominator, 2);

        using Difference_3o6_0o5 = RationalDifference<Rational<3, 6>, Rational<0, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o5::kNumerator, 1);
        SYNTROPY_UNIT_EQUAL(Difference_3o6_0o5::kDenominator, 2);
    })

    .TestCase("Subtracting a rational from zero returns the negated rational.", [](auto& fixture)
    {
        using Difference_0o1_3o1 = RationalDifference<Rational<0, 1>, Rational<3, 1>>;

        SYNTROPY_UNIT_EQUAL(Difference_0o1_3o1::kNumerator, -3);
        SYNTROPY_UNIT_EQUAL(Difference_0o1_3o1::kDenominator, 1);

        using Difference_0o2_3o5 = RationalDifference<Rational<0, 2>, Rational<3, 5>>;

        SYNTROPY_UNIT_EQUAL(Difference_0o2_3o5::kNumerator, -3);
        SYNTROPY_UNIT_EQUAL(Difference_0o2_3o5::kDenominator, 5);
    })

    .TestCase("Multiplying two rationals returns the unreduced product of both numerators and both denominators.", [](auto& fixture)
    {
        using Product_6o8_9o15 = RationalProduct<Rational<6, 8>, Rational<9, 15>>;

        SYNTROPY_UNIT_EQUAL(Product_6o8_9o15::kNumerator, 9);
        SYNTROPY_UNIT_EQUAL(Product_6o8_9o15::kDenominator, 20);

        using Product_Minus6o2_7o11 = RationalProduct<Rational<-6, 2>, Rational<7, 11>>;

        SYNTROPY_UNIT_EQUAL(Product_Minus6o2_7o11::kNumerator, -21);
        SYNTROPY_UNIT_EQUAL(Product_Minus6o2_7o11::kDenominator, 11);
    })

    .TestCase("Multiplying a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using Product_6o8_1o1 = RationalProduct<Rational<6, 8>, Rational<1, 1>>;

        SYNTROPY_UNIT_EQUAL(Product_6o8_1o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Product_6o8_1o1::kDenominator, 4);

        using Product_Minus4o4_13o17 = RationalProduct<Rational<-4, 4>, Rational<13, 17>>;

        SYNTROPY_UNIT_EQUAL(Product_Minus4o4_13o17::kNumerator, -13);
        SYNTROPY_UNIT_EQUAL(Product_Minus4o4_13o17::kDenominator, 17);
    })

    .TestCase("Dividing a rational by another one returns the unreduced rational equivalent their.", [](auto& fixture)
    {
        using Quotient_6o8_9o15 = RationalQuotient<Rational<6, 8>, Rational<9, 15>>;

        SYNTROPY_UNIT_EQUAL(Quotient_6o8_9o15::kNumerator, 5);
        SYNTROPY_UNIT_EQUAL(Quotient_6o8_9o15::kDenominator, 4);

        using Quotient_Minus6o2_7o11 = RationalQuotient<Rational<-6, 2>, Rational<7, 11>>;

        SYNTROPY_UNIT_EQUAL(Quotient_Minus6o2_7o11::kNumerator, -33);
        SYNTROPY_UNIT_EQUAL(Quotient_Minus6o2_7o11::kDenominator, 7);
    })

    .TestCase("Dividing a rational by one returns an unreduced ratio equivalent to the former.", [](auto& fixture)
    {
        using Quotient_6o8_1o1 = RationalQuotient<Rational<6, 8>, Rational<1, 1>>;

        SYNTROPY_UNIT_EQUAL(Quotient_6o8_1o1::kNumerator, 3);
        SYNTROPY_UNIT_EQUAL(Quotient_6o8_1o1::kDenominator, 4);
    })
 
    .TestCase("Dividing one by a rational returns the reciprocal of the latter.", [](auto& fixture)
    {
        using Quotient_1o1_6o7 = RationalQuotient<Rational<1, 1>, Rational<6, 7>>;

        SYNTROPY_UNIT_EQUAL(Quotient_1o1_6o7::kNumerator, 7);
        SYNTROPY_UNIT_EQUAL(Quotient_1o1_6o7::kDenominator, 6);
    });

}