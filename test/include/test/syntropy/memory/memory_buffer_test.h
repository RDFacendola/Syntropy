

/// \file memory_buffer.h
/// \brief Contains unit tests for the MemoryBuffer class.
///
/// \author Raffaele D. Facendola - May 2020.

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory_buffer.h"

#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/test_macros.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* MEMORY BUFFER UNIT TEST                                              */
    /************************************************************************/

    /// \brief Unit test for MemoryBuffer.
    struct MemoryBufferUnitTest
    {
        /// \brief Basic operations on MemoryBuffer.
        void BaseOperations() const;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto MemoryBufferTestSuite = MakeAutoTestSuite<MemoryBufferUnitTest>("MemoryBufferUnityTest");

    inline const auto MemoryBufferSerializationTestCase = MakeAutoTestCase("BaseOperations", &MemoryBufferUnitTest::BaseOperations);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryBufferUnitTest.

    inline void MemoryBufferUnitTest::BaseOperations() const
    {
        using namespace syntropy::Literals;

        auto buffer = MemoryBuffer(10_Bytes, 8_Align, GetDefaultMemoryResource());


    }

}