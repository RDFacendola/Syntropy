

/// \file memory_buffer.h
/// \brief Contains unit tests for the MemoryBuffer class.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

#include "syntropy/memory/bytes.h"

#include "syntropy/serialization/memory_stream_buffer.h"
#include "syntropy/serialization/append_stream_buffer.h"
#include "syntropy/serialization/consume_stream_buffer.h"

#include "syntropy/serialization/append_stream.h"
#include "syntropy/serialization/consume_stream.h"

#include "syntropy/serialization/msgpack/msgpack_stream_encoder.h"

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

        Fix32 a = 0;
        Fix32 b = 0;
        Fix32 c = 0;
        Float d = 0;

        auto out_buffer = MemoryStreamBuffer{};

        auto msgpack_writer = MakeAppendStream<MsgpackStreamEncoder>(out_buffer);

        msgpack_writer << 4 << 5 << 42.0f;



        //auto msgpack_writer = MsgpackWriter{ out_stream };

        //msgpack_writer << 4 << 5 << 42.0f;

        //auto writer = StreamWriter{ out_stream };
        //auto reader = StreamReader{ in_stream };

        //writer << 4 << 5 << 42.30f;

        //Fix32 a = 0;
        //Fix32 b = 0;
        //Float c = 0.0f;

        //reader >> a >> b >> c;

    }

}