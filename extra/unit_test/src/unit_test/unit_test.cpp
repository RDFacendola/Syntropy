
/// \file unit_test.cpp
///
/// Runs the entire Syntropy test suite.
///
/// \author Raffaele D. Facendola - April 2021

// ========================================================================= //

#include "unit_test/unit_test_application.h"

#include "test_suites/syntropy/language/templates/ut_concepts.h"
#include "test_suites/syntropy/language/templates/ut_invoke.h"
#include "test_suites/syntropy/language/templates/ut_math.h"
#include "test_suites/syntropy/language/templates/ut_priority.h"
#include "test_suites/syntropy/language/templates/ut_ratio.h"
#include "test_suites/syntropy/language/templates/ut_templates.h"
#include "test_suites/syntropy/language/templates/ut_type_traits.h"

// ===========================================================================

int main()
{
    return UnitTest::UnitTestApplication{}.Run();
}

// ========================================================================= //
