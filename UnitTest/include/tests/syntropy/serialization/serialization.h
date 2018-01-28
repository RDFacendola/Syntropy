
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
		std::string nickname_;

		const std::string& GetName() const { return name_; };
		void SetName(std::string name) { name_ = name; };
    };

    /// \brief Class used for serialization test cases.
    struct Cat : Pet
    {
		
    };

    static std::vector<syntropy::TestCase> GetTestCases();

    TestSyntropySerialization();

    /// \brief Test object deserialization.
    void TestDeserialization();

	/// \brief Test object serialization.
	void TestSerialization();

	/// \brief Tests if object serialization is successful.
	template<typename TClass, typename TPredicate>
	void TestSerializationResults(const nlohmann::json& json, const TClass& object, TPredicate&& predicate);

private:

    const syntropy::reflection::Class* cat_class_;
};

template<typename TClass, typename TPredicate>
void TestSyntropySerialization::TestSerializationResults(const nlohmann::json& json, const TClass& object, TPredicate&& predicate)
{
	auto copy = *syntropy::serialization::DeserializeObjectFromJSON<TClass>(json);
	SYNTROPY_UNIT_ASSERT(predicate(object, copy));
}
