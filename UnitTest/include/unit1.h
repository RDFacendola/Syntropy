
/// \file unit1.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy.h"

#include "reflection/reflection.h"
#include "reflection/class_interfaces.h"
#include "reflection/property_interfaces.h"

#include "reflection/fundamental_types.h"
#include "reflection/core_types.h"
#include "reflection/stl_types.h"

#include "serialization/json.h"

#include "nlohmann/json/src/json.hpp"

#include "unit2.h"

#include <tuple>
#include <iostream>
#include <iomanip>

#define SUPPRESS_PASSED_TESTS

#ifndef SUPPRESS_PASSED_TESTS

    #define TEST_TRUE(test) \
    std::cout << __FUNCTION__ << ((test) ? ": PASSED - " : ": NOT PASSED - ")<< #test << " is true\n";

    #define TEST_FALSE(test) \
    std::cout << __FUNCTION__ << (!(test) ? ": PASSED - " : ": NOT PASSED - ") << #test << " is false\n";

#else

    #define TEST_TRUE(test) \
    if(!(test)) std::cout << __FUNCTION__ << ": NOT PASSED - " << #test << " is true\n";

    #define TEST_FALSE(test) \
    if((test)) std::cout << __FUNCTION__ << ": NOT PASSED - " << #test << " is false\n";

#endif

#define RUN_TEST(test) \
test();
//std::cout << "Running test " << #test << "()\n\n"; \
//test(); \
//std::cout << "\n";

struct Blob{


    Blob()
        : blob_(0)
    {
        //std::cout << "Blob - Default constructor.\n";
    }

    Blob(const Blob& other)
        : blob_(other.blob_)
    {
        //std::cout << "Blob - Copy constructor.\n";
    }

    Blob(Blob&& other)
        : blob_(std::move(other.blob_))
    {
        //std::cout << "Blob - Move constructor.\n";
    }

    Blob& operator=(const Blob& other)
    {
        Blob(other).Swap(*this);
        return *this;
    }

    Blob& operator=(Blob&& other)
    {
        Swap(other);
        //Blob(std::move(other)).Swap(*this);
        return *this;
    }

    void Swap(Blob& other) noexcept
    {
        std::swap(blob_, other.blob_);
    }

    virtual ~Blob() = default;

    int blob_;

};

struct StreamableBlob {

    int blob_;

};

struct DerivedBlob : Blob {

    virtual ~DerivedBlob() = default;

    int derived_blob_;

};

inline std::istream& operator >> (std::istream &in, Blob& blob) {

    in >> blob.blob_;

    return in;

}

inline std::ostream& operator << (std::ostream &out, const StreamableBlob& blob) {
 
    out << blob.blob_;
 
    return out;
 
}

class AbstractFoo {

public:

    virtual void BeAbstract() = 0;

};

class Foo : public Bar {

    friend class syntropy::reflection::ClassDefinitionT<Foo>;

public:

    Foo() noexcept
        : const_value_(666)
        , const_pointer_(nullptr)
        , boolean_(false) {}

    Foo(const Foo& other) = delete;
    //    : value_(other.value_)
    //    , const_value_(other.const_value_)
    //    , pointer_(other.pointer_)
    //    , pointer_to_const_(other.pointer_to_const_)
    //    , const_pointer_(other.const_pointer_)
    //    , blob_(other.blob_)
    //    , boolean_(other.boolean_)
    //{
    //    std::cout << "Copy ctor!\n";
    //}

    float GetValue() const {

        return value_;

    }

    void SetValue(float value) {

        value_ = value;

    }

    float GetConstValue() const {

        return const_value_;

    }
    
    float* GetPointer() const {

        return pointer_;

    }

    void SetPointer(float* pointer) {

        pointer_ = pointer;

    }

    const float* GetPointerToConst() const {

        return pointer_to_const_;

    }

