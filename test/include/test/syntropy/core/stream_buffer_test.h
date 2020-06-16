/// \file memory_buffer.h
/// \brief Contains unit tests for the MemoryBuffer class.
///
/// \author Raffaele D. Facendola - May 2020.

#pragma once

#include "syntropy/memory/bytes.h"

#include "syntropy/core/types.h"

#include "syntropy/experimental/serialization/streams/stream_buffer.h"
#include "syntropy/experimental/serialization/streams/stream_buffer_transaction.h"

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
        void Before();

        /// \brief Test for append \ consume functionalities.
        void AppendConsume();

        /// \brief Test for transactional functionalities.
        void Transactions();

        /// \brief Stream buffer.
        StreamBuffer stream_buffer_;

        /// \brief Const memory range.
        ConstMemoryRange const_memory_range_;
        
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

    inline void StreamBufferTest::Before()
    {
        stream_buffer_ = {};
        const_memory_range_ = {};
    }

    inline void StreamBufferTest::AppendConsume()
    {
        // Empty.

        SYNTROPY_UNIT_TRACE(const_memory_range_ = stream_buffer_.Append({}));

        SYNTROPY_UNIT_TEST(const_memory_range_.GetSize() == Bytes{ 0 });
        SYNTROPY_UNIT_TEST(stream_buffer_.IsEmpty());


        // Append.

        SYNTROPY_UNIT_TRACE(const_memory_range_ = stream_buffer_.Append(MakeConstMemoryRange(ToFix16(42))));

        SYNTROPY_UNIT_TEST(const_memory_range_.GetSize() == BytesOf<Fix16>());
        SYNTROPY_UNIT_TEST(stream_buffer_.GetSize() == BytesOf<Fix16>());
        SYNTROPY_UNIT_TEST(!stream_buffer_.IsEmpty());
        SYNTROPY_UNIT_TEST(stream_buffer_.GetCapacity() >= stream_buffer_.GetSize());

        SYNTROPY_UNIT_TRACE(const_memory_range_ = stream_buffer_.Append(MakeConstMemoryRange(ToFloat(42.0f))));

        SYNTROPY_UNIT_TEST(const_memory_range_.GetSize() == BytesOf<Float>());
        SYNTROPY_UNIT_TEST(stream_buffer_.GetSize() == (BytesOf<Fix16>() + BytesOf<Float>()));
        SYNTROPY_UNIT_TEST(!stream_buffer_.IsEmpty());

        //



    }

    inline void StreamBufferTest::Transactions()
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