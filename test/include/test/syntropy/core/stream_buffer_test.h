

/// \file memory_buffer.h
/// \brief Contains unit tests for the MemoryBuffer class.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

#include "syntropy/memory/bytes.h"

#include "syntropy/experimental/serialization/streams/stream_buffer.h"

#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/test_macros.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* STREAM BUFFER TEST                                                   */
    /************************************************************************/

    /// \brief Unit test for StreamBuffer.
    struct StreamBufferTest
    {
        /// \brief Test for append \ consume functionalities.
        void AppendConsume() const;

        /// \brief Test for transactional functionalities.
        void Transactions() const;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto StreamBufferTestSuite = MakeAutoTestSuite<StreamBufferTest>("StreamBufferTest");

    inline const auto StreamBufferAppendConsumeTestCase = MakeAutoTestCase("AppendConsume", &StreamBufferTest::AppendConsume);
    inline const auto StreamBufferTransactionsTestCase = MakeAutoTestCase("Transactions", &StreamBufferTest::Transactions);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ByteStringUnitTest.

    inline void StreamBufferTest::AppendConsume() const
    {
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

    inline void StreamBufferTest::Transactions() const
    {
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