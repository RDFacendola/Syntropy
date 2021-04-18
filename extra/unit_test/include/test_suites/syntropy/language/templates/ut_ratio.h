
/// \file ut_math.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/ratio.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* RATIO TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Ratio test fixture.
    struct RatioTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_ratio
        = Syntropy::UnitTest::MakeAutoUnitTest<RatioTestFixture>(
            u8"ratio.templates.language.syntropy")

    .TestCase(u8"A ratio-type is a ratio.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsRatio<Ratio<1, 2>>), true);
    })

    .TestCase(u8"A non-ratio-type is not a ratio.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsRatio<Int>), false);
    })

    .TestCase(u8"The common ratio of two ratios is a third ratio to which "
              u8"both can be converted to.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((CommonRatio<Ratio<3, 14>, Ratio<6, 21>>),
                           (Ratio<3, 42>));
    })

    .TestCase(u8"Adding two ratios returns the reduced sum of the twos.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioAdd<Ratio<3, 6>, Ratio<2, 5>>),
                           (Ratio<9, 10>));
    })

    .TestCase(u8"The sum of a ratio and 0 is equal to the reduced ratio "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioAdd<Ratio<3, 6>, Ratio<0, 1>>),
                           (Ratio<1, 2>));
    })

    .TestCase(u8"Ratio addition is commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioAdd<Ratio<3, 6>, Ratio<2, 5>>),
                           (RatioAdd<Ratio<2, 5>, Ratio<3, 6>>));
    })

    .TestCase(u8"Subtracting two ratios returns the reduced difference of the "
              u8"twos.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioSubtract<Ratio<3, 6>, Ratio<2, 5>>),
                           (Ratio<1, 10>));
    })

    .TestCase(u8"The difference between a ratio and 0 is equal to the reduced "
              u8"ratio itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioSubtract<Ratio<3, 6>, Ratio<0, 1>>),
                           (Ratio<1, 2>));
    })

    .TestCase(u8"Ratio difference is anti-commutative.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioSubtract<Ratio<3, 6>, Ratio<2, 5>>),
                           (Ratio<1, 10>));

        SYNTROPY_UNIT_SAME((RatioSubtract<Ratio<2, 5>, Ratio<3, 6>>),
                           (Ratio<-1, 10>));
    })

    .TestCase(u8"Multiplying two ratios returns the reduced product of the "
              u8" twos.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioMultiply<Ratio<6, 8>, Ratio<9, 15>>),
                           (Ratio<9, 20>));
    })

    .TestCase(u8"The product between a ratio and 1 is the reduced ratio "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioMultiply<Ratio<3, 6>, Ratio<1, 1>>),
                           (Ratio<1, 2>));
    })

    .TestCase(u8"Dividing two ratios returns the reduced division of the "
              u8" twos.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioDivide<Ratio<6, 8>, Ratio<9, 15>>),
                           (Ratio<5, 4>));
    })

    .TestCase(u8"Dividing a ratio by 1 returns the reduced ratio itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioDivide<Ratio<3, 6>, Ratio<1, 1>>),
                           (Ratio<1, 2>));
    })

    .TestCase(u8"Dividing 1 by a ratio returns the reciprocal of the reduced "
              u8"ratio.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RatioDivide<Ratio<1, 1>, Ratio<3, 6>>),
                           (Ratio<2, 1>));
    })

    .TestCase(u8"Ratios compare equal to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioEqual<Ratio<3, 6>, Ratio<3, 6>>), true);
    })

    .TestCase(u8"Ratios compare equal to their reduced form.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioEqual<Ratio<3, 6>, Ratio<1, 2>>), true);
    })

    .TestCase(u8"Ratios compare no different than themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioNotEqual<Ratio<3, 6>, Ratio<3, 6>>), false);
    })

    .TestCase(u8"Ratios compare different to different ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioNotEqual<Ratio<3, 6>, Ratio<4, 5>>), true);
    })

    .TestCase(u8"Ratios compare less-than larger ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLess<Ratio<3, 6>, Ratio<4, 5>>), true);
    })

    .TestCase(u8"Ratios compare not less-than smaller ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLess<Ratio<4, 5>, Ratio<3, 6>>), false);
    })

    .TestCase(u8"Ratios compare not less-than themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLess<Ratio<3, 6>, Ratio<1, 2>>), false);
    })

    .TestCase(u8"Ratios compare greater-than smaller ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreater<Ratio<4, 5>, Ratio<3, 6>>), true);
    })

    .TestCase(u8"Ratios compare not greater-than larger ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreater<Ratio<3, 6>, Ratio<4, 5>>), false);
    })

    .TestCase(u8"Ratios compare not greater-than themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreater<Ratio<3, 6>, Ratio<1, 2>>), false);
    })

    .TestCase(u8"Ratios compare less-equal-than larger ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLessEqual<Ratio<3, 6>, Ratio<4, 5>>), true);
    })

    .TestCase(u8"Ratios compare not less-equal-than smaller ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLessEqual<Ratio<4, 5>, Ratio<3, 6>>), false);
    })

    .TestCase(u8"Ratios compare less-equal-than themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioLessEqual<Ratio<3, 6>, Ratio<1, 2>>), true);
    })

    .TestCase(u8"Ratios compare greater-equal-than larger ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreaterEqual<Ratio<4, 5>, Ratio<3, 6>>),
                            true);
    })

    .TestCase(u8"Ratios compare not greater-equal-than smaller ratios.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreaterEqual<Ratio<3, 6>, Ratio<4, 5>>),
                            false);
    })

    .TestCase(u8"Ratios compare greater-equal-than themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((RatioGreaterEqual<Ratio<3, 6>, Ratio<1, 2>>),
                            true);
    });

}

// ===========================================================================
