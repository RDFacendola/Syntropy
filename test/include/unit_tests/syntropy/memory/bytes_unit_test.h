/// \file bytes_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/bytes.h"

#include "syntropy/unit_test/unit_test.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* BYTES TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Bytes test fixture.
    struct BytesTestFixture
    {
        Bytes bytes_default_;

        Bytes bytes0_ = Bytes{ 0 };
        Bytes bytes1_ = Bytes{ 1 };
        Bytes bytes2_ = Bytes{ 2 };
        Bytes bytes3_ = Bytes{ 3 };
        Bytes bytes4_ = Bytes{ 4 };
        Bytes bytes6_ = Bytes{ 6 };
        Bytes bytes7_ = Bytes{ 7 };
        Bytes bytes10_ = Bytes{ 10 };
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& bytes_unit_test = MakeAutoUnitTest<BytesTestFixture>("bytes.memory.syntropy")

    .TestCase("Default initialized Bytes are empty.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_default_, fixture.bytes0_);
    })

    .TestCase("Sum of two byte amounts is equal to a byte amount constructed with the sum of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ + fixture.bytes3_, fixture.bytes10_);
    })

    .TestCase("Difference of two byte amounts is equal to a byte amount constructed with the difference of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ - fixture.bytes3_, fixture.bytes4_);
    })

    .TestCase("Product of a byte amount times an integer value (and vice-versa) is equal to a byte amount constructed with the product of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes3_ * 2, fixture.bytes6_);
        SYNTROPY_UNIT_EQUAL(2 * fixture.bytes3_, fixture.bytes6_);
    })

    .TestCase("Quotient of a byte amount divided by an integer value is equal to a byte amount constructed with the quotient of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes6_ / 3, fixture.bytes2_);
    })

    .TestCase("Quotient of two byte amounts is equal to an integer equal to the quotient of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes6_ / fixture.bytes2_, 3);
    })

    .TestCase("Division remainder of a byte amount and an integer value is equal to a byte amount constructed with the division remainder of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ % 3, fixture.bytes1_);
    })

    .TestCase("Division remainder of two byte amounts is equal to an integer equal to the quotient remainder of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ % fixture.bytes4_, 3);
    });

}