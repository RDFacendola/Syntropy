
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

private:

    float value_;
    const float const_value_;
    float* pointer_;
    const float * pointer_to_const_;
    float* const const_pointer_;

    Blob blob_;


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

        DefineProperty("value", &Foo::value_);
        DefineProperty("const_value", &Foo::const_value_);
        DefineProperty("pointer", &Foo::pointer_);
        DefineProperty("pointer_to_const", &Foo::pointer_to_const_);
        DefineProperty("const_pointer", &Foo::const_pointer_);

        DefineProperty("PValue", &Foo::GetValue, &Foo::SetValue);
        DefineProperty("PConstValue", &Foo::GetConstValue);
        DefineProperty("PPointer", &Foo::GetPointer, &Foo::SetPointer);
        DefineProperty("PPointerToConst", &Foo::GetPointerToConst, &Foo::SetPointerToConst);
        DefineProperty("PConstPointer", &Foo::GetConstPointer);

        DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);

    }


};



class Tester {

public:

    void FieldTest() {

        Foo foo;

        syntropy::MetaInstance meta_foo(foo);
        auto& meta_class = syntropy::MetaClass::GetClass<Foo>();

        auto value = meta_class.GetProperty("value");
        auto const_value = meta_class.GetProperty("const_value");
        auto pointer = meta_class.GetProperty("pointer");
        auto pointer_to_const = meta_class.GetProperty("pointer_to_const");
        auto const_pointer = meta_class.GetProperty("const_pointer");

        float x = 0;
        float* p = &x;
        const float* q = &x;

        TEST_TRUE(value->Write(meta_foo, 56.0f));
        TEST_TRUE(value->Read(meta_foo, x));

        TEST_FALSE(const_value->Write(meta_foo, 47.0f));
        TEST_TRUE(const_value->Read(meta_foo, x));

        TEST_TRUE(pointer->Write(meta_foo, p));
        TEST_TRUE(pointer->Read(meta_foo, p));

        TEST_TRUE(pointer_to_const->Write(meta_foo, q));
        TEST_TRUE(pointer_to_const->Read(meta_foo, q));

        TEST_FALSE(const_pointer->Write(meta_foo, p));
        TEST_TRUE(const_pointer->Read(meta_foo, p));

    }

    void PropertyTest() {

        Foo foo;

        syntropy::MetaInstance meta_foo(foo);
        auto& meta_class = syntropy::MetaClass::GetClass<Foo>();

        auto value = meta_class.GetProperty("PValue");
        auto const_value = meta_class.GetProperty("PConstValue");
        auto pointer = meta_class.GetProperty("PPointer");
        auto pointer_to_const = meta_class.GetProperty("PPointerToConst");
        auto const_pointer = meta_class.GetProperty("PConstPointer");

        auto blob = meta_class.GetProperty("Blob");

        Blob bb;
        float x = 100;
        float* p = &x;
        const float* q = &x;

        TEST_TRUE(value->Write(meta_foo, x));
        TEST_TRUE(value->Read(meta_foo, x));

        TEST_FALSE(const_value->Write(meta_foo, 47.0f));
        TEST_TRUE(const_value->Read(meta_foo, x));

        TEST_TRUE(pointer->Write(meta_foo, p));
        TEST_TRUE(pointer->Read(meta_foo, p));

        TEST_TRUE(pointer_to_const->Write(meta_foo, q));
        TEST_TRUE(pointer_to_const->Read(meta_foo, q));

        TEST_FALSE(const_pointer->Write(meta_foo, p));
        TEST_TRUE(const_pointer->Read(meta_foo, p));

        TEST_TRUE(blob->Write(meta_foo, Blob{ 47 }));
        TEST_TRUE(blob->Read(meta_foo, bb));

    }

    void Do() {

        FieldTest();
        PropertyTest();

    }

};