    void SetPointerToConst(const float* pointer_to_const) {

        pointer_to_const_ = pointer_to_const;

    }

    float* const GetConstPointer() const {

        return const_pointer_;

    }

    const Blob& GetBlob() const {

        return blob_;

    }

    void SetBlob(const Blob& blob) {

        blob_ = blob;

    }

    const Blob& GetAccessor() const {

        return blob_;

    }

    Blob& GetAccessor() {

        return blob_;

    }

    const std::unique_ptr<Blob>& GetUBlob() const {

        return u_blob_;

    }

    void SetUBlob(std::unique_ptr<Blob> blob) {

        u_blob_ = std::move(blob);

    }

    const std::unique_ptr<Blob>& GetUBlobAccessor() const {

        return u_blob_;

    }

    std::unique_ptr<Blob>& GetUBlobAccessor() {

        return u_blob_;

    }

public:

    float value_;
    int value2_;
    const float const_value_;
    float* pointer_;
    const float * pointer_to_const_;
    float* const const_pointer_;
    bool boolean_;
    std::string string_;
    std::wstring wstring_;

    std::set<int> vector_int_;
    //std::map<syntropy::HashedString, int> map_;
    std::map<std::string, Blob*> map_;
    //std::map<syntropy::HashedString, int> map_;

    Foo* fooptr_;

    Blob blob_;

    Blob* p_blob_;
    std::unique_ptr<Blob> u_blob_;
    std::shared_ptr<Blob> s_blob_;

};

class FooBar : public Foo {

public:

    FooBar() noexcept
        : Foo() {}

    //FooBar(const FooBar& other)
    //    : Foo(other) {}

};

template <>
struct syntropy::reflection::ClassDeclaration<AbstractFoo> {

public:

    static constexpr const char* GetName() noexcept {

        return "AbstractFoo";

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Blob> {

public:

    static constexpr const char* GetName() noexcept {

        return "Blob";

    }

    template <typename TDefinition>
    void operator()(TDefinition& definition) const {

        using syntropy::serialization::JSONRead;

        definition.DefineProperty("blob", &Blob::blob_) << JSONRead();
    }

};



template <>
struct syntropy::reflection::ClassDeclaration<DerivedBlob> {

public:

    static constexpr const char* GetName() noexcept {

        return "DerivedBlob";

    }

    template <typename TDefinition>
    void operator()(TDefinition& definition) const {

        using syntropy::serialization::JSONRead;

        definition.DefineBaseClass<Blob>();
        definition.DefineProperty("derived_blob", &DerivedBlob::derived_blob_) << JSONRead();

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Bar>  {

public:

    static constexpr const char* GetName() noexcept {

        return "Bar";

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Foo> {

public:

    static constexpr const char* GetName() noexcept {

        return "Foo";

    }

    void operator()(ClassDefinitionT<Foo>& definition) const {
        
        using syntropy::serialization::JSONRead;

        definition.DefineBaseClass<Bar>();

        definition.DefineProperty("string_value", &Foo::string_) << JSONRead();
        definition.DefineProperty("wstring_value", &Foo::wstring_) << JSONRead();

        definition.DefineProperty("p_blob", &Foo::p_blob_) << JSONRead();
        definition.DefineProperty("u_blob", &Foo::u_blob_) << JSONRead();
        definition.DefineProperty("s_blob", &Foo::s_blob_) << JSONRead();
        
        definition.DefineProperty("float_value", &Foo::value_) << JSONRead();

        definition.DefineProperty("int_value", &Foo::value2_) << JSONRead();
        definition.DefineProperty("const_value", &Foo::const_value_) /*<< JSONRead()*/;
        definition.DefineProperty("pointer", &Foo::pointer_) << JSONRead();
        definition.DefineProperty("pointer_to_const", &Foo::pointer_to_const_) << JSONRead();
        definition.DefineProperty("const_pointer", &Foo::const_pointer_) /*<< JSONRead()*/;
        definition.DefineProperty("boolean", &Foo::boolean_) << JSONRead();
        definition.DefineProperty("vector_int", &Foo::vector_int_) << JSONRead();
        definition.DefineProperty("map", &Foo::map_) << JSONRead();

        definition.DefineProperty("blob_value", &Foo::blob_);
        
        definition.DefineProperty("Value", &Foo::GetValue, &Foo::SetValue) << JSONRead();
        definition.DefineProperty("ConstValue", &Foo::GetConstValue) /*<< JSONRead()*/;
        definition.DefineProperty("Pointer", &Foo::GetPointer, &Foo::SetPointer) << JSONRead();
        definition.DefineProperty("PointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst) << JSONRead();
        definition.DefineProperty("ConstPointer", &Foo::GetConstPointer) /*<< JSONRead()*/;
        definition.DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob) << JSONRead();
        definition.DefineProperty("UBlob", &Foo::GetUBlob, &Foo::SetUBlob) << JSONRead();
       

        definition.DefineProperty("Accessor",
                                  static_cast<const Blob&(Foo::*)() const>(&Foo::GetAccessor), 
                                  static_cast<Blob&(Foo::*)()>(&Foo::GetAccessor)) << JSONRead();

        definition.DefineProperty("UBlobAccessor",
                                  static_cast<const std::unique_ptr<Blob>&(Foo::*)() const>(&Foo::GetUBlobAccessor), 
                                  static_cast<std::unique_ptr<Blob>&(Foo::*)()>(&Foo::GetUBlobAccessor)) << JSONRead();


        //definition.DefineProperty("Toxic", &Blob::blob_);   // Declaring Blob stuffs inside Foo

    }
    
};

template <>
struct syntropy::reflection::ClassDeclaration<FooBar> {

public:

    static constexpr const char* GetName() noexcept {

        return "FooBar";

    }

    template <typename TDefinition>
    void operator()(TDefinition& definition) const {

        definition.DefineBaseClass<Foo>();

    }
    
};

class Tester {

public:
    void TypeTest() const {

#define OUTPUT_TYPE(type) PrintType<type>(#type)

        OUTPUT_TYPE(signed char);
        OUTPUT_TYPE(unsigned char);
        OUTPUT_TYPE(char);

        std::cout << "\n";

        OUTPUT_TYPE(short int);
        OUTPUT_TYPE(unsigned short int);
        OUTPUT_TYPE(int);
        OUTPUT_TYPE(unsigned int);
        OUTPUT_TYPE(long int);
        OUTPUT_TYPE(unsigned long int);
        OUTPUT_TYPE(long long int);
        OUTPUT_TYPE(unsigned long long int);
        
        std::cout << "\n";

        OUTPUT_TYPE(int8_t);
        OUTPUT_TYPE(int16_t);
        OUTPUT_TYPE(int32_t);
        OUTPUT_TYPE(int64_t);

        std::cout << "\n";

        OUTPUT_TYPE(int_fast8_t);
        OUTPUT_TYPE(int_fast16_t);
        OUTPUT_TYPE(int_fast32_t);
        OUTPUT_TYPE(int_fast64_t);

        std::cout << "\n";

        OUTPUT_TYPE(int_least8_t);
        OUTPUT_TYPE(int_least16_t);
        OUTPUT_TYPE(int_least32_t);
        OUTPUT_TYPE(int_least64_t);

        std::cout << "\n";

        OUTPUT_TYPE(uint8_t);
        OUTPUT_TYPE(uint16_t);
        OUTPUT_TYPE(uint32_t);
        OUTPUT_TYPE(uint64_t);

        std::cout << "\n";

        OUTPUT_TYPE(uint_fast8_t);
        OUTPUT_TYPE(uint_fast16_t);
        OUTPUT_TYPE(uint_fast32_t);
        OUTPUT_TYPE(uint_fast64_t);

        std::cout << "\n";

        OUTPUT_TYPE(uint_least8_t);
        OUTPUT_TYPE(uint_least16_t);
        OUTPUT_TYPE(uint_least32_t);
        OUTPUT_TYPE(uint_least64_t);

        std::cout << "\n";

        OUTPUT_TYPE(intmax_t);
        OUTPUT_TYPE(intptr_t);
        OUTPUT_TYPE(uintmax_t);
        OUTPUT_TYPE(uintptr_t);

    }

    void SynopsisTest() const {

        std::cout << "Class '" << foo_class_ << "' is " << (foo_class_.IsAbstract() ? "" : "not ") << "abstract\n";

        for (const auto& property : foo_class_.GetProperties()) {

            std::cout << "Property " << property.GetName() << " : " << property.GetType() << "\n";

        }

    }

    void FieldTest() const {

        using syntropy::reflection::Readable;
        using syntropy::reflection::Writeable;

        Foo foo;
        Foo foo2;

        syntropy::reflection::Any anyfoo(&foo);

        {
            auto property = foo_class_.GetProperty("blob_value");

            auto& writer = *property->GetInterface<Writeable>();
            auto& reader = *property->GetInterface<Readable>();

            //auto FooAny = syntropy::reflection::MakeAny<Foo*>(&foo);

            // 1. Copy property values from one object to another (done)
            // 1.1 What about movable-only values?
            // 2. Drill down an object (aka take an object, enumerate properties recursively)
            // 3. Display property values. (StreamExtractable?)

            foo.blob_.blob_ = 10;
            foo2.blob_.blob_ = 20;

            //writer(foo2, reader(anyfoo));     // BUG! Foo* == const Foo* yields FALSE (instead of true)
            writer(foo2, reader(foo));          // Copy a property from "foo" to "foo2" via reflection.

            TEST_TRUE(foo.blob_.blob_ == foo2.blob_.blob_);

        }

        {
            auto property = foo_class_.GetProperty("float_value");

            auto& writer = *property->GetInterface<Writeable>();

            foo.value_ = 10.0f;

            writer(anyfoo, 5.0f);

            TEST_TRUE(foo.value_ == 5.0f);
        }

        {
            auto property = foo_class_.GetProperty("const_value");

            auto writer = property->GetInterface<Writeable>();

            TEST_TRUE(writer == nullptr);
        }

    }

    void PropertyTest() const  {

        using syntropy::reflection::Readable;
        using syntropy::reflection::Writeable;

        Foo foo;
        Foo foo2;

        {
            auto property = foo_class_.GetProperty("Value");

            auto& writer = *property->GetInterface<Writeable>();
            auto& reader = *property->GetInterface<Readable>();

            foo.SetValue(666.0f);
            foo2.SetValue(800.0f);

            writer(foo2, reader(foo));  // Copy a property from "foo" to "foo2" via reflection.

            TEST_TRUE(foo.GetValue() == foo2.GetValue());

            writer(foo2, 100.0f);

            TEST_TRUE(foo2.GetValue() == 100.0f);

            float x = 200.0f;

            writer(foo2, x);

            TEST_TRUE(foo2.GetValue() == x);
        }

    }

    void MoveTest() const {

        //Foo foo;

        //auto up = std::make_unique<Blob>();

        //up->blob_ = 600;
        //TEST_FALSE(field_movable_->Set(foo, up));                   // up is lvalue reference, moving must be performed explicitly
        //TEST_TRUE(field_movable_->Set(foo, std::move(up)));         // calls move assignment
        //TEST_FALSE(field_movable_->Get(foo, up));                   // getters cannot modify the internal state of the object
        //TEST_TRUE(field_movable_->Move(foo, up));                   // modify foo state

        //up = std::make_unique<Blob>();
        //up->blob_ = 500;
        //TEST_FALSE(property_movable_->Set(foo, up));                // up is lvalue reference, moving must be performed explicitly
        //TEST_TRUE(property_movable_->Set(foo, std::move(up)));      // calls move assignment
        //TEST_FALSE(property_movable_->Get(foo, up));                // getters cannot modify the internal state of the object
        //TEST_FALSE(property_movable_->Move(foo, up));               // cant modify foo state since the property only provides const access to the field

        //up = std::make_unique<Blob>();
        //up->blob_ = 400;
        //TEST_FALSE(property_movable_accessor_->Set(foo, up));                   // up is lvalue reference, moving must be performed explicitly
        //TEST_TRUE(property_movable_accessor_->Set(foo, std::move(up)));         // calls move assignment
        //TEST_FALSE(property_movable_accessor_->Get(foo, up));                   // Getters cannot modify the internal state of the object
        //TEST_TRUE(property_movable_accessor_->Move(foo, up));                   // modify foo state

        //Blob bb;
        //bb.blob_ = 1000;
        //TEST_TRUE(property_pod_->Set(foo, std::move(bb)));          // calls move assignment

        //bb.blob_ = 500;
        //TEST_TRUE(property_pod_->Move(foo, bb));                    // may affect foo state, fallback to "get" if needed

    }

    void ConversionTest() const {

        Foo foo;
            

        //foo.boolean_ = false;

        //TEST_TRUE(field_float_value_->Set(foo, 512) &&                                // From int to float.
        //          foo.value_ == 512.0f);                        

        //TEST_TRUE(field_int_value_->Set(foo, 1024.5632f) &&                           // From float to int.
        //          foo.value2_ == 1024);

//         int int_val;
//
//         TEST_TRUE(field_float_value_->Set(foo, "256.25") &&
//                   foo.value_ == 256.25f);
// 
//         TEST_TRUE(field_int_value_->Set(foo, "47") &&
//                   foo.value2_ == 47);
// 
//         TEST_TRUE(property_value_->Set(foo, "125.50") &&
//                   foo.GetValue() == 125.50f);
// 
//         TEST_TRUE(property_accessor_->Set(foo, "64.00") &&
//                   foo.GetAccessor().blob_ == 64);
// 
//         TEST_TRUE(property_pod_->Set(foo, "16.50") &&
//                   foo.GetBlob().blob_ == 16);
// 
//         TEST_FALSE(field_float_value_->Set(foo, Blob{ 50 }));                         // Blob cannot be inward-interpreted
// 
//         TEST_TRUE(field_int_value_->Set(foo, StreamableBlob{ 800 }) &&                // Streamable blob can be inward-interpreted
//                   foo.value2_ == 800);
// 
//         TEST_FALSE(property_pointer_->Set(foo, "56.23f"));
//         
//         TEST_TRUE(field_boolean_->Set(foo, "1") &&
//                   foo.boolean_ == true);
// 
//         TEST_TRUE(field_boolean_->Set(foo, "0") &&
//                   foo.boolean_ == false);
// 
//         TEST_TRUE(field_boolean_->Set(foo, "false") &&                                // From string to boolean.
//                   foo.boolean_ == false);
// 
//         TEST_FALSE(field_boolean_->Set(foo, "whatever"));
// 
//         TEST_FALSE(field_float_value_->Set(foo, "false"));                            // Wrong types
// 
//         TEST_TRUE(field_float_value_->Get(foo, int_val) &&
//                   static_cast<int>(foo.value_) == int_val);                           // Outward interpreting
        
    }

    void PolymorphismTest() const {

        //FooBar foobar;
        //Bar bar;
        //        
        //float x = 0;
        //float* p = &x;
        //const float* q = &x;
        //const float y(10);
        //Blob bb;

        //// Applying to a derived class

        //TEST_TRUE(field_float_value_->Set(foobar, 40.2f));
        //TEST_TRUE(field_float_value_->Get(foobar, x));

        //TEST_FALSE(field_const_value_->Set(foobar, x));
        //TEST_TRUE(field_const_value_->Get(foobar, x));

        //TEST_TRUE(field_pointer_->Set(foobar, p));
        //TEST_TRUE(field_pointer_->Get(foobar, p));

        //TEST_TRUE(field_pointer_to_const_->Set(foobar, q));
        //TEST_TRUE(field_pointer_to_const_->Get(foobar, q));

        //TEST_FALSE(field_const_pointer_->Set(foobar, p));
        //TEST_TRUE(field_const_pointer_->Get(foobar, p));

        //TEST_TRUE(property_value_->Set(foobar, y));
        //TEST_TRUE(property_value_->Get(foobar, x));

        //TEST_FALSE(property_const_value_->Set(foobar, y));
        //TEST_TRUE(property_const_value_->Get(foobar, x));

        //TEST_TRUE(property_pointer_->Set(foobar, p));
        //TEST_TRUE(property_pointer_->Get(foobar, p));

        //TEST_TRUE(property_pointer_to_const_->Set(foobar, q));
        //TEST_TRUE(property_pointer_to_const_->Get(foobar, q));

        //TEST_FALSE(property_const_pointer_->Set(foobar, p));
        //TEST_TRUE(property_const_pointer_->Get(foobar, p));

        //TEST_TRUE(property_pod_->Set(foobar, bb));
        //TEST_TRUE(property_pod_->Get(foobar, bb));

        //TEST_TRUE(property_accessor_->Set(foobar, bb));
        //TEST_TRUE(property_accessor_->Get(foobar, bb));

        //// Applying to the base class

        //TEST_FALSE(field_float_value_->Set(bar, 40.2f));
        //TEST_FALSE(field_float_value_->Get(bar, x));

        //TEST_FALSE(field_const_value_->Get(bar, x));

        //TEST_FALSE(field_pointer_->Set(bar, p));
        //TEST_FALSE(field_pointer_->Get(bar, p));

        //TEST_FALSE(field_pointer_to_const_->Set(bar, q));
        //TEST_FALSE(field_pointer_to_const_->Get(bar, q));

        //TEST_FALSE(field_const_pointer_->Get(bar, p));

        //TEST_FALSE(property_value_->Set(bar, y));
        //TEST_FALSE(property_value_->Get(bar, x));

        //TEST_FALSE(property_const_value_->Get(bar, x));

        //TEST_FALSE(property_pointer_->Set(bar, p));
        //TEST_FALSE(property_pointer_->Get(bar, p));

        //TEST_FALSE(property_pointer_to_const_->Set(bar, q));
        //TEST_FALSE(property_pointer_to_const_->Get(bar, q));

        //TEST_FALSE(property_const_pointer_->Get(bar, p));

        //TEST_FALSE(property_pod_->Set(bar, bb));
        //TEST_FALSE(property_pod_->Get(bar, bb));

        //TEST_FALSE(property_accessor_->Set(bar, bb));
        //TEST_FALSE(property_accessor_->Get(bar, bb));

    }

    void InstancingTest() {

        using syntropy::reflection::Constructible;

        auto bar_ctor = bar_class_.GetInterface<Constructible<>>();
        auto foobar_ctor = foobar_class_.GetInterface<Constructible<>>();

        if (!bar_ctor || !foobar_ctor)
        {
            return;
        }

        auto bar = (*bar_ctor)();
        auto foobar = (*foobar_ctor)();

        //FooBar bee;
        //FooBar* beep = &bee;

        //auto foobarp = syntropy::reflection::MakeInstance(beep);

        TEST_TRUE(syntropy::reflection::AnyCast<Bar*>(&bar) != nullptr);
		TEST_FALSE(syntropy::reflection::AnyCast<Foo*>(&bar) != nullptr);
		TEST_FALSE(syntropy::reflection::AnyCast<FooBar*>(&bar) != nullptr);

    }

    void ForwardingTest() {

        //using syntropy::reflection::Constructible;

        //float x = 0;

        //FooBar foobar;

        //auto foobar_instance = syntropy::reflection::MakeInstance(foobar);
        //auto const_foobar_instance = syntropy::reflection::MakeConstInstance(foobar_instance);

        //TEST_TRUE(field_float_value_->Set(foobar_instance, 100.0f) && foobar.value_ == 100.0f);
        //TEST_FALSE(field_float_value_->Set(const_foobar_instance, 200.0f) || foobar.value_ == 200.0f);                  // Const instance
        ////TEST_TRUE(field_float_value_->Set((*foobar_class_.GetInterface<Constructible<>>())(), 300.0f));                 // Also, leak :D
        //TEST_FALSE(field_float_value_->Set(MakeConstInstance(foobar), 400.0f) || foobar.value_ == 400.0f);              // Const instance
        //TEST_TRUE(field_float_value_->Set(foobar, 500.0f) && foobar.value_ == 500.0f);
        ////TEST_TRUE(field_float_value_->Set(MakeFooBar(), 999.0f));                                                     // r-value reference

        //TEST_TRUE(field_float_value_->Get(foobar_instance, x));
        //TEST_TRUE(field_float_value_->Get(const_foobar_instance, x));
        ////TEST_TRUE(field_float_value_->Get((*foobar_class_.GetInterface<Constructible<>>())(), x));                      // Also, leak :D
        //TEST_TRUE(field_float_value_->Get(MakeConstInstance(foobar), x));
        //TEST_TRUE(field_float_value_->Get(foobar, x));
        //TEST_TRUE(field_float_value_->Get(MakeFooBar(), x));
        
    }

    void AssignTest() {

//         auto instance = syntropy::reflection::GetClass("int")->Instantiate();
// 
//         TEST_TRUE(instance.Assign(5) && *instance.As<int>() == 5);
//         TEST_TRUE(instance.Assign(47.0f) && *instance.As<int>() == 47);
//         
//         instance = syntropy::reflection::GetClass("Blob")->Instantiate();
// 
//         Blob blob;
// 
//         blob.blob_ = 9999;
// 
//         TEST_TRUE(instance.Assign(blob));
//         TEST_FALSE(instance.Assign(5));
// 
//         auto consti = syntropy::reflection::MakeConstInstance(instance);
// 
//         TEST_TRUE(consti.Assign(blob));

    }
    
    template <typename TType>
    void PrintType(const char* type_name) const{

        std::cout << std::setw(30) << type_name << ": ";

        auto& class_instance = syntropy::reflection::ClassOf<TType>();

        std::cout << class_instance.GetDefaultName();

        for (auto&& name_alias : class_instance.GetNameAliases()) {

            std::cout << ", " << name_alias;

        }

        std::cout << "\n";

    }

    void DeserializeTest() {

        Foo foo;

        foo.p_blob_ = nullptr;
        foo.u_blob_ = nullptr;
        foo.s_blob_ = nullptr;

        nlohmann::json json = R"({
                                    "int_value": 42,
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
                                              }  
                                 })"_json;

        syntropy::serialization::DeserializeObjectFromJSON(foo, json);

        TEST_TRUE(foo.value_ == 67.5f);
        TEST_TRUE(foo.value2_ == 42);
        TEST_TRUE(foo.boolean_ == true);
        TEST_TRUE(foo.string_ == "awesome!");
        TEST_TRUE(foo.wstring_ == L"wawesome?");
        TEST_TRUE(foo.GetBlob().blob_ == 47);
        TEST_FALSE(foo.const_value_ == 100.0f);
        //TEST_TRUE(dynamic_cast<DerivedBlob*>(foo.p_blob_) != nullptr);

    }

    void Do() {

        //RUN_TEST(TypeTest);

        RUN_TEST(SynopsisTest);

        std::cout << "\n\n";

        RUN_TEST(FieldTest);
        RUN_TEST(MoveTest);
        RUN_TEST(PropertyTest);
        RUN_TEST(ConversionTest);
        RUN_TEST(PolymorphismTest);
        RUN_TEST(InstancingTest);
        RUN_TEST(ForwardingTest);
        RUN_TEST(AssignTest);
        RUN_TEST(DeserializeTest);

    }

    Tester() 
        : foo_class_(syntropy::reflection::ClassOf<Foo>())
        , foobar_class_(syntropy::reflection::ClassOf<FooBar>())
        , bar_class_(syntropy::reflection::ClassOf<Bar>())
        , abstract_class_(syntropy::reflection::ClassOf<AbstractFoo>()){
    
        auto& derived_blob_class = syntropy::reflection::ClassOf<DerivedBlob>();

        SYNTROPY_UNUSED(derived_blob_class);

        field_int_value_ = foo_class_.GetProperty("int_value");
        field_float_value_ = foo_class_.GetProperty("float_value");
        field_const_value_ = foo_class_.GetProperty("const_value");
        field_pointer_ = foo_class_.GetProperty("pointer");
        field_pointer_to_const_ = foo_class_.GetProperty("pointer_to_const");
        field_const_pointer_ = foo_class_.GetProperty("const_pointer");
        field_boolean_ = foo_class_.GetProperty("boolean");
        field_movable_ = foo_class_.GetProperty("u_blob");

        assert(field_int_value_);
        assert(field_float_value_);
        assert(field_const_value_);
        assert(field_pointer_);
        assert(field_pointer_to_const_);
        assert(field_const_pointer_);
        assert(field_boolean_);
        assert(field_movable_);

        property_value_ = foo_class_.GetProperty("Value");
        property_const_value_ = foo_class_.GetProperty("ConstValue");
        property_pointer_ = foo_class_.GetProperty("Pointer");
        property_pointer_to_const_ = foo_class_.GetProperty("PointerToConst");
        property_const_pointer_ = foo_class_.GetProperty("ConstPointer");
        property_pod_ = foo_class_.GetProperty("Blob");
        property_accessor_ = foo_class_.GetProperty("Accessor");
        property_movable_ = foo_class_.GetProperty("UBlob");
        property_movable_accessor_ = foo_class_.GetProperty("UBlobAccessor");
        
        assert(property_value_);
        assert(property_const_value_);
        assert(property_pointer_);
        assert(property_pointer_to_const_);
        assert(property_const_pointer_);
        assert(property_pod_);
        assert(property_accessor_);
        assert(property_movable_);
        assert(property_movable_accessor_);

    }

private:

    const syntropy::reflection::Class& bar_class_;
    const syntropy::reflection::Class& foo_class_;
    const syntropy::reflection::Class& foobar_class_;
    const syntropy::reflection::Class& abstract_class_;

    const syntropy::reflection::Property* field_int_value_;
    const syntropy::reflection::Property* field_float_value_;
    const syntropy::reflection::Property* field_const_value_;
    const syntropy::reflection::Property* field_pointer_;
    const syntropy::reflection::Property* field_pointer_to_const_;
    const syntropy::reflection::Property* field_const_pointer_;
    const syntropy::reflection::Property* field_boolean_;
    const syntropy::reflection::Property* field_movable_;

    const syntropy::reflection::Property* property_value_;
    const syntropy::reflection::Property* property_const_value_;
    const syntropy::reflection::Property* property_pointer_;
    const syntropy::reflection::Property* property_pointer_to_const_;
    const syntropy::reflection::Property* property_const_pointer_;
    const syntropy::reflection::Property* property_pod_;
    const syntropy::reflection::Property* property_accessor_;
    const syntropy::reflection::Property* property_movable_;
    const syntropy::reflection::Property* property_movable_accessor_;

};
