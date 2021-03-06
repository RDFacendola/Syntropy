/// \file bytes_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/memory/foundation/bytes.h"


#include "syntropy/memory/foundation/byte.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Memory::UnitTest
{
    /************************************************************************/
    /* BYTES TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Bytes test fixture.
    struct BytesTestFixture
    {
        /// \brief Memory buffer.
        Byte buffer_[10];

        /// \brief Read-write pointer to a buffer.
        BasePtr<Byte> read_write_ptr_;

        /// \brief Read-only pointer to a buffer.
        BasePtr<const Byte> read_only_ptr_;

        /// \brief Default constructed bytes object.
        const Bytes bytes_default_ = Bytes{};

        /// \brief Numeric constants.

        const Bytes bytes0_ = Bytes{ 0 };
        const Bytes bytes1_ = Bytes{ 1 };
        const Bytes bytes2_ = Bytes{ 2 };
        const Bytes bytes3_ = Bytes{ 3 };
        const Bytes bytes4_ = Bytes{ 4 };
        const Bytes bytes5_ = Bytes{ 5 };
        const Bytes bytes6_ = Bytes{ 6 };
        const Bytes bytes7_ = Bytes{ 7 };
        const Bytes bytes8_ = Bytes{ 8 };
        const Bytes bytes10_ = Bytes{ 10 };

        /// \brief Modifiable bytes object.
        Bytes bytes_lhs_ = Bytes{ 5 };

        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& bytes_unit_test = MakeAutoUnitTest<BytesTestFixture>("bytes.memory.syntropy")

    .TestCase("Bytes are convertible to and from integers.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(ToInt(fixture.bytes2_), 2);
        SYNTROPY_UNIT_EQUAL(ToSize(2), fixture.bytes2_);
    })

    .TestCase("SizeOf return a bytes amount equal to the size of a type, converted to a bytes amount.", [](auto& fixture)
    {
        using namespace Literals;

        SYNTROPY_UNIT_EQUAL(SizeOf<Fix32>(), 4_Bytes);
        SYNTROPY_UNIT_EQUAL(SizeOf(Fix64{}), 8_Bytes);
    })

    .TestCase("Default initialized Bytes are empty.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_default_, fixture.bytes0_);
    })

    .TestCase("Sum of two byte amounts is equal to a byte amount constructed with the sum of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ + fixture.bytes3_, fixture.bytes10_);
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_ += fixture.bytes3_, fixture.bytes8_);
    })

    .TestCase("Difference of two byte amounts is equal to a byte amount constructed with the difference of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ - fixture.bytes3_, fixture.bytes4_);
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_ -= fixture.bytes3_, fixture.bytes2_);
    })

    .TestCase("Product of a byte amount times an integer value (and vice-versa) is equal to a byte amount constructed with the product of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes3_ * 2, fixture.bytes6_);
        SYNTROPY_UNIT_EQUAL(2 * fixture.bytes3_, fixture.bytes6_);
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_ *= 2, fixture.bytes10_);
    })

    .TestCase("Quotient of a byte amount divided by an integer value is equal to a byte amount constructed with the quotient of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes6_ / 3, fixture.bytes2_);
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_ /= 2, fixture.bytes2_);
    })

    .TestCase("Quotient of two byte amounts is equal to an integer equal to the quotient of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes6_ / fixture.bytes2_, 3);
    })

    .TestCase("Division remainder of a byte amount and an integer value is equal to a byte amount constructed with the division remainder of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_ % 3, fixture.bytes1_);
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_ %= 2, fixture.bytes1_);
    })

    .TestCase("Division remainder of two byte amounts is equal to an integer equal to the quotient remainder of the two integer values.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_% fixture.bytes4_, Bytes{ 3 });
    })

    .TestCase("Pre-incrementing a byte amount produces a byte amount which is equal to the integer value pre-incremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(++fixture.bytes_lhs_, Bytes{ 6 });
    })

    .TestCase("Pre-decrementing a byte amount produces a byte amount which is equal to the integer value pre-decremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(--fixture.bytes_lhs_, Bytes{ 4 });
    })

    .TestCase("Post-incrementing a byte amount produces a byte amount which is equal to the integer value post-incremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_++, Bytes{ 5 });
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_, Bytes{ 6 });
    })

    .TestCase("Post-decrementing a byte amount produces a byte amount which is equal to the integer value post-decremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_--, Bytes{ 5 });
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_, Bytes{ 4 });
    })

    .TestCase("Bytes literals behaves as binary metric prefixes equivalent.", [](auto& fixture)
    {
        using namespace Literals;

        SYNTROPY_UNIT_EQUAL(1_Bytes,   Bytes{ 0x0000000000000001ll });
        SYNTROPY_UNIT_EQUAL(2_KiBytes, Bytes{ 0x0000000000000800ll });
        SYNTROPY_UNIT_EQUAL(3_MiBytes, Bytes{ 0x0000000000300000ll });
        SYNTROPY_UNIT_EQUAL(4_GiBytes, Bytes{ 0x0000000100000000ll });
        SYNTROPY_UNIT_EQUAL(5_TiBytes, Bytes{ 0x0000050000000000ll });
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline void BytesTestFixture::Before()
    {
        bytes_lhs_ = Bytes{ 5 };

        read_write_ptr_ = &buffer_[4];
        read_only_ptr_ = &buffer_[4];
    }

}