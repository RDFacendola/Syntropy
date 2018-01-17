
/// \file serialization.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include "reflection/reflection.h"
#include "reflection/class.h"



#include "nlohmann/json/src/json.hpp"

/************************************************************************/
/* TEST SYNTROPY SERIALIZATION                                          */
/************************************************************************/

/// \brief Test suite used to test serialization functionalities within Syntropy.
class TestSyntropySerialization : public syntropy::TestFixture
{
public:

    /// \brief Class used for serialization test cases.
    struct Pet
    {
        std::string name_;
    };

    /// \brief Class used for serialization test cases.
    struct Cat : Pet
    {

    };

    static std::vector<syntropy::TestCase> GetTestCases();

    TestSyntropySerialization();

    /// \brief Test object deserialization.
    void TestDeserialization();

private:

    const syntropy::reflection::Class* cat_class_;
};
