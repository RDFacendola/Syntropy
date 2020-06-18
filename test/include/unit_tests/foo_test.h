/// \file foo_test_suite.h
/// \brief Contains empty templates for a test suite and many test cases.
///
/// \author Raffaele D. Facendola - June 2020.

#pragma once

namespace syntropy::unit_test
{
    /************************************************************************/
    /* FOO TEST SUITE                                                       */
    /************************************************************************/

    /// \brief Foo test suite.
    struct FooTestSuite
    {
        /// \brief Setup the fixture before each test case. Can be omitted.
        void Before();

        /// \brief Tear down the fixture after each test case. Can be omitted.
        void After();

        /// \brief Test case
        void FooTestCase();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// IMPORTANT! This file must be added to "unit_tests.h" otherwise it won't be run.

    inline const auto AutoFooTestSuite = MakeAutoTestSuite<FooTestSuite>("FooTestSuite");

    inline const auto AutoFooTestCase = MakeAutoTestCase("FooTestCase", &FooTestSuite::FooTestCase);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FooTestSuite.
    
    inline void FooTestSuite::Before()
    {

    }

    inline void FooTestSuite::After()
    {

    }

    inline void FooTestSuite::FooTestCase()
    {

    }

}