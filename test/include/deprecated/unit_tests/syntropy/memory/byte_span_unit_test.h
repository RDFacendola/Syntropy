/// \file byte_span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/serialization/serialization_types.h"
#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/containers/fix_array.h"
#include "syntropy/memory/foundation/byte.h"
#include "syntropy/memory/foundation/byte_span.h"
#include "syntropy/memory/foundation/bytes.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/unit_test/unit_test.h"

#include <iostream>

namespace Syntropy::Memory::UnitTest
{
    /************************************************************************/
    /* BYTE SPAN TEST FIXTURE                                               */
    /************************************************************************/

    /// \brief Byte span test fixture.
    struct ByteSpanTestFixture
    {
        /// \brief Union used to test data conversion.
        union
        {
            /// \brief Raw buffer.
            FixArray<Byte, 32> raw_;

            /// \brief Typed elements span.
            FixArray<Fix64, 4> elements_;

        } union_;

        /// \brief Setup the fixture before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& byte_span_unit_test = MakeAutoUnitTest<ByteSpanTestFixture>("byte_span.memory.syntropy")

    .TestCase("Span have a size which is exactly equal to the memory footprint of a single element times the number of elements in the span.", [](auto& fixture)
    {
        using namespace Literals;

        auto span = Span<Fix64>{ &fixture.union_.elements_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Memory::Size(span), Bytes{ 80 });
    })

    .TestCase("Byte pointers are aligned to the same alignment requirement of the memory region they refer to.", [](auto& fixture)
    {
        using namespace Literals;

        auto byte_ptr = reinterpret_cast<BytePtr>(16);

        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_ptr, 2_Alignment), true);
    })

    .TestCase("Byte spans are aligned to the same alignment requirement of the memory region they refer to.", [](auto& fixture)
    {
        using namespace Literals;

        auto byte_ptr = reinterpret_cast<BytePtr>(16);

        auto byte_span = ByteSpan{ byte_ptr, 4 };

        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(Memory::IsAlignedTo(byte_span, 2_Alignment), true);
    })

    .TestCase("Aligning a byte span to a value less than the original alignment returns the same span.", [](auto& fixture)
    {
        using namespace Literals;

        auto byte_ptr = reinterpret_cast<BytePtr>(32);

        auto byte_span = ByteSpan{ byte_ptr, 4 };

        SYNTROPY_UNIT_EQUAL(Memory::Align(byte_span, 16_Alignment), byte_span);
    })

    .TestCase("Aligning an unaligned byte span reduces the span size by the difference between the original alignment and the requested one.", [](auto& fixture)
    {
        using namespace Literals;

        auto byte_ptr = reinterpret_cast<BytePtr>(10);
        auto byte_ptr_aligned = reinterpret_cast<BytePtr>(16);

        auto byte_span = ByteSpan{ byte_ptr, 8 };
        auto byte_span_aligned = ByteSpan{ byte_ptr_aligned, 2 };

        SYNTROPY_UNIT_EQUAL(Memory::Align(byte_span, 8_Alignment), byte_span_aligned);
    })

    .TestCase("Over-aligning a byte span returns an empty span.", [](auto& fixture)
    {
        using namespace Literals;

        auto byte_ptr = reinterpret_cast<BytePtr>(10);

        auto byte_span = ByteSpan{ byte_ptr, 2 };

        SYNTROPY_UNIT_EQUAL(Memory::Align(byte_span, 16_Alignment), ByteSpan{});
    })

    .TestCase("Byte spans can be converted to typed spans.", [](auto& fixture)
    {
        auto span = fixture.union_.elements_.GetData();
        auto byte_span = fixture.union_.raw_.GetData();

        SYNTROPY_UNIT_EQUAL(ToSpan<Fix64>(byte_span), span);
    })

    .TestCase("Typed spans can be converted to byte spans.", [](auto& fixture)
    {
        auto span = fixture.union_.elements_.GetData();
        auto byte_span = fixture.union_.raw_.GetData();

        SYNTROPY_UNIT_EQUAL(ToByteSpan(span), byte_span);
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemorySpanTestFixture.

    inline void ByteSpanTestFixture::Before()
    {
        for (auto index = 0; index < 4; ++index)
        {
            union_.elements_[index] = Fix64{ index * index };
        }
    }

}