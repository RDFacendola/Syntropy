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
        auto rw_memory_span = RWMemorySpan{};
        auto rd_memory_span = MemorySpan{};

        SYNTROPY_UNIT_EQUAL(!rw_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rw_memory_span), Bytes{ 0 });
        SYNTROPY_UNIT_EQUAL(rw_memory_span.GetData(), nullptr);

        SYNTROPY_UNIT_EQUAL(!rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rd_memory_span), Bytes{ 0 });
        SYNTROPY_UNIT_EQUAL(rd_memory_span.GetData(), nullptr);
    })

    .TestCase("Memory spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };

        SYNTROPY_UNIT_EQUAL(!!rw_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rw_memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(rw_memory_span.GetData(), &fixture.buffer_[0]);

        SYNTROPY_UNIT_EQUAL(!!rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rd_memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(rd_memory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], &fixture.buffer_[0] + 10 };
        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], &fixture.buffer_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rd_memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(rd_memory_span.GetData(), &fixture.buffer_[0]);

        SYNTROPY_UNIT_EQUAL(!!rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(Size(rd_memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(rd_memory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans are always equivalent to themselves.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };

        SYNTROPY_UNIT_EQUAL(rw_memory_span == rw_memory_span, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rw_memory_span, false);

        SYNTROPY_UNIT_EQUAL(Equals(rw_memory_span, rw_memory_span), true);

        SYNTROPY_UNIT_EQUAL(rd_memory_span == rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rd_memory_span, false);

        SYNTROPY_UNIT_EQUAL(Equals(rw_memory_span, rw_memory_span), true);

        SYNTROPY_UNIT_EQUAL(rw_memory_span == rd_memory_span, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rd_memory_span, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span == rw_memory_span, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rw_memory_span, false);
    })

    .TestCase("Memory spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };
        auto rw_memory_span_short = RWMemorySpan{ &fixture.buffer_[0], 9 };
        auto rw_memory_span_equivalent = RWMemorySpan{ &fixture.buffer_equivalent_[0], 10 };
        auto rw_memory_span_different = RWMemorySpan{ &fixture.buffer_different_[3], 7 };

        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_span_short = MemorySpan{ &fixture.buffer_[0], 9 };
        auto rd_memory_span_equivalent = MemorySpan{ &fixture.buffer_equivalent_[0], 10 };
        auto rd_memory_span_different = MemorySpan{ &fixture.buffer_different_[3], 7 };

        SYNTROPY_UNIT_EQUAL(rw_memory_span == rw_memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rw_memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span == rw_memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rw_memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span == rw_memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rw_memory_span_different, true);

        SYNTROPY_UNIT_EQUAL(rd_memory_span == rd_memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rd_memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span == rd_memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rd_memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span == rd_memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rd_memory_span_different, true);

        SYNTROPY_UNIT_EQUAL(rw_memory_span == rd_memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rd_memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span == rd_memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rd_memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span == rd_memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(rw_memory_span != rd_memory_span_different, true);

        SYNTROPY_UNIT_EQUAL(rd_memory_span == rw_memory_span_short, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rw_memory_span_short, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span == rw_memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rw_memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span == rw_memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(rd_memory_span != rw_memory_span_different, true);
    })

    .TestCase("Memory spans front elements are readable.", [](auto& fixture)
    {
        auto rw_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Front(rw_memory_span), Byte{ 0 });
        SYNTROPY_UNIT_EQUAL(Front(rd_memory_span), Byte{ 0 });
    })

    .TestCase("Memory spans front elements are writable.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };

        Front(rw_memory_span) = Byte{ 42 };

        SYNTROPY_UNIT_EQUAL(Front(rw_memory_span), Byte{ 42 });
    })

    .TestCase("Removing front elements from a memory span yields another memory span which is equal to the remaining elements.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10};
        auto rw_popfront1 = RWMemorySpan{ &fixture.buffer_[1], 9 };
        auto rw_popfront3 = RWMemorySpan{ &fixture.buffer_[3], 7 };

        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_popfront1 = MemorySpan{ &fixture.buffer_[1], 9 };
        auto rd_popfront3 = MemorySpan{ &fixture.buffer_[3], 7 };

        SYNTROPY_UNIT_EQUAL(PopFront(rw_memory_span), rw_popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(rw_memory_span, 3), rw_popfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(rd_memory_span), rd_popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(rd_memory_span, 3), rd_popfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(rw_memory_span), rd_popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(rw_memory_span, 3), rd_popfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(rd_memory_span), rw_popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(rd_memory_span, 3), rw_popfront3);
    })

    .TestCase("Selecting the first elements of a memory span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0],  10};
        auto rw_first4 = RWMemorySpan{ &fixture.buffer_[0], 4 };

        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0],  10};
        auto rd_first4 = MemorySpan{ &fixture.buffer_[0], 4 };

        SYNTROPY_UNIT_EQUAL(First(rw_memory_span, 4), rw_first4);
        SYNTROPY_UNIT_EQUAL(First(rd_memory_span, 4), rd_first4);
        SYNTROPY_UNIT_EQUAL(First(rw_memory_span, 4), rd_first4);
        SYNTROPY_UNIT_EQUAL(First(rd_memory_span, 4), rw_first4);
    })

    .TestCase("Memory spans contain themselves.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, rw_memory_span), true);

        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, rw_memory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, rd_memory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, rw_memory_span), true);
        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, rd_memory_span), true);
    })

    .TestCase("Memory spans contains another span if the latter refers to a memory location inside the first.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };
        auto rw_memory_subspan = RWMemorySpan{ &fixture.buffer_[2], 4 };

        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_subspan = MemorySpan{ &fixture.buffer_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, rw_memory_subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, rd_memory_subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, rw_memory_subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, rd_memory_subspan), true);
    })

    .TestCase("Empty memory spans are contained in any other span.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 10 };
        auto rd_memory_span = MemorySpan{ &fixture.buffer_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, RWMemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, MemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(rw_memory_span, MemorySpan{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(rd_memory_span, RWMemorySpan{}), true);
    })

    .TestCase("Empty memory spans are not contained in themselves.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Contains(RWMemorySpan{}, RWMemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(MemorySpan{}, MemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(RWMemorySpan{}, MemorySpan{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(MemorySpan{}, RWMemorySpan{}), false);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 4 };
        auto rd_cmemory_span = MemorySpan{ &fixture.buffer_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rw_memory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rd_cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rd_cmemory_span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rw_memory_span), true);
    })

    .TestCase("Disjoint spans do not overlap.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 4 };
        auto rw_disjoint = RWMemorySpan{ &fixture.buffer_[6], 4 };

        auto rd_cmemory_span = MemorySpan{ &fixture.buffer_[0], 4 };
        auto rd_cdisjoint = MemorySpan{ &fixture.buffer_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rw_disjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rd_cdisjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rd_cdisjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rw_disjoint), false);
    })

    .TestCase("Contiguous spans do not overlap.", [](auto& fixture)
    {
        auto rw_memory_span = RWMemorySpan{ &fixture.buffer_[0], 4 };
        auto rw_contiguous = RWMemorySpan{ &fixture.buffer_[4], 3 };

        auto rd_cmemory_span = MemorySpan{ &fixture.buffer_[0], 4 };
        auto rd_ccontiguous = MemorySpan{ &fixture.buffer_[4], 3 };

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rd_ccontiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rw_contiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_memory_span, rw_contiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_cmemory_span, rd_ccontiguous), false);
    })

    .TestCase("Empty spans do not overlap with any other span.", [](auto& fixture)
    {
        auto rw_span = RWMemorySpan{ &fixture.buffer_[0], 4 };
        auto rw_empty = RWMemorySpan{};

        auto rd_span = MemorySpan{ &fixture.buffer_[0], 4 };
        auto rd_empty = MemorySpan{};

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_empty, rw_empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_span, rw_empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_empty, rw_span), false);

        SYNTROPY_UNIT_EQUAL(Overlaps(rd_empty, rd_empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_span, rd_empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_empty, rd_span), false);
    })

    .TestCase("Overlapping test is commutative.", [](auto& fixture)
    {
        auto rw_left = RWMemorySpan{ &fixture.buffer_[0], 4 };
        auto rw_right = RWMemorySpan{ &fixture.buffer_[2], 4 };

        auto rd_left = MemorySpan{ &fixture.buffer_[0], 4 };
        auto rd_right = MemorySpan{ &fixture.buffer_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_left, rw_right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_right, rw_left), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(rd_left, rd_right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_right, rd_left), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(rw_left, rd_right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rw_right, rd_left), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(rd_left, rw_right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(rd_right, rw_left), true);
    })

    .TestCase("Memory spans are aligned to the same alignment requirement of the memory region they refer to.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.aligned_buffer16_[0], 4 };
        auto rd_memory_span = MemorySpan{ &fixture.aligned_buffer16_[0], 4 };

        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rw_memory_span, 2_Alignment), true);

        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 128_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 64_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 32_Alignment), false);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 16_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 8_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 4_Alignment), true);
        SYNTROPY_UNIT_EQUAL(IsAlignedTo(rd_memory_span, 2_Alignment), true);
    })

    .TestCase("Aligning a memory span to a value less than the original alignment returns the same span.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.aligned_buffer16_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Align(rw_memory_span, 16_Alignment), rw_memory_span);
    })

    .TestCase("Aligning an unaligned memory span reduces the span size by the difference between the original alignment and the requested one.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.aligned_buffer16_[1], 9 };
        auto rw_memory_span_aligned = RWMemorySpan{ &fixture.aligned_buffer16_[8], 2 };

        SYNTROPY_UNIT_EQUAL(Align(rw_memory_span, 8_Alignment), rw_memory_span_aligned);
    })

    .TestCase("Over-aligning a memory span returns an empty span.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_memory_span = RWMemorySpan{ &fixture.aligned_buffer16_[8], 2 };

        SYNTROPY_UNIT_EQUAL(Align(rw_memory_span, 16_Alignment), RWMemorySpan{});
    })

    .TestCase("Memory spans can be converted to strongly-typed spans", [](auto& fixture)
    {
        using namespace literals;

        auto rw_span = RWSpan<Fix64>{ fixture.union_.elements_, 4 };
        auto rw_memory_span = RWMemorySpan{ fixture.union_.raw_, 32 };

         //auto blah = ToSpan<Fix64>(rw_memory_span);
 
         //auto k = (blah == rw_span);

         //SYNTROPY_UNIT_EQUAL(blah, rw_span);
    })

    .TestCase("Strongly-typed spans can be converted to memory spans and read-only memory spans.", [](auto& fixture)
    {
        using namespace literals;

        auto rw_span = RWSpan<Fix64>{ fixture.union_.elements_, 4 };
        auto rw_memory_span = RWMemorySpan{ fixture.union_.raw_, 32 };
        auto rd_memory_span = MemorySpan{ fixture.union_.raw_, 32 };

        SYNTROPY_UNIT_EQUAL(ToRWMemorySpan(rw_span), rw_memory_span);
        SYNTROPY_UNIT_EQUAL(ToMemorySpan(rw_span), rd_memory_span);
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