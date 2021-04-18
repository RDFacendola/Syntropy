
/// \file ut_math.h
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
    /* RATIO TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Templates test fixture.
    struct RatioTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_ratio
        = Syntropy::UnitTest::MakeAutoUnitTest<RatioTestFixture>(
            u8"ratio.templates.language.syntropy")

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
    });
}

// ===========================================================================
