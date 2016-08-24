
/// \file unit1.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy.h"

#include "class.h"

#include "unit2.h"

#include "any.h"

#include <tuple>
#include <iostream>

#define TEST_TRUE(test) \
std::cout << ((test) ? "PASSED - " : "NOT PASSED - ")<< #test << " is true\n";

#define TEST_FALSE(test) \
std::cout << (!(test) ? "PASSED - " : "NOT PASSED - ") << #test << " is false\n";

#define RUN_TEST(test) \
std::cout << "Running test " << #test << "()\n\n"; \
test(); \
std::cout << "\n";

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

class Foo : public Bar {

    friend class syntropy::reflection::ClassDefinition<Foo>;

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
struct syntropy::reflection::ClassDeclaration<Blob> {

public:

    std::unique_ptr<syntropy::reflection::ClassDefinition<Blob>> operator()() const {

        auto c = std::make_unique<syntropy::reflection::ClassDefinition<Blob>>("Blob");

        c->DefineProperty("blob", &Blob::blob_);

        return c;

    }

};

template <>
struct syntropy::reflection::ClassDeclaration<Bar>  {

public:

    std::unique_ptr<syntropy::reflection::ClassDefinition<Bar>> operator()() const {

        return std::make_unique<syntropy::reflection::ClassDefinition<Bar>>("Bar");

    }
    
};

template <>
struct syntropy::reflection::ClassDeclaration<Foo> {

public:

    std::unique_ptr<syntropy::reflection::ClassDefinition<Foo>> operator()() const{

        auto c = std::make_unique<syntropy::reflection::ClassDefinition<Foo>>("Foo");

        c->DefineBaseClass<Bar>();

        c->DefineProperty("float_value", &Foo::value_);
        c->DefineProperty("int_value", &Foo::value2_);
        c->DefineProperty("const_value", &Foo::const_value_);
        c->DefineProperty("pointer", &Foo::pointer_);
        c->DefineProperty("pointer_to_const", &Foo::pointer_to_const_);
        c->DefineProperty("const_pointer", &Foo::const_pointer_);
        c->DefineProperty("boolean", &Foo::boolean_);
        
        c->DefineProperty("Value", &Foo::GetValue, &Foo::SetValue);
        c->DefineProperty("ConstValue", &Foo::GetConstValue);
        c->DefineProperty("Pointer", &Foo::GetPointer, &Foo::SetPointer);
        c->DefineProperty("PointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst);
        c->DefineProperty("ConstPointer", &Foo::GetConstPointer);
        c->DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);
       
        c->DefineProperty("Accessor", 
                          static_cast<const Blob&(Foo::*)() const>(&Foo::GetAccessor), 
                          static_cast<Blob&(Foo::*)()>(&Foo::GetAccessor));

        return c;

    }
    
};

template <>
struct syntropy::reflection::ClassDeclaration<FooBar> {

public:

    std::unique_ptr<syntropy::reflection::ClassDefinition<FooBar>> operator()() const{

        auto c = std::make_unique<syntropy::reflection::ClassDefinition<FooBar>>("FooBar");

        c->DefineBaseClass<Foo>();

        return c;

    }
    
};
class Tester {

public:

    void SynopsisTest() const {

        std::cout << "Class '" << foo_class_.GetName().GetString() 
                  << "' is " << (foo_class_.IsAbstract() ? "" : "not ") << "abstract\n";

        std::cout << "Class '" << foo_class_.GetName().GetString()
                  << "' is " << (foo_class_.GetFactory() ? "" : "not ") << "instantiable\n";

        for (const auto& property : foo_class_.GetProperties()) {

            std::cout << "Property " << property.second.GetName().GetString() << " : " << property.second.GetClass().GetName().GetString() << "\n";

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

    void InterpretTest() const {

        Foo foo;
            
        int int_val;

        foo.boolean_ = false;

        TEST_TRUE(field_float_value_->Set(foo, "256.25") &&
                  foo.value_ == 256.25f);

        TEST_TRUE(field_int_value_->Set(foo, "47") &&
                  foo.value2_ == 47);

        TEST_TRUE(property_value_->Set(foo, "125.50") &&
                  foo.GetValue() == 125.50f);

        TEST_TRUE(property_accessor_->Set(foo, "64.00") &&
                  foo.GetAccessor().blob_ == 64);

        TEST_TRUE(property_pod_->Set(foo, "16.50") &&
                  foo.GetBlob().blob_ == 16);

        TEST_FALSE(field_float_value_->Set(foo, Blob{ 50 }));                         // Blob cannot be inward-interpreted

        TEST_TRUE(field_int_value_->Set(foo, StreamableBlob{ 800 }) &&                // Streamable blob can be inward-interpreted
                  foo.value2_ == 800);

        TEST_FALSE(property_pointer_->Set(foo, "56.23f"));
        
        TEST_TRUE(field_boolean_->Set(foo, "1") &&
                  foo.boolean_ == true);

        TEST_TRUE(field_boolean_->Set(foo, "0") &&
                  foo.boolean_ == false);

        TEST_TRUE(field_boolean_->Set(foo, "false") &&                                // From string to boolean.
                  foo.boolean_ == false);

        TEST_FALSE(field_boolean_->Set(foo, "whatever"));

        TEST_TRUE(field_float_value_->Set(foo, 512) &&                                // From int to float.
                  foo.value_ == 512.0f);                        

        TEST_TRUE(field_int_value_->Set(foo, 1024.5632f) &&                           // From float to int.
                  foo.value2_ == 1024);

        TEST_FALSE(field_float_value_->Set(foo, "false"));                            // Wrong types

        TEST_TRUE(field_float_value_->Get(foo, int_val) &&
                  static_cast<int>(foo.value_) == int_val);                           // Outward interpreting
        
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
                
        auto bar = bar_class_.GetFactory()->Instantiate();
        auto foobar = foobar_class_.GetFactory()->Instantiate();

        std::cout << "Instancing 'bar' of type " << bar.GetClass().GetName().GetString() << "\n";
        std::cout << "Instancing 'foobar' of type " << foobar.GetClass().GetName().GetString() << "\n";

        TEST_TRUE(bar.As<Bar>() != nullptr);
        TEST_FALSE(bar.As<Foo>() != nullptr);
        TEST_FALSE(bar.As<FooBar>() != nullptr);

        TEST_TRUE(foobar.As<Bar>() != nullptr);
        TEST_TRUE(foobar.As<Foo>() != nullptr);
        TEST_TRUE(foobar.As<FooBar>() != nullptr);

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

    void Do() {

        RUN_TEST(SynopsisTest);
        RUN_TEST(FieldTest);
        RUN_TEST(PropertyTest);
        //RUN_TEST(InterpretTest);
        RUN_TEST(PolymorphismTest);
        RUN_TEST(InstancingTest);

    }

    Tester() 
        : foo_class_(syntropy::reflection::Class::GetClass<Foo>())
        , foobar_class_(syntropy::reflection::Class::GetClass<FooBar>())
        , bar_class_(syntropy::reflection::Class::GetClass<Bar>()){
    
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
