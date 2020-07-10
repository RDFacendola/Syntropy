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
        /// \brief Buffer of random bytes.
        Byte buffer_[10];

        /// \brief Equivalent buffer of random bytes.
        Byte buffer_equivalent_[10];

        /// \brief Different buffer of random bytes.
        Byte buffer_different_[10];

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

        SYNTROPY_UNIT_EQUAL(!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 0 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), nullptr);
    })

    .TestCase("Memory spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(!!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], &fixture.buffer_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span.GetSize(), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(Size(memory_span), Bytes{ 10 });
        SYNTROPY_UNIT_EQUAL(memory_span.GetData(), &fixture.buffer_[0]);
    })

    .TestCase("Memory spans are always equivalent to themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(memory_span == memory_span, true);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span, false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(memory_span, memory_span), true);
    })

    .TestCase("Memory spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_span_equivalent = MemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto memory_span_different = MemorySpan{ &fixture.buffer_different_[3], Bytes{ 7 } };

        SYNTROPY_UNIT_EQUAL(memory_span == memory_span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(memory_span == memory_span_different, false);
        SYNTROPY_UNIT_EQUAL(memory_span != memory_span_different, true);
    })

    .TestCase("Memory spans are not identical to other spans, even if they compare equivalent.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_span_equivalent = MemorySpan{ &fixture.buffer_equivalent_[0], Bytes{ 10 } };
        auto memory_span_identical = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, memory_span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(memory_span, memory_span_identical), true);
    })

    .TestCase("Memory spans front elements are readable.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(Front(memory_span), Byte{ 0 });
    })

    .TestCase("Memory spans front elements are writable.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        Front(memory_span) = Byte{ 42 };

        SYNTROPY_UNIT_EQUAL(Front(memory_span), Byte{ 42 });
    })

    .TestCase("Removing front elements from a memory span yields another memory span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto popfront = MemorySpan{ &fixture.buffer_[1], Bytes{ 9 } };

        SYNTROPY_UNIT_EQUAL(PopFront(memory_span), popfront);
    })

    .TestCase("Memory spans contain themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, memory_span), true);
    })

    .TestCase("Memory spans contains another span if the latter refers to a memory location inside the first.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };
        auto memory_subspan = MemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, memory_subspan), true);
    })

    .TestCase("Empty memory spans are contained in any other span.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 10 } };

        SYNTROPY_UNIT_EQUAL(Contains(memory_span, MemorySpan{}), true);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, memory_span), true);
    })

    .TestCase("Disjoint spans do not overlap.", [](auto& fixture)
    {
        auto memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto disjoint = MemorySpan{ &fixture.buffer_[6], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(memory_span, disjoint), false);
    })

    .TestCase("Contiguous spans do not overlap.", [](auto& fixture)
    {
        auto span_memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto contiguous = MemorySpan{ &fixture.buffer_[4], Bytes{ 3 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(span_memory_span, contiguous), false);
    })

    .TestCase("Empty spans do not overlap with any other span.", [](auto& fixture)
    {
        auto span_memory_span = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto empty = MemorySpan{};

        SYNTROPY_UNIT_EQUAL(Overlaps(empty, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(span_memory_span, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(empty, span_memory_span), false);
    })

    .TestCase("Overlapping test is commutative.", [](auto& fixture)
    {
        auto left = MemorySpan{ &fixture.buffer_[0], Bytes{ 4 } };
        auto right = MemorySpan{ &fixture.buffer_[2], Bytes{ 4 } };

        SYNTROPY_UNIT_EQUAL(Overlaps(left, right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(right, left), true);
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
    }

}