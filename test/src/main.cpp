/// \file main.cpp
/// \brief Defines the entry point of the unit test application.
///
/// \author Raffaele D. Facendola - June 2020.

#include "application/unit_test_application.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    auto result = static_cast<int>(Syntropy::UnitTestApplication({ argc, argv }).Run());

    system("pause");

    return result;
}


