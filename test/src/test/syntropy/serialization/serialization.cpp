#include "test/syntropy/serialization/serialization.h"

#include "syntropy/unit_test/test_runner.h"

#include "syntropy/reflection/class.h"

#include "syntropy/reflection/interfaces/class_interfaces.h"
#include "syntropy/reflection/interfaces/property_interfaces.h"

#include "syntropy/reflection/types/fundamental_types.h"
#include "syntropy/reflection/types/core_types.h"
#include "syntropy/reflection/types/stl_types.h"

#include "syntropy/serialization/json/json.h"

#include "syntropy/serialization/json/deserializers/fundamental_deserializers.h"
#include "syntropy/serialization/json/deserializers/stl_deserializers.h"
#include "syntropy/serialization/json/deserializers/core_deserializers.h"

#include "syntropy/serialization/json/serializers/stl_serializers.h"
#include "syntropy/serialization/json/serializers/core_serializers.h"
#include "syntropy/serialization/json/serializers/fundamental_serializers.h"

#include <sstream>
#include <iomanip>

/************************************************************************/
/* TEST CLASSES                                                         */
/************************************************************************/

// Animal

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Animal>
{
	static constexpr const char* name_{ "TestSyntropySerialization::Animal" };


	void operator()(ClassT<TestSyntropySerialization::Animal>& class_t) const
	{
		using syntropy::reflection::Enumeration;
		using syntropy::serialization::JSONConvertible;
		using syntropy::serialization::JSONConstructible;

		
		class_t << reflection::EnumerationClass<TestSyntropySerialization::Animal>(
			{
			{ "Pet", TestSyntropySerialization::Animal::kPet },
			{ "Wild", TestSyntropySerialization::Animal::kWild },
			{ "Unknown", TestSyntropySerialization::Animal::kUnknown }
			});

		
		class_t.AddInterface<JSONConvertible>();
	}

};

// Pet

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Pet>
{
    static constexpr const char* name_{ "TestSyntropySerialization::Pet" };


    void operator()(ClassT<TestSyntropySerialization::Pet>& class_t) const
    {
        using syntropy::serialization::JSONProperty;
        using syntropy::serialization::JSONConvertible;

        class_t.AddProperty("Name", &TestSyntropySerialization::Pet::GetName, &TestSyntropySerialization::Pet::SetName) << JSONProperty();
        class_t.AddProperty("Nickname", &TestSyntropySerialization::Pet::nickname_) << JSONProperty();
		class_t.AddProperty("Numbers", &TestSyntropySerialization::Pet::numbers_) << JSONProperty();
		class_t.AddProperty("Names", &TestSyntropySerialization::Pet::names_) << JSONProperty();
		class_t.AddProperty("Map", &TestSyntropySerialization::Pet::map_) << JSONProperty();
		class_t.AddProperty("Label", &TestSyntropySerialization::Pet::hashed_string_) << JSONProperty();
		class_t.AddProperty("Context", &TestSyntropySerialization::Pet::context_) << JSONProperty();
		class_t.AddProperty("Animal", &TestSyntropySerialization::Pet::animal_) << JSONProperty();
        class_t.AddInterface<JSONConvertible>();
    }

};

// Cat

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropySerialization::Cat>
{
    static constexpr const char* name_{ "TestSyntropySerialization::Cat" };

    void operator()(ClassT<TestSyntropySerialization::Cat>& class_t) const
    {
        class_t.AddBaseClass<TestSyntropySerialization::Pet>();
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
	Petto.numbers_ = { 3 , 4 , 5 };
	Petto.names_ = { "A", "B", "C" };
	Petto.map_ = { {std::make_shared<int>(3), "1"} , { std::make_shared<int>(4), "2" } };
	Petto.hashed_string_ = syntropy::Label("Mammt");
	Petto.context_ = Petto.hashed_string_;
	Petto.animal_ = TestSyntropySerialization::Animal::kUnknown;

    std::optional<nlohmann::json> json = SerializeObjectToJSON(Petto);

    TestSerializationResults(*json, Petto, 
        [](const TestSyntropySerialization::Pet& A, const TestSyntropySerialization::Pet& B) -> bool
    {
        return A.name_ == B.name_
            && A.nickname_ == B.nickname_;
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
