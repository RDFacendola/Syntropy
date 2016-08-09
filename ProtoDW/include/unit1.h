
/// \file unit1.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy.h"
#include "meta_class.h"

#include "unit2.h"

#include "any.h"

#include <tuple>
#include <iostream>

#define TEST_TRUE(test) \
std::cout << ((test) ? "PASSED - " : "NOT PASSED - ")<< #test << " is true" << std::endl;

#define TEST_FALSE(test) \
std::cout << (!(test) ? "PASSED - " : "NOT PASSED - ") << #test << " is false" << std::endl;


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

    friend class syntropy::MetaClassDefinition<Foo>;

public:

    Foo()
        : const_value_(666)
        , const_pointer_(nullptr){}

    Foo(const Foo& other)
        : value_(other.value_)
        , const_value_(other.const_value_)
        , pointer_(other.pointer_)
        , pointer_to_const_(other.pointer_to_const_)
        , const_pointer_(other.const_pointer_)
        , blob_(other.blob_) {

        std::cout << "Copy ctor!" << std::endl;

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

class FooDerived : public Foo {

public:

    FooDerived()
        : Foo() {}

    FooDerived(const FooDerived& other)
        : Foo(other) {}

};

template <>
class syntropy::MetaClassDefinition<Bar>  {

public:

    syntropy::MetaClassDeclaration<Bar> operator()() const {

        return syntropy::MetaClassDeclaration<Bar>("syntropy::Bar");

    }


};

template <>
class syntropy::MetaClassDefinition<Foo> {

public:

    syntropy::MetaClassDeclaration<Foo> operator()() const{

        auto meta_class = syntropy::MetaClassDeclaration<Foo>("syntropy::Foo");

        meta_class.DefineBaseClass<Bar>();

        meta_class.DefineProperty("float_value", &Foo::value_);
        meta_class.DefineProperty("int_value", &Foo::value2_);
        meta_class.DefineProperty("const_value", &Foo::const_value_);
        meta_class.DefineProperty("pointer", &Foo::pointer_);
        meta_class.DefineProperty("pointer_to_const", &Foo::pointer_to_const_);
        meta_class.DefineProperty("const_pointer", &Foo::const_pointer_);
        meta_class.DefineProperty("boolean", &Foo::boolean_);
        
        meta_class.DefineProperty("Value", &Foo::GetValue, &Foo::SetValue);
        meta_class.DefineProperty("ConstValue", &Foo::GetConstValue);
        meta_class.DefineProperty("Pointer", &Foo::GetPointer, &Foo::SetPointer);
        meta_class.DefineProperty("PointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst);
        meta_class.DefineProperty("ConstPointer", &Foo::GetConstPointer);
        meta_class.DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);
        meta_class.DefineProperty<Blob>("Accessor", &Foo::GetAccessor, &Foo::GetAccessor);

        return meta_class;

    }
    
};

class Tester {

public:

    void SynopsisTest() const {

        std::cout << "Class '" << foo_class_.GetName().GetString() << "'" << std::endl;

        for (const auto& property : foo_class_.GetProperties()) {

            std::cout << "Property '" << property.first.GetString() << "': " << property.second.GetType().name() << std::endl;

        }

        std::cout << std::endl;

    }

    void FieldTest() const {

        Foo foo;

        float x = 0;
        float* p = &x;
        const float* q = &x;

        TEST_TRUE(field_float_value_->Write(foo, 40.2f));
        TEST_TRUE(field_float_value_->Read(foo, x));

        TEST_FALSE(field_const_value_->Write(foo, x));
        TEST_TRUE(field_const_value_->Read(foo, x));

        TEST_TRUE(field_pointer_->Write(foo, p));
        TEST_TRUE(field_pointer_->Read(foo, p));

        TEST_TRUE(field_pointer_to_const_->Write(foo, q));
        TEST_TRUE(field_pointer_to_const_->Read(foo, q));

        TEST_FALSE(field_const_pointer_->Write(foo, p));
        TEST_TRUE(field_const_pointer_->Read(foo, p));

        std::cout << std::endl;

    }

    void PropertyTest() const  {

        Foo foo;

        Blob bb;

        float x = 100;
        float* p = &x;
        const float* q = &x;

        const float y(10);

        TEST_TRUE(property_value_->Write(foo, y));
        TEST_TRUE(property_value_->Read(foo, x));

        TEST_FALSE(property_const_value_->Write(foo, y));
        TEST_TRUE(property_const_value_->Read(foo, x));

        TEST_TRUE(property_pointer_->Write(foo, p));
        TEST_TRUE(property_pointer_->Read(foo, p));

        TEST_TRUE(property_pointer_to_const_->Write(foo, q));
        TEST_TRUE(property_pointer_to_const_->Read(foo, q));

        TEST_FALSE(property_const_pointer_->Write(foo, p));
        TEST_TRUE(property_const_pointer_->Read(foo, p));

        TEST_TRUE(property_pod_->Write(foo, bb));
        TEST_TRUE(property_pod_->Read(foo, bb));

        TEST_TRUE(property_accessor_->Write(foo, bb));
        TEST_TRUE(property_accessor_->Read(foo, bb));

        std::cout << std::endl;

    }

    void InterpretTest() const {

        Foo foo;
            
        int int_val;

        foo.boolean_ = false;

        TEST_TRUE(field_float_value_->Write(foo, "256.25") &&
                  foo.value_ == 256.25f);

        TEST_TRUE(field_int_value_->Write(foo, "47") &&
                  foo.value2_ == 47);

        TEST_TRUE(property_value_->Write(foo, "125.50") &&
                  foo.GetValue() == 125.50f);

        TEST_TRUE(property_accessor_->Write(foo, "64.00") &&
                  foo.GetAccessor().blob_ == 64);

        TEST_TRUE(property_pod_->Write(foo, "16.50") &&
                  foo.GetBlob().blob_ == 16);

        TEST_FALSE(field_float_value_->Write(foo, Blob{ 50 }));                         // Blob cannot be inward-interpreted

        TEST_TRUE(field_int_value_->Write(foo, StreamableBlob{ 800 }) &&                // Streamable blob can be inward-interpreted
                  foo.value2_ == 800);

        TEST_FALSE(property_pointer_->Write(foo, "56.23f"));
        
        TEST_TRUE(field_boolean_->Write(foo, "1") &&
                  foo.boolean_ == true);

        TEST_TRUE(field_boolean_->Write(foo, "0") &&
                  foo.boolean_ == false);

        TEST_TRUE(field_boolean_->Write(foo, "false", std::ios_base::boolalpha) &&      // From string to boolean.
                  foo.boolean_ == false);

        TEST_FALSE(field_boolean_->Write(foo, "whatever"));

        TEST_TRUE(field_float_value_->Write(foo, 512) &&                                // From int to float.
                  foo.value_ == 512.0f);                        

        TEST_TRUE(field_int_value_->Write(foo, 1024.5632f) &&                           // From float to int.
                  foo.value2_ == 1024);

        TEST_FALSE(field_float_value_->Write(foo, "false"));                            // Wrong types

        TEST_TRUE(field_float_value_->Read(foo, int_val) &&
                  static_cast<int>(foo.value_) == int_val);                             // Outward interpreting

        std::cout << std::endl;

    }

    void PolymorphismTest() const {

        FooDerived dfoo;

        TEST_TRUE(field_float_value_->Write(dfoo, 100.0f));     // dfoo derives from Foo.

        std::cout << std::endl;

    }

    void Do() {

        SynopsisTest();
        FieldTest();
        PropertyTest();
        InterpretTest();
        PolymorphismTest();
        
        system("pause");

    }

    Tester() 
        : foo_class_(syntropy::MetaClass::GetClass<Foo>()){
    
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

    const syntropy::MetaClass& foo_class_;

    const syntropy::MetaClassProperty* field_int_value_;
    const syntropy::MetaClassProperty* field_float_value_;
    const syntropy::MetaClassProperty* field_const_value_;
    const syntropy::MetaClassProperty* field_pointer_;
    const syntropy::MetaClassProperty* field_pointer_to_const_;
    const syntropy::MetaClassProperty* field_const_pointer_;
    const syntropy::MetaClassProperty* field_boolean_;

    const syntropy::MetaClassProperty* property_value_;
    const syntropy::MetaClassProperty* property_const_value_;
    const syntropy::MetaClassProperty* property_pointer_;
    const syntropy::MetaClassProperty* property_pointer_to_const_;
    const syntropy::MetaClassProperty* property_const_pointer_;
    const syntropy::MetaClassProperty* property_pod_;
    const syntropy::MetaClassProperty* property_accessor_;

};
