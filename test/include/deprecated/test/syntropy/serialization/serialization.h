
/// \file serialization.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/unit_test/test_fixture.h"
#include "syntropy/unit_test/test_case.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/class.h"

#include "syntropy/serialization/json/shared.h"

#include "syntropy/containers/vector.h"
#include "syntropy/containers/set.h"
#include "syntropy/types/string.h"

/************************************************************************/
/* TEST SYNTROPY SERIALIZATION                                          */
/************************************************************************/

/// \brief Test suite used to test serialization functionalities within Syntropy.
class TestSyntropySerialization : public syntropy::TestFixture
{
public:

    enum class Animal
    {
        kPet,
        kWild,
        kUnknown
    };

    /// \brief Class used for serialization test cases.
    struct Pet
    {
        syntropy::String name_;
        syntropy::String nickname_;

        syntropy::Vector<int> numbers_;
        syntropy::Set<syntropy::String> names_;
        syntropy::Map<std::shared_ptr<int>, syntropy::Label> map_;

        syntropy::Label hashed_string_;
        syntropy::Context context_;

        Animal animal_;

        const syntropy::String& GetName() const { return name_; };
        void SetName(syntropy::String name) { name_ = name; };
    };

    /// \brief Class used for serialization test cases.
    struct Cat : Pet
    {
        
    };

    static syntropy::Vector<syntropy::TestCase> GetTestCases();

    TestSyntropySerialization();

    /// \brief Test object deserialization.
    void TestDeserialization();

    /// \brief Test object serialization.
    void TestSerialization();

    /// \brief Tests if object serialization is successful.
    template<typename TClass, typename TPredicate>
    void TestSerializationResults(const syntropy::serialization::JSON& json, const TClass& object, TPredicate&& predicate);

private:

    const syntropy::reflection::Class* cat_class_;
};

template<typename TClass, typename TPredicate>
void TestSyntropySerialization::TestSerializationResults(const syntropy::serialization::JSON& json, const TClass& object, TPredicate&& predicate)
{
    auto copy = *syntropy::serialization::template DeserializeObjectFromJSON<TClass>(json);
    SYNTROPY_UNIT_ASSERT(predicate(object, copy));
}
