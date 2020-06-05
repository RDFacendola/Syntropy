
/// \file msgpack.h
/// \brief Contains unit tests for the visitor class.
///
/// \author Raffaele D. Facendola - May 2020.

#include "syntropy/serialization/msgpack/msgpack_format.h"
#include "syntropy/serialization/msgpack/msgpack_reader.h"
#include "syntropy/serialization/msgpack/msgpack_writer.h"
#include "syntropy/serialization/msgpack/msgpack_vector_extensions.h"

#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/test_macros.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* MSGPACK UNIT TEST                                                    */
    /************************************************************************/

    /// \brief Unit test for Msgpack serialization.
    struct MsgpackUnitTest
    {
        /// \brief Msgpack serialization\deserialization test.
        void Serialization() const;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto MsgpackTestSuite = MakeAutoTestSuite<MsgpackUnitTest>("MsgpackUnitTest");

    inline const auto MsgpackSerializationTestCase = MakeAutoTestCase("Serialization", &MsgpackUnitTest::Serialization);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MsgpackUnitTest.

    inline void MsgpackUnitTest::Serialization() const
    {
        auto writer = MsgpackWriter();


        auto reader = MsgpackReader();
    }

}