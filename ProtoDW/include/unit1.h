
/// \file unit1.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy.h"

#include "reflection.h"

#include "unit2.h"

#include "any.h"

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

    int blob_;

};

struct StreamableBlob {

    int blob_;

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

    friend class syntropy::reflection::Class::Definition<Foo>;

public:

    Foo() noexcept
        : const_value_(666)
        , const_pointer_(nullptr){}

    Foo(const Foo& other)
        : value_(other.value_)
        , const_value_(other.const_value_)
        , pointer_(other.pointer_)
        , pointer_to_const_(other.pointer_to_const_)
        , const_pointer_(other.const_pointer_)
        , blob_(other.blob_) {

        std::cout << "Copy ctor!\n";

    }

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

public:

    float value_;
    int value2_;
    const float const_value_;
    float* pointer_;
    const float * pointer_to_const_;
    float* const const_pointer_;
    bool boolean_;

    Foo* fooptr_;

    Blob blob_;
    
};

class FooBar : public Foo {

public:

    FooBar() noexcept
        : Foo() {}

    FooBar(const FooBar& other)
        : Foo(other) {}

};

template <>
struct syntropy::reflection::ClassDeclaration<AbstractFoo> {

public:

    auto operator()() const {
    
        return syntropy::reflection::Class::Definition<AbstractFoo>("AbstractFoo");

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Blob> {

public:

    auto operator()() const {

        syntropy::reflection::Class::Definition<Blob> definition("Blob");

        definition.DefineProperty("blob", &Blob::blob_);

        return definition;

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Bar>  {

public:

    auto operator()() const {

        return syntropy::reflection::Class::Definition<Bar>("Bar");

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Foo> {

public:

    auto operator()() const{
        
        syntropy::reflection::Class::Definition<Foo> definition("Foo");

        definition.DefineBaseClass<Bar>();

        definition.DefineProperty("float_value", &Foo::value_);
        definition.DefineProperty("int_value", &Foo::value2_);
        definition.DefineProperty("const_value", &Foo::const_value_);
        definition.DefineProperty("pointer", &Foo::pointer_);
        definition.DefineProperty("pointer_to_const", &Foo::pointer_to_const_);
        definition.DefineProperty("const_pointer", &Foo::const_pointer_);
        definition.DefineProperty("boolean", &Foo::boolean_);
        
        definition.DefineProperty("Value", &Foo::GetValue, &Foo::SetValue);
        definition.DefineProperty("ConstValue", &Foo::GetConstValue);
        definition.DefineProperty("Pointer", &Foo::GetPointer, &Foo::SetPointer);
        definition.DefineProperty("PointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst);
        definition.DefineProperty("ConstPointer", &Foo::GetConstPointer);
        definition.DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);
       
        definition.DefineProperty("Accessor",
                                  static_cast<const Blob&(Foo::*)() const>(&Foo::GetAccessor), 
                                  static_cast<Blob&(Foo::*)()>(&Foo::GetAccessor));

        return definition;

    }
    
};

template <>
struct syntropy::reflection::ClassDeclaration<FooBar> {

public:

    auto operator()() const{

        syntropy::reflection::Class::Definition<FooBar> definition("FooBar");

        definition.DefineBaseClass<Foo>();

        return definition;

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

        std::cout << "Class '" << foo_class_ << "' is " << (foo_class_.IsInstantiable() ? "" : "not ") << "instantiable\n";

        for (const auto& property : foo_class_.GetProperties()) {

            std::cout << "Property " << property.GetName() << " : " << property.GetType() << "\n";

        }

    }

    void FieldTest() const {

        Foo foo;

        float x = 0;
        float* p = &x;
        const float* q = &x;

        TEST_TRUE(field_float_value_->Set(foo, 40.2f));
        TEST_TRUE(field_float_value_->Get(foo, x));

        TEST_FALSE(field_const_value_->Set(foo, x));
        TEST_TRUE(field_const_value_->Get(foo, x));

        TEST_TRUE(field_pointer_->Set(foo, p));
        TEST_TRUE(field_pointer_->Get(foo, p));

        TEST_TRUE(field_pointer_to_const_->Set(foo, q));
        TEST_TRUE(field_pointer_to_const_->Get(foo, q));

        TEST_FALSE(field_const_pointer_->Set(foo, p));
        TEST_TRUE(field_const_pointer_->Get(foo, p));

    }

    void PropertyTest() const  {

        Foo foo;

        Blob bb;

        float x = 100;
        float* p = &x;
        const float* q = &x;

        const float y(10);

        TEST_TRUE(property_value_->Set(foo, y));
        TEST_TRUE(property_value_->Get(foo, x));

        TEST_FALSE(property_const_value_->Set(foo, y));
        TEST_TRUE(property_const_value_->Get(foo, x));

        TEST_TRUE(property_pointer_->Set(foo, p));
        TEST_TRUE(property_pointer_->Get(foo, p));

        TEST_TRUE(property_pointer_to_const_->Set(foo, q));
        TEST_TRUE(property_pointer_to_const_->Get(foo, q));

        TEST_FALSE(property_const_pointer_->Set(foo, p));
        TEST_TRUE(property_const_pointer_->Get(foo, p));

        TEST_TRUE(property_pod_->Set(foo, bb));
        TEST_TRUE(property_pod_->Get(foo, bb));

        TEST_TRUE(property_accessor_->Set(foo, bb));
        TEST_TRUE(property_accessor_->Get(foo, bb));

    }

    void ConversionTest() const {

        Foo foo;
            

        foo.boolean_ = false;

        TEST_TRUE(field_float_value_->Set(foo, 512) &&                                // From int to float.
                  foo.value_ == 512.0f);                        

        TEST_TRUE(field_int_value_->Set(foo, 1024.5632f) &&                           // From float to int.
                  foo.value2_ == 1024);

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

        FooBar foobar;
        Bar bar;
                
        float x = 0;
        float* p = &x;
        const float* q = &x;
        const float y(10);
        Blob bb;

        // Applying to a derived class

        TEST_TRUE(field_float_value_->Set(foobar, 40.2f));
        TEST_TRUE(field_float_value_->Get(foobar, x));

        TEST_FALSE(field_const_value_->Set(foobar, x));
        TEST_TRUE(field_const_value_->Get(foobar, x));

        TEST_TRUE(field_pointer_->Set(foobar, p));
        TEST_TRUE(field_pointer_->Get(foobar, p));

        TEST_TRUE(field_pointer_to_const_->Set(foobar, q));
        TEST_TRUE(field_pointer_to_const_->Get(foobar, q));

        TEST_FALSE(field_const_pointer_->Set(foobar, p));
        TEST_TRUE(field_const_pointer_->Get(foobar, p));

        TEST_TRUE(property_value_->Set(foobar, y));
        TEST_TRUE(property_value_->Get(foobar, x));

        TEST_FALSE(property_const_value_->Set(foobar, y));
        TEST_TRUE(property_const_value_->Get(foobar, x));

        TEST_TRUE(property_pointer_->Set(foobar, p));
        TEST_TRUE(property_pointer_->Get(foobar, p));

        TEST_TRUE(property_pointer_to_const_->Set(foobar, q));
        TEST_TRUE(property_pointer_to_const_->Get(foobar, q));

        TEST_FALSE(property_const_pointer_->Set(foobar, p));
        TEST_TRUE(property_const_pointer_->Get(foobar, p));

        TEST_TRUE(property_pod_->Set(foobar, bb));
        TEST_TRUE(property_pod_->Get(foobar, bb));

        TEST_TRUE(property_accessor_->Set(foobar, bb));
        TEST_TRUE(property_accessor_->Get(foobar, bb));

        // Applying to the base class

        TEST_FALSE(field_float_value_->Set(bar, 40.2f));
        TEST_FALSE(field_float_value_->Get(bar, x));

        TEST_FALSE(field_const_value_->Get(bar, x));

        TEST_FALSE(field_pointer_->Set(bar, p));
        TEST_FALSE(field_pointer_->Get(bar, p));

        TEST_FALSE(field_pointer_to_const_->Set(bar, q));
        TEST_FALSE(field_pointer_to_const_->Get(bar, q));

        TEST_FALSE(field_const_pointer_->Get(bar, p));

        TEST_FALSE(property_value_->Set(bar, y));
        TEST_FALSE(property_value_->Get(bar, x));

        TEST_FALSE(property_const_value_->Get(bar, x));

        TEST_FALSE(property_pointer_->Set(bar, p));
        TEST_FALSE(property_pointer_->Get(bar, p));

        TEST_FALSE(property_pointer_to_const_->Set(bar, q));
        TEST_FALSE(property_pointer_to_const_->Get(bar, q));

        TEST_FALSE(property_const_pointer_->Get(bar, p));

        TEST_FALSE(property_pod_->Set(bar, bb));
        TEST_FALSE(property_pod_->Get(bar, bb));

        TEST_FALSE(property_accessor_->Set(bar, bb));
        TEST_FALSE(property_accessor_->Get(bar, bb));

    }

    void InstancingTest() {

        auto bar = bar_class_.Instantiate();
        auto foobar = foobar_class_.Instantiate();
        auto abstractfoo = abstract_class_.Instantiate();

        FooBar bee;
        FooBar* beep = &bee;

        auto foobarp = syntropy::reflection::MakeInstance(beep);

        TEST_TRUE(bar);
        TEST_TRUE(foobar);
        TEST_FALSE(abstractfoo);

        TEST_TRUE(bar.As<Bar>() != nullptr);
        TEST_FALSE(bar.As<Foo>() != nullptr);
        TEST_FALSE(bar.As<FooBar>() != nullptr);

        TEST_TRUE(foobar.As<Bar>() != nullptr);
        TEST_TRUE(foobar.As<Foo>() != nullptr);
        TEST_TRUE(foobar.As<FooBar>() != nullptr);

        TEST_TRUE(foobar.As<FooBar**>() == nullptr);
        TEST_TRUE(foobar.As<FooBar**>() == nullptr);
        //TEST_TRUE(foobar.As<FooBar[1][2]>() == nullptr);
                
        TEST_TRUE(foobarp.As<FooBar>() == nullptr);
        TEST_TRUE(foobarp.As<FooBar*>() != nullptr);
        TEST_TRUE(foobarp.As<FooBar**>() == nullptr);      

        float x = 0;
        float* p = &x;
        const float* q = &x;
        const float y(10);
        Blob bb;
        
        // Applying to a derived class

        TEST_TRUE(field_float_value_->Set(foobar, 40.2f));
        TEST_TRUE(field_float_value_->Get(foobar, x));

        TEST_FALSE(field_const_value_->Set(foobar, x));
        TEST_TRUE(field_const_value_->Get(foobar, x));

        TEST_TRUE(field_pointer_->Set(foobar, p));
        TEST_TRUE(field_pointer_->Get(foobar, p));

        TEST_TRUE(field_pointer_to_const_->Set(foobar, q));
        TEST_TRUE(field_pointer_to_const_->Get(foobar, q));

        TEST_FALSE(field_const_pointer_->Set(foobar, p));
        TEST_TRUE(field_const_pointer_->Get(foobar, p));

        TEST_TRUE(property_value_->Set(foobar, y));
        TEST_TRUE(property_value_->Get(foobar, x));

        TEST_FALSE(property_const_value_->Set(foobar, y));
        TEST_TRUE(property_const_value_->Get(foobar, x));

        TEST_TRUE(property_pointer_->Set(foobar, p));
        TEST_TRUE(property_pointer_->Get(foobar, p));

        TEST_TRUE(property_pointer_to_const_->Set(foobar, q));
        TEST_TRUE(property_pointer_to_const_->Get(foobar, q));

        TEST_FALSE(property_const_pointer_->Set(foobar, p));
        TEST_TRUE(property_const_pointer_->Get(foobar, p));

        TEST_TRUE(property_pod_->Set(foobar, bb));
        TEST_TRUE(property_pod_->Get(foobar, bb));

        TEST_TRUE(property_accessor_->Set(foobar, bb));
        TEST_TRUE(property_accessor_->Get(foobar, bb));

        // Applying to the base class

        TEST_FALSE(field_float_value_->Set(bar, 40.2f));
        TEST_FALSE(field_float_value_->Get(bar, x));

        TEST_FALSE(field_const_value_->Get(bar, x));

        TEST_FALSE(field_pointer_->Set(bar, p));
        TEST_FALSE(field_pointer_->Get(bar, p));

        TEST_FALSE(field_pointer_to_const_->Set(bar, q));
        TEST_FALSE(field_pointer_to_const_->Get(bar, q));

        TEST_FALSE(field_const_pointer_->Get(bar, p));

        TEST_FALSE(property_value_->Set(bar, y));
        TEST_FALSE(property_value_->Get(bar, x));

        TEST_FALSE(property_const_value_->Get(bar, x));

        TEST_FALSE(property_pointer_->Set(bar, p));
        TEST_FALSE(property_pointer_->Get(bar, p));

        TEST_FALSE(property_pointer_to_const_->Set(bar, q));
        TEST_FALSE(property_pointer_to_const_->Get(bar, q));

        TEST_FALSE(property_const_pointer_->Get(bar, p));

        TEST_FALSE(property_pod_->Set(bar, bb));
        TEST_FALSE(property_pod_->Get(bar, bb));

        TEST_FALSE(property_accessor_->Set(bar, bb));
        TEST_FALSE(property_accessor_->Get(bar, bb));

    }

    void ForwardingTest() {

        float x = 0;

        FooBar foobar;

        auto foobar_instance = syntropy::reflection::MakeInstance(foobar);
        auto const_foobar_instance = syntropy::reflection::MakeConstInstance(foobar_instance);

        TEST_TRUE(field_float_value_->Set(foobar_instance, 100.0f) && foobar.value_ == 100.0f);
        TEST_FALSE(field_float_value_->Set(const_foobar_instance, 200.0f) || foobar.value_ == 200.0f);                  // Const instance
        TEST_TRUE(field_float_value_->Set(foobar_class_.Instantiate(), 300.0f));                                        // Also, leak :D
        TEST_FALSE(field_float_value_->Set(MakeConstInstance(foobar), 400.0f) || foobar.value_ == 400.0f);              // Const instance
        TEST_TRUE(field_float_value_->Set(foobar, 500.0f) && foobar.value_ == 500.0f);
        //TEST_TRUE(field_float_value_->Set(MakeFooBar(), 999.0f));                                                     // r-value reference

        TEST_TRUE(field_float_value_->Get(foobar_instance, x));
        TEST_TRUE(field_float_value_->Get(const_foobar_instance, x));
        TEST_TRUE(field_float_value_->Get(foobar_class_.Instantiate(), x));                                             // Also, leak :D
        TEST_TRUE(field_float_value_->Get(MakeConstInstance(foobar), x));
        TEST_TRUE(field_float_value_->Get(foobar, x));
        TEST_TRUE(field_float_value_->Get(MakeFooBar(), x));

    }

    void AssignTest() {

        auto instance = syntropy::reflection::GetClass("int")->Instantiate();

        TEST_TRUE(instance.Assign(5));
        
        instance = syntropy::reflection::GetClass("Blob")->Instantiate();

        Blob blob;

        blob.blob_ = 9999;

        TEST_TRUE(instance.Assign(blob));
        TEST_FALSE(instance.Assign(5));

        auto consti = syntropy::reflection::MakeConstInstance(instance);

        TEST_TRUE(consti.Assign(blob));

    }
    
    template <typename TType>
    void PrintType(const char* type_name) const{

        std::cout << std::setw(30) << type_name << ": ";

        for (auto&& name_alias : syntropy::reflection::ClassOf<TType>().GetNames()) {

            std::cout << name_alias << ", ";

        }

        std::cout << "\n";

    }

    FooBar MakeFooBar() {

        return FooBar();

    }

    syntropy::reflection::Instance MakeConstInstance(const FooBar& foobar) {

        return syntropy::reflection::MakeConstInstance(foobar);

    }

    void Do() {

        //RUN_TEST(TypeTest);

        RUN_TEST(SynopsisTest);

        std::cout << "\n\n";

        RUN_TEST(FieldTest);
        RUN_TEST(PropertyTest);
        RUN_TEST(ConversionTest);
        RUN_TEST(PolymorphismTest);
        RUN_TEST(InstancingTest);
        RUN_TEST(ForwardingTest);
        RUN_TEST(AssignTest);

    }

    Tester() 
        : foo_class_(syntropy::reflection::Class::GetClass<Foo>())
        , foobar_class_(syntropy::reflection::Class::GetClass<FooBar>())
        , bar_class_(syntropy::reflection::Class::GetClass<Bar>())
        , abstract_class_(syntropy::reflection::Class::GetClass<AbstractFoo>()){
    
        field_int_value_ = foo_class_.GetProperty("int_value");
        field_float_value_ = foo_class_.GetProperty("float_value");
        field_const_value_ = foo_class_.GetProperty("const_value");
        field_pointer_ = foo_class_.GetProperty("pointer");
        field_pointer_to_const_ = foo_class_.GetProperty("pointer_to_const");
        field_const_pointer_ = foo_class_.GetProperty("const_pointer");
        field_boolean_ = foo_class_.GetProperty("boolean");

        assert(field_int_value_);
        assert(field_float_value_);
        assert(field_const_value_);
        assert(field_pointer_);
        assert(field_pointer_to_const_);
        assert(field_const_pointer_);
        assert(field_boolean_);

        property_value_ = foo_class_.GetProperty("Value");
        property_const_value_ = foo_class_.GetProperty("ConstValue");
        property_pointer_ = foo_class_.GetProperty("Pointer");
        property_pointer_to_const_ = foo_class_.GetProperty("PointerToConst");
        property_const_pointer_ = foo_class_.GetProperty("ConstPointer");
        property_pod_ = foo_class_.GetProperty("Blob");
        property_accessor_ = foo_class_.GetProperty("Accessor");
        
        assert(property_value_);
        assert(property_const_value_);
        assert(property_pointer_);
        assert(property_pointer_to_const_);
        assert(property_const_pointer_);
        assert(property_pod_);
        assert(property_accessor_);

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

    const syntropy::reflection::Property* property_value_;
    const syntropy::reflection::Property* property_const_value_;
    const syntropy::reflection::Property* property_pointer_;
    const syntropy::reflection::Property* property_pointer_to_const_;
    const syntropy::reflection::Property* property_const_pointer_;
    const syntropy::reflection::Property* property_pod_;
    const syntropy::reflection::Property* property_accessor_;

};
