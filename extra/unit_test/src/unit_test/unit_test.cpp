/// \file unit_test.cpp
///
/// Runs the entire Syntropy test suite.
///
/// \author Raffaele D. Facendola - April 2021

// ========================================================================= //

#include "unit_test/unit_test_application.h"

#include <iostream>

int main()
{
    return UnitTest::UnitTestApplication{}.Run();
}

// ========================================================================= //
