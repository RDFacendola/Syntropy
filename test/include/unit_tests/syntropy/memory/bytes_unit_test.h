/// \file bytes_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/memory/bytes.h"


#include "syntropy/memory/byte.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::unit_test
{
    /************************************************************************/
    /* BYTES TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Bytes test fixture.
    struct BytesTestFixture
    {
        /// \brief Memory buffer.
        Memory::Byte buffer_[10];

        /// \brief Read-write pointer to a buffer.
        Pointer<Memory::Byte> read_write_ptr_;

        /// \brief Read-only pointer to a buffer.
        Pointer<const Memory::Byte> read_only_ptr_;

        /// \brief Default constructed bytes object.
        const Memory::Bytes bytes_default_ = Memory::Bytes{};

        /// \brief Numeric constants.

        const Memory::Bytes bytes0_ = Memory::Bytes{ 0 };
        const Memory::Bytes bytes1_ = Memory::Bytes{ 1 };
        const Memory::Bytes bytes2_ = Memory::Bytes{ 2 };
        const Memory::Bytes bytes3_ = Memory::Bytes{ 3 };
        const Memory::Bytes bytes4_ = Memory::Bytes{ 4 };
        const Memory::Bytes bytes5_ = Memory::Bytes{ 5 };
        const Memory::Bytes bytes6_ = Memory::Bytes{ 6 };
        const Memory::Bytes bytes7_ = Memory::Bytes{ 7 };
        const Memory::Bytes bytes8_ = Memory::Bytes{ 8 };
        const Memory::Bytes bytes10_ = Memory::Bytes{ 10 };

        /// \brief Modifiable bytes object.
        Memory::Bytes bytes_lhs_ = Memory::Bytes{ 5 };

        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& bytes_unit_test = MakeAutoUnitTest<BytesTestFixture>("bytes.memory.syntropy")

    .TestCase("Memory::Bytes are convertible to and from integers.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Memory::ToInt(fixture.bytes2_), 2);
        SYNTROPY_UNIT_EQUAL(Memory::ToBytes(2), fixture.bytes2_);
    })

    .TestCase("SizeOf return a bytes amount equal to the size of a type, converted to a bytes amount.", [](auto& fixture)
    {
        using namespace Memory::Literals;

        SYNTROPY_UNIT_EQUAL(Memory::SizeOf<Fix32>(), 4_Bytes);
        SYNTROPY_UNIT_EQUAL(Memory::SizeOf(Fix64{}), 8_Bytes);
    })

    .TestCase("Default initialized Memory::Bytes are empty.", [](auto& fixture)
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
        SYNTROPY_UNIT_EQUAL(fixture.bytes7_% fixture.bytes4_, Memory::Bytes{ 3 });
    })

    .TestCase("Pre-incrementing a byte amount produces a byte amount which is equal to the integer value pre-incremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(++fixture.bytes_lhs_, Memory::Bytes{ 6 });
    })

    .TestCase("Pre-decrementing a byte amount produces a byte amount which is equal to the integer value pre-decremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(--fixture.bytes_lhs_, Memory::Bytes{ 4 });
    })

    .TestCase("Post-incrementing a byte amount produces a byte amount which is equal to the integer value post-incremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_++, Memory::Bytes{ 5 });
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_, Memory::Bytes{ 6 });
    })

    .TestCase("Post-decrementing a byte amount produces a byte amount which is equal to the integer value post-decremented by one.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_--, Memory::Bytes{ 5 });
        SYNTROPY_UNIT_EQUAL(fixture.bytes_lhs_, Memory::Bytes{ 4 });
    })

    .TestCase("Memory::Bytes literals behaves as binary metric prefixes equivalent.", [](auto& fixture)
    {
        using namespace Memory::Literals;

        SYNTROPY_UNIT_EQUAL(1_Bytes,   Memory::Bytes{ 0x0000000000000001ll });
        SYNTROPY_UNIT_EQUAL(2_KiBytes, Memory::Bytes{ 0x0000000000000800ll });
        SYNTROPY_UNIT_EQUAL(3_MiBytes, Memory::Bytes{ 0x0000000000300000ll });
        SYNTROPY_UNIT_EQUAL(4_GiBytes, Memory::Bytes{ 0x0000000100000000ll });
        SYNTROPY_UNIT_EQUAL(5_TiBytes, Memory::Bytes{ 0x0000050000000000ll });
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline void BytesTestFixture::Before()
    {
        bytes_lhs_ = Memory::Bytes{ 5 };

        read_write_ptr_ = &buffer_[4];
        read_only_ptr_ = &buffer_[4];
    }

}