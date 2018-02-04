#include "tests/syntropy/reflection/reflection.h"

#include "unit_test/test_runner.h"

#include "reflection/interfaces/class_interfaces.h"
#include "reflection/interfaces/property_interfaces.h"

#include "reflection/types/fundamental_types.h"
#include "reflection/types/core_types.h"
#include "reflection/types/stl_types.h"

#include <sstream>
#include <iomanip>

/************************************************************************/
/* TEST CLASSES                                                         */
/************************************************************************/

// Pet

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropyReflection::Pet>
{
    static constexpr const char* name_{ "TestSyntropyReflection::Pet" };

    void operator()(ClassT<TestSyntropyReflection::Pet>& class_t) const
    {
        class_t.AddProperty("Name", &TestSyntropyReflection::Pet::name_);
        class_t.AddProperty("Age", &TestSyntropyReflection::Pet::age_);
        class_t.AddProperty("Paws", &TestSyntropyReflection::Pet::GetPawsCount);      // Read only!
    }
};

TestSyntropyReflection::Pet::Pet(size_t paws_count)
    : paws_count_(paws_count)
{

}

size_t TestSyntropyReflection::Pet::GetPawsCount() const
{
    return paws_count_;
}

// IPurrable

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropyReflection::IPurrable>
{
    static constexpr const char* name_{ "TestSyntropyReflection::IPurrable" };
};

// Cat

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropyReflection::Cat>
{
    static constexpr const char* name_{ "TestSyntropyReflection::Cat" };

    void operator()(ClassT<TestSyntropyReflection::Cat>& class_t) const
    {
        class_t.AddNameAlias("Catto");

        class_t.AddBaseClass<TestSyntropyReflection::Pet>();
        class_t.AddBaseClass<TestSyntropyReflection::IPurrable>();

        //definition.AddProperty("Toxic", &TestSyntropyReflection::Base::const_);   // #TODO This shouldn't compile.
    }
};

TestSyntropyReflection::Cat::Cat()
    : Pet(4)
{

}

void TestSyntropyReflection::Cat::Purr()
{
    // Purrrrrr!
}

// Canary

template <>
struct syntropy::reflection::ClassDeclarationT<TestSyntropyReflection::Canary>
{
    static constexpr const char* name_{ "TestSyntropyReflection::Canary" };

    void operator()(ClassT<TestSyntropyReflection::Cat>& class_t) const
    {
        class_t.AddBaseClass<TestSyntropyReflection::Pet>();
    }
};

TestSyntropyReflection::Canary::Canary()
    : Pet(2)
{

}

/************************************************************************/
/* TEST SYNTROPY REFLECTION                                             */
/************************************************************************/

syntropy::AutoTestSuite<TestSyntropyReflection> suite("syntropy.reflection.reflection");

std::vector<syntropy::TestCase> TestSyntropyReflection::GetTestCases()
{
    return
    {
        { "class names", &TestSyntropyReflection::TestClassNames },
        { "dynamic class", &TestSyntropyReflection::TestDynamicClass },
        { "class attributes", &TestSyntropyReflection::TestClassAttributes },
        { "class inheritance", &TestSyntropyReflection::TestClassInheritance },
        { "class properties", &TestSyntropyReflection::TestClassProperties },
        { "class instancing", &TestSyntropyReflection::TestClassInstancing },
        { "property read", &TestSyntropyReflection::TestPropertyRead },
        { "property write", &TestSyntropyReflection::TestPropertyWrite },
        { "property move", &TestSyntropyReflection::TestPropertyMove }
    };
}

TestSyntropyReflection::TestSyntropyReflection()
{
    using namespace syntropy::reflection;

    pet_class_ = &ClassOf<Pet>();
    purrable_interface_ = &ClassOf<IPurrable>();
    cat_class_ = &ClassOf<Cat>();
    canary_class_ = &ClassOf<Canary>();
}

void TestSyntropyReflection::TestClassNames()
{
    using namespace syntropy::reflection;

    SYNTROPY_UNIT_ASSERT(cat_class_->GetDefaultName() == "TestSyntropyReflection::Cat");
    SYNTROPY_UNIT_ASSERT(cat_class_->GetNameAliases().size() == 1);
    SYNTROPY_UNIT_ASSERT(cat_class_->GetNameAliases().front() == "Catto");
}

void TestSyntropyReflection::TestDynamicClass()
{
    using namespace syntropy::reflection;

    SYNTROPY_UNIT_TRACE(auto cat_instance = std::make_unique<Cat>());
    SYNTROPY_UNIT_TRACE(Pet* pet_instance = cat_instance.get());

    SYNTROPY_UNIT_ASSERT(ClassOf(*pet_instance) == *cat_class_);
}

void TestSyntropyReflection::TestClassAttributes()
{
    using namespace syntropy::reflection;

    SYNTROPY_UNIT_ASSERT(!cat_class_->IsAbstract());
    SYNTROPY_UNIT_ASSERT(purrable_interface_->IsAbstract());
}

