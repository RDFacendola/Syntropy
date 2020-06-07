

/// \file memory_buffer.h
/// \brief Contains unit tests for the MemoryBuffer class.
///
/// \author Raffaele D. Facendola - May 2020.

#include "syntropy/memory/bytes.h"
#include "syntropy/core/stream_buffer.h"

#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/test_macros.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* BYTE STRING UNIT TEST                                                */
    /************************************************************************/

    /// \brief Unit test for ByteString.
    struct ByteStringUnitTest
    {
        /// \brief Basic operations on ByteString.
        void BasicOperations() const;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto ByteStringTestSuite = MakeAutoTestSuite<ByteStringUnitTest>("ByteStringUnityTest");

    inline const auto ByteStringSerializationTestCase = MakeAutoTestCase("BasicOperations", &ByteStringUnitTest::BasicOperations);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ByteStringUnitTest.

    inline void ByteStringUnitTest::BasicOperations() const
    {
        using namespace syntropy::Literals;

        auto stream = StreamBuffer{};

        Int ar = 5;
        Float br = 42.01f;
        Int cr = 1000;
        Float dr = 1000.87f;

        Int aw = 0;
        Float bw = 0.0f;
        Int cw = 0;
        Float dw = 0.0f;

        stream.WriteSequential(MakeConstMemoryRange(ar));
        stream.WriteSequential(MakeConstMemoryRange(br));

        stream.ReadSequential(MakeMemoryRange(aw));
        stream.ReadSequential(MakeMemoryRange(bw));

        stream.WriteSequential(MakeConstMemoryRange(cr));
        stream.WriteSequential(MakeConstMemoryRange(dr));

        stream.ReadSequential(MakeMemoryRange(cw));
        stream.ReadSequential(MakeMemoryRange(dw));

        auto size = stream.GetSize();


    }

}