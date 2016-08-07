
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
std::cout << ((test) ? "PASSED - " : "NOT PASSED - ") << #test << std::endl;

#define TEST_FALSE(test) \
std::cout << (!(test) ? "PASSED - " : "NOT PASSED - ") << #test << std::endl;


struct Blob{

    int blob_;

};

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
class syntropy::MetaClassDefinition<Bar> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("syntropy::Bar") {}


};

template <>
class syntropy::MetaClassDefinition<Foo> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("syntropy::Foo"){

        DefineBaseClass<Bar>(); 

        DefineProperty("float_value", &Foo::value_);
        DefineProperty("int_value", &Foo::value2_);
        DefineProperty("const_value", &Foo::const_value_);
        DefineProperty("pointer", &Foo::pointer_);
        DefineProperty("pointer_to_const", &Foo::pointer_to_const_);
        DefineProperty("const_pointer", &Foo::const_pointer_);

        DefineProperty("Value", &Foo::GetValue, &Foo::SetValue);
        DefineProperty("ConstValue", &Foo::GetConstValue);
        DefineProperty("Pointer", &Foo::GetPointer, &Foo::SetPointer);
        DefineProperty("PointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst);
        DefineProperty("ConstPointer", &Foo::GetConstPointer);      
        DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);
        DefineProperty("Accessor", &Foo::GetAccessor, &Foo::GetAccessor);

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

    void DataDrivenTest() const {

        Foo foo;

        syntropy::Any any(100.0f);      // Read from file

        TEST_TRUE(field_float_value_->Write(foo, any));         // Should test the underlying type of Any
        TEST_TRUE(field_float_value_->Read(foo, any));          // Should change the underlying type of Any

        std::cout << std::endl;

    }

    void ConversionTest() const {

        Foo foo;

        TEST_TRUE(field_float_value_->Write(foo, 100));         // Inward widening conversion from int to float.
        TEST_TRUE(field_int_value_->Write(foo, 100.5f));        // Inward narrowing conversion from float to int.

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
        DataDrivenTest();
        ConversionTest();
        
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

        assert(field_int_value_);
        assert(field_float_value_);
        assert(field_const_value_);
        assert(field_pointer_);
        assert(field_pointer_to_const_);
        assert(field_const_pointer_);
        
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
    
    const syntropy::MetaClassProperty* property_value_;
    const syntropy::MetaClassProperty* property_const_value_;
    const syntropy::MetaClassProperty* property_pointer_;
    const syntropy::MetaClassProperty* property_pointer_to_const_;
    const syntropy::MetaClassProperty* property_const_pointer_;
    const syntropy::MetaClassProperty* property_pod_;
    const syntropy::MetaClassProperty* property_accessor_;

};