void TestSyntropyReflection::TestClassInheritance()
{
    using namespace syntropy::reflection;

    SYNTROPY_UNIT_ASSERT(cat_class_->IsA(*pet_class_));
    SYNTROPY_UNIT_ASSERT(cat_class_->IsA(*purrable_interface_));

    SYNTROPY_UNIT_ASSERT(!pet_class_->IsA(*cat_class_));            // Inheritance should be antisymmetric!
    SYNTROPY_UNIT_ASSERT(!canary_class_->IsA(*cat_class_));         // Unrelated classes.
}

void TestSyntropyReflection::TestClassProperties()
{
    using namespace syntropy::reflection;

    {
        SYNTROPY_UNIT_ASSERT(pet_class_->GetProperty("Name"));
        SYNTROPY_UNIT_ASSERT(pet_class_->GetProperty("Age"));
        SYNTROPY_UNIT_ASSERT(pet_class_->GetProperty("Paws"));
    }

    {
        // Derived classes may not return a parent's property (the result may be ambiguous).
        // #TODO Add a boolean to search inside parents as well?

        SYNTROPY_UNIT_ASSERT(!cat_class_->GetProperty("Name"));
        SYNTROPY_UNIT_ASSERT(!cat_class_->GetProperty("Age"));
        SYNTROPY_UNIT_ASSERT(!cat_class_->GetProperty("Paws"));
    }
}

void TestSyntropyReflection::TestClassInstancing()
{
    using syntropy::reflection::Constructible;

    SYNTROPY_UNIT_TRACE(auto constructible_class = cat_class_->GetInterface<Constructible<>>());

    SYNTROPY_UNIT_ASSERT(constructible_class != nullptr);

    SYNTROPY_UNIT_TRACE(auto cat = (*constructible_class)());

    SYNTROPY_UNIT_ASSERT(syntropy::reflection::AnyCast<Cat*>(&cat) != nullptr);
}

void TestSyntropyReflection::TestPropertyRead()
{
    SYNTROPY_UNIT_SKIP("Not yet implemented.");

    using syntropy::reflection::Readable;

    Cat sylvester;
    Canary tweety;

    sylvester.name_ == "Sylvester";
    tweety.name_ == "Tweety";

    SYNTROPY_UNIT_TRACE(auto name_property = pet_class_->GetProperty("Name"));
    SYNTROPY_UNIT_TRACE(auto paws_property = pet_class_->GetProperty("Paws"));

    SYNTROPY_UNIT_TRACE(auto name_reader = name_property->GetInterface<Readable>());
    SYNTROPY_UNIT_TRACE(auto paws_reader = paws_property->GetInterface<Readable>());

    SYNTROPY_UNIT_ASSERT(name_reader != nullptr);
    SYNTROPY_UNIT_ASSERT(paws_reader != nullptr);

    //SYNTROPY_UNIT_TRACE(auto sylverster_name = (*name_reader)(sylvester));
    //SYNTROPY_UNIT_TRACE(auto tweety_name = (*name_reader)(tweety));

    //SYNTROPY_UNIT_TRACE(auto sylverster_paws = (*paws_reader)(sylvester));
    //SYNTROPY_UNIT_TRACE(auto tweety_paws = (*paws_reader)(tweety));

    // #TODO Is it possible to marshal among implicitly-convertible types?
    // #TODO Make sure it is possible to read a property on a derived object as well.
    // #TODO Make sure it works for reflection::Any as well.
}

void TestSyntropyReflection::TestPropertyWrite()
{
    SYNTROPY_UNIT_SKIP("Not yet implemented.");

    // Write via property setter.
    // Verify movable objects cannot be written (MoveWriteable?)

    //Cat sylvester;

    //SYNTROPY_UNIT_TRACE(auto name_property_ = pet_class_->GetProperty("Name"));

    //SYNTROPY_UNIT_TRACE(auto name_writer = name_property_->GetInterface<syntropy::reflection::Writeable>());

    //SYNTROPY_UNIT_TRACE(sylvester.name_ == "Sylvester");

    //SYNTROPY_UNIT_TRACE((*name_writer)(sylvester, "Tom"));

    //SYNTROPY_UNIT_ASSERT(sylvester.name_ == "Tom");

    // #TODO Add support to movable properties.
    // #TODO Is it possible to marshal among implicitly-convertible types?
    // #TODO Make sure it is possible to write a property on a derived object as well.
    // #TODO Make sure it works for reflection::Any as well.
}

void TestSyntropyReflection::TestPropertyMove()
{
    SYNTROPY_UNIT_SKIP("Not yet implemented.");

    // #TODO Implement syntropy::reflection::Moveable.
    // #TODO Should be different than Readable as an outward move does change the object status.
    // #TODO Inward move should be part of the Writable interface as from the object point of view they are the same.
    // #TODO Is it possible to marshal among implicitly-convertible types?
    // #TODO Make sure it works for reflection::Any as well.
}

