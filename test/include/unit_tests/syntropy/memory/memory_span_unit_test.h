/// \file memory_span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"

#include "syntropy/memory/memory_span.h"

#include "syntropy/unit_test/unit_test.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* MEMORY SPAN TEST FIXTURE                                             */
    /************************************************************************/

    /// \brief Span test fixture.
    struct MemorySpanTestFixture
    {
        /// \brief Buffer aligned to 16 Bytes.
        alignas(16) Byte aligned_buffer16_[10];

        /// \brief Buffer of random bytes.
        Byte buffer_[10];

        /// \brief Equivalent buffer of random bytes.
        Byte buffer_equivalent_[10];

        /// \brief Different buffer of random bytes.
        Byte buffer_different_[10];

        union
        {
            /// \brief Raw buffer.
            Byte raw_[32];

            /// \brief Typed elements span.
            Fix64 elements_[4];
        } union_;

        /// \brief Setup the fixture before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& memory_span_unit_test = MakeAutoUnitTest<MemorySpanTestFixture>("memory_span.memory.syntropy")

    .TestCase("Default constructed memory spans are null.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{};
        auto cmemory_span = ReadOnlyMemorySpan{};

        SYNTROPY_UNIT_EQUAL(!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 0 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), nullptr);

        SYNTROPY_UNIT_EQUAL(!cmemory_span, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetSize(), Bytes{ 0 });
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetData(), nullptr);
    })

    .TestCase("Memory spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(!!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), &fixture.buffer_[0]);

        SYNTROPY_UNIT_EQUAL(!!cmemory_span, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(cmemory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], &fixture.buffer_[0] + 10 };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], &fixture.buffer_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), &fixture.buffer_[0]);

        SYNTROPY_UNIT_EQUAL(!!cmemory_span, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(cmemory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(cmemory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans are always equivalent to themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(memory_span == memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span, false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(memory_span, memory_span), true);

        SYNTROPY_UNIT_EQUAL(cmemory_span == cmemory_span, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span != cmemory_span, false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(cmemory_span, cmemory_span), true);

        SYNTROPY_UNIT_EQUAL(cmemory_span == memory_span, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span != memory_span, false);
        SYNTROPY_UNIT_EQUAL(memory_span == cmemory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span != cmemory_span, false);
    })

    .TestCase("Memory spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_span_short = MemorySpan{ &fixture.buffer_[0], Bytes{ 9 } };
        auto memory_span_equivalent = MemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto memory_span_different = MemorySpan{ &fixture.buffer_different_[3], Bytes{ 7 } };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span_short = MemorySpan{ &fixture.buffer_[0], Bytes{ 9 } };
        auto cmemory_span_equivalent = ReadOnlyMemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto cmemory_span_different = ReadOnlyMemorySpan{ &fixture.buffer_different_[3], Bytes{ 7 } };

        SYNTROPY_UNIT_EQUAL(memory_span == memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(memory_span == memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(memory_span == memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span_different, true);

        SYNTROPY_UNIT_EQUAL(cmemory_span == cmemory_span_short, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span != cmemory_span_short, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span == cmemory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span != cmemory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span == cmemory_span_different, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span != cmemory_span_different, true);

        SYNTROPY_UNIT_EQUAL(memory_span == cmemory_span_short, false);
        SYNTROPY_UNIT_EQUAL(memory_span != cmemory_span_short, true);
        SYNTROPY_UNIT_EQUAL(memory_span == cmemory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(memory_span != cmemory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(memory_span == cmemory_span_different, false);
        SYNTROPY_UNIT_EQUAL(memory_span != cmemory_span_different, true);

        SYNTROPY_UNIT_EQUAL(cmemory_span == memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span != memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span == memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(cmemory_span != memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span == memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(cmemory_span != memory_span_different, true);
    })

    .TestCase("Memory spans are not identical to other spans, even if they compare equivalent.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_span_equivalent = MemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto memory_span_identical = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span_equivalent = ReadOnlyMemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto cmemory_span_identical = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, memory_span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, memory_span_identical), true);

        SYNTROPY_UNIT_EQUAL(AreIdentical(cmemory_span, cmemory_span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(cmemory_span, cmemory_span_identical), true);

        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, cmemory_span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, cmemory_span_identical), true);

        SYNTROPY_UNIT_EQUAL(AreIdentical(cmemory_span, memory_span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(cmemory_span, memory_span_identical), true);
    })

    .TestCase("Memory spans front elements are readable.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(Front(memory_span), Byte{ 0 });
        SYNTROPY_UNIT_EQUAL(Front(cmemory_span), Byte{ 0 });
    })

    .TestCase("Memory spans front elements are writable.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        Front(memory_span) = Byte{ 42 };

        SYNTROPY_UNIT_EQUAL(Front(memory_span), Byte{ 42 });
    })

    .TestCase("Removing front elements from a memory span yields another memory span which is equal to the remaining elements.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.buffer_[0], 10_Bytes };
        auto popfront1 = MemorySpan{ &fixture.buffer_[1], 9_Bytes };
        auto popfront3 = MemorySpan{ &fixture.buffer_[3], 7_Bytes };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], 10_Bytes };
        auto cpopfront1 = ReadOnlyMemorySpan{ &fixture.buffer_[1], 9_Bytes };
        auto cpopfront3 = ReadOnlyMemorySpan{ &fixture.buffer_[3], 7_Bytes };

        SYNTROPY_UNIT_EQUAL(PopFront(memory_span), popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(memory_span, 3_Bytes), popfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(cmemory_span), cpopfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(cmemory_span, 3_Bytes), cpopfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(memory_span), cpopfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(memory_span, 3_Bytes), cpopfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(cmemory_span), popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(cmemory_span, 3_Bytes), popfront3);
    })

    .TestCase("Selecting the first elements of a memory span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.buffer_[0],  10_Bytes };
        auto first4 = MemorySpan{ &fixture.buffer_[0], 4_Bytes };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0],  10_Bytes };
        auto cfirst4 = ReadOnlyMemorySpan{ &fixture.buffer_[0], 4_Bytes };

        SYNTROPY_UNIT_EQUAL(First(memory_span, 4_Bytes), first4);
        SYNTROPY_UNIT_EQUAL(First(cmemory_span, 4_Bytes), cfirst4);
        SYNTROPY_UNIT_EQUAL(First(memory_span, 4_Bytes), cfirst4);
        SYNTROPY_UNIT_EQUAL(First(cmemory_span, 4_Bytes), first4);
    })

    .TestCase("Memory spans contain themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, memory_span), true);

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, memory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, memory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(memory_span, cmemory_span), true);
    })

    .TestCase("Memory spans contains another span if the latter refers to a memory location inside the first.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_subspan = MemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_subspan = ReadOnlyMemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, memory_subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, memory_subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(memory_span, cmemory_span), true);
    })

    .TestCase("Empty memory spans are contained in any other span.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, MemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, ReadOnlyMemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(memory_span, ReadOnlyMemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(cmemory_span, MemorySpan{}), true);
    })

    .TestCase("Empty memory spans are not contained in themselves.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Contains(MemorySpan{}, MemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(ReadOnlyMemorySpan{}, ReadOnlyMemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(MemorySpan{}, ReadOnlyMemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(ReadOnlyMemorySpan{}, MemorySpan{}), false);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, memory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, memory_span), true);
    })

    .TestCase("Disjoint spans do not overlap.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto disjoint = MemorySpan{ &fixture.buffer_[6], Bytes{ 4 } };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto cdisjoint = ReadOnlyMemorySpan{ &fixture.buffer_[6], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, disjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, cdisjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, cdisjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, disjoint), false);
    })

    .TestCase("Contiguous spans do not overlap.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto contiguous = MemorySpan{ &fixture.buffer_[4], Bytes{ 3 } };

        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto ccontiguous = ReadOnlyMemorySpan{ &fixture.buffer_[4], Bytes{ 3 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, contiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, ccontiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, ccontiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cmemory_span, contiguous), false);
    })

    .TestCase("Empty spans do not overlap with any other span.", [](auto& fixture)
    {
        auto span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto empty = MemorySpan{};

        auto cspan = ReadOnlyMemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto cempty = ReadOnlyMemorySpan{};

        SYNTROPY_UNIT_EQUAL(Overlaps(empty, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(span, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(empty, span), false);

        SYNTROPY_UNIT_EQUAL(Overlaps(cempty, cempty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, cempty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cempty, cspan), false);
    })

    .TestCase("Overlapping test is commutative.", [](auto& fixture)
    {
        auto left = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto right = MemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        auto cleft = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto cright = MemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(left, right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(right, left), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(cleft, cright), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cright, cleft), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(left, cright), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(right, cleft), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(cleft, right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cright, left), true);
    })

    .TestCase("Memory spans are aligned to the same alignment requirement of the memory region they refer to.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.aligned_buffer16_[0], Bytes{ 4 } };
        auto cmemory_span = ReadOnlyMemorySpan{ &fixture.aligned_buffer16_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(memory_span, 2_Alignment), true);

        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(cmemory_span, 2_Alignment), true);
    })

    .TestCase("Aligning a memory span to a value less than the original alignment returns the same span.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.aligned_buffer16_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Align(memory_span, 16_Alignment), memory_span);
    })

    .TestCase("Aligning an unaligned memory span reduces the span size by the difference between the original alignment and the requested one.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.aligned_buffer16_[1], Bytes{ 9 } };
        auto memory_span_aligned = MemorySpan{ &fixture.aligned_buffer16_[8], Bytes{ 2 } };

        SYNTROPY_UNIT_EQUAL(Align(memory_span, 8_Alignment), memory_span_aligned);
    })

    .TestCase("Over-aligning a memory span returns an empty span.", [](auto& fixture)
    {
        using namespace literals;

        auto memory_span = MemorySpan{ &fixture.aligned_buffer16_[8], Bytes{ 2 } };

        SYNTROPY_UNIT_EQUAL(Align(memory_span, 16_Alignment), MemorySpan{});
    })

    .TestCase("Memory spans can be converted to strongly-typed spans", [](auto& fixture)
    {
        using namespace literals;

        auto span = Span<Fix64>{ fixture.union_.elements_, 4 };
        auto memory_span = MemorySpan{ fixture.union_.raw_, 32_Bytes };

        SYNTROPY_UNIT_EQUAL(ToSpan<Fix64>(memory_span), span);
    })

    .TestCase("Strongly-typed spans can be converted to memory spans and read-only memory spans.", [](auto& fixture)
    {
        using namespace literals;

        auto span = Span<Fix64>{ fixture.union_.elements_, 4 };
        auto memory_span = MemorySpan{ fixture.union_.raw_, 32_Bytes };
        auto read_only_memory_span = ReadOnlyMemorySpan{ fixture.union_.raw_, 32_Bytes };

        SYNTROPY_UNIT_EQUAL(ToMemorySpan(span), memory_span);
        SYNTROPY_UNIT_EQUAL(ToReadOnlyMemorySpan(span), read_only_memory_span);
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemorySpanTestFixture.

    inline void MemorySpanTestFixture::Before()
    {
        for (auto index = 0; index < 10; ++index)
        {
            buffer_[index] = Byte{ index };
            buffer_equivalent_[index] = buffer_[index];
            buffer_different_[index] = Byte{ index * 2 };
        }

        for (auto index = 0; index < 4; ++index)
        {
            union_.elements_[index] = Fix64{ index * index };
        }
    }

}