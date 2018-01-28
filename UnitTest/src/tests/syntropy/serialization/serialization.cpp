#include "tests/syntropy/serialization/serialization.h"

#include "unit_test/test_runner.h"

#include "reflection/interfaces/class_interfaces.h"
#include "reflection/interfaces/property_interfaces.h"

#include "reflection/types/fundamental_types.h"
#include "reflection/types/core_types.h"
#include "reflection/types/stl_types.h"

#include "serialization/json/json.h"
#include "serialization/json/deserializers/stl_deserializers.h"
#include "serialization/json/deserializers/core_deserializers.h"

#include <sstream>
#include <iomanip>

/************************************************************************/
/* TEST CLASSES                                                         */
/************************************************************************/

// Collar

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Collar>
{
	static constexpr const char* name_{ "TestSyntropySerialization::Collar" };


	void operator()(ClassDefinitionT<TestSyntropySerialization::Collar>& definition) const
	{		
		using syntropy::serialization::JSONClass;
		using syntropy::serialization::JSONProperty;
		using syntropy::serialization::JSONConvertible;

		definition << JSONClass();
		definition.DefineProperty("Colour", &TestSyntropySerialization::Collar::colour_) << JSONProperty();
		definition.AddInterface<JSONConvertible>();
	}
};

// Pet

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Pet>
{
    static constexpr const char* name_{ "TestSyntropySerialization::Pet" };


    void operator()(ClassDefinitionT<TestSyntropySerialization::Pet>& definition) const
    {
        using syntropy::serialization::JSONProperty;
		using syntropy::serialization::JSONConvertible;

		definition.DefineProperty("Name", &TestSyntropySerialization::Pet::GetName, &TestSyntropySerialization::Pet::SetName) << JSONProperty();
		definition.DefineProperty("Nickname", &TestSyntropySerialization::Pet::nickname_) << JSONProperty();
		definition.DefineProperty("Collar", &TestSyntropySerialization::Pet::collar_) << JSONProperty();
		definition.AddInterface<JSONConvertible>();
	}

};

// Cat

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Cat>
{
    static constexpr const char* name_{ "TestSyntropySerialization::Cat" };

    void operator()(ClassDefinitionT<TestSyntropySerialization::Cat>& definition) const
    {
        definition.DefineBaseClass<TestSyntropySerialization::Pet>();
    }
};


/************************************************************************/
/* TEST SYNTROPY SERIALIZATION                                          */
/************************************************************************/

syntropy::AutoTestSuite<TestSyntropySerialization> suite("syntropy.serialization.serialization");

std::vector<syntropy::TestCase> TestSyntropySerialization::GetTestCases()
{
    return
    {
        { "deserialization", &TestSyntropySerialization::TestDeserialization },
		{ "serialization", &TestSyntropySerialization::TestSerialization }
    };
}

TestSyntropySerialization::TestSyntropySerialization()
{
    using namespace syntropy::reflection;

    cat_class_ = &ClassOf<Cat>();
}

void TestSyntropySerialization::TestSerialization()
{
	using namespace syntropy::serialization;

	TestSyntropySerialization::Pet Petto;
	Petto.name_ = "Kitty";
	Petto.nickname_ = "Kitten";
	Petto.collar_ = new TestSyntropySerialization::Collar();
	Petto.collar_->colour_= "Blue";

	std::optional<nlohmann::json> json = SerializeObjectToJSON(Petto);

	TestSerializationResults(*json, Petto, 
		[](const TestSyntropySerialization::Pet& A, const TestSyntropySerialization::Pet& B) -> bool
	{
		return A.name_ == B.name_
			&& A.nickname_ == B.nickname_
			&& A.collar_ != nullptr && B.collar_ != nullptr
			&& A.collar_->colour_ == B.collar_->colour_;
	});
}

void TestSyntropySerialization::TestDeserialization()
{
	SYNTROPY_UNIT_SKIP("Not yet implemented.");

	nlohmann::json json = R"({
                                "Name": "Kitty",
                                "float_value": 67.5,
                                "const_value": 100.0,
                                "boolean": true,
                                "string_value": "awesome!",
                                "wstring_value": "wawesome?",
                                "Blob": { 
                                            "blob": 47 
                                        },
                                "vector_int": [1, 2, 5, 5, 5],
                                "map": [ 
                                            {"id": "alpha", "blob": 1 },
                                            {"id": "beta", "blob": 2 },
                                            {"id": "gamma", "blob": 3 },
                                            {"id": "delta", "blob": 4 }
                                        ],
                                "p_blob": {
                                            "$class": "DerivedBlob",
                                            "blob": 1,
                                            "derived_blob": 47
                                            },
                                "UBlob": {
                                            "$class": "DerivedBlob",
                                            "blob": 2,
                                            "derived_blob": 48
                                            },
                                "s_blob": {
                                            "$class": "DerivedBlob",
                                            "blob": 3,
                                            "derived_blob": 49
                                            },
                                "nondefault": 100
                                })"_json;

	auto cat = syntropy::serialization::DeserializeObjectFromJSON<Cat>(json);

	SYNTROPY_UNIT_ASSERT(cat.has_value());

	// #TODO Test actual parameter values.

}
