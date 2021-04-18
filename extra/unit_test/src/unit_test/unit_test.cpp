
/// \file unit_test.cpp
///
/// Runs the entire Syntropy test suite.
///
/// \author Raffaele D. Facendola - April 2021

// ========================================================================= //

#include "unit_test/unit_test_application.h"

#include "test_suites/syntropy/core/algorithms/compare_unit_test.h"
#include "test_suites/syntropy/core/algorithms/swap_unit_test.h"

#include "test_suites/syntropy/language/templates/concepts_unit_test.h"
#include "test_suites/syntropy/language/templates/invoke_unit_test.h"
#include "test_suites/syntropy/language/templates/math_unit_test.h"
#include "test_suites/syntropy/language/templates/priority_unit_test.h"
#include "test_suites/syntropy/language/templates/ratio_unit_test.h"
#include "test_suites/syntropy/language/templates/templates_unit_test.h"
#include "test_suites/syntropy/language/templates/type_traits_unit_test.h"


// ===========================================================================

int main()
{
    return UnitTest::UnitTestApplication{}.Run();
}

// ========================================================================= //
