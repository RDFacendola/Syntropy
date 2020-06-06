

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

        Int five = 5;
        Int fortytwo = 42;

        Int fiver = 1000;
        Int fortytwor = 1000;

        stream.Append(MakeConstMemoryRange(five));
        stream.Append(MakeConstMemoryRange(fortytwo));

        stream.Read(0, MakeMemoryRange(fiver));
        stream.Read(108, MakeMemoryRange(fortytwor));

        auto c = stream.GetCapacity();

    }

}