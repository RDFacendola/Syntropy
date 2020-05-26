
/// \file visitor.h
/// \brief Contains unit tests for the visitor class.
///
/// \author Raffaele D. Facendola - May 2020.

#include "syntropy/language/visitor.h"

#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/test_macros.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* VISITOR UNIT TEST                                                    */
    /************************************************************************/

    /// \brief Unit test for syntropy::Visitor
    struct VisitorUnitTest
    {
        /// \brief Basic test case for visit functionality.
        void Visit() const;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto VisitorTestSuite = MakeAutoTestSuite<VisitorUnitTest>("VisitorUnitTest");

    inline const auto VisitorVisitTestCase = MakeAutoTestCase("Visit", &VisitorUnitTest::Visit);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VisitorUnitTest.

    inline void VisitorUnitTest::Visit() const
    {
        auto accept_int = [](int x) { SYNTROPY_UNIT_SUCCESS("visitor.Visit(int)"); };
        auto accept_float = [](float x) { SYNTROPY_UNIT_FAIL("visitor.Visit(float)"); };

        auto visitor = MakeVisitor(accept_int, accept_float);

        int visitable = 10;

        visitor.Visit(visitable);
    }

}