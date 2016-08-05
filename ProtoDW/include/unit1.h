
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

	const Blob& GetBlob() const {

		return blob_;

	}

	void SetBlob(const Blob& blob) {

		blob_ = blob;

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
class syntropy::MetaClassDefinition<int> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("int") {}


};

template <>
class syntropy::MetaClassDefinition<float> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("float") {}


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

		DefineProperty("Blob", &Foo::GetBlob, &Foo::SetBlob);

    }


};



class Tester {

public:

    void FieldTest() {

        Foo foo;

        syntropy::MetaInstance meta_foo(foo);

        auto value = syntropy::MetaClass::GetClass<Foo>().GetProperty("value");
        auto const_value = syntropy::MetaClass::GetClass<Foo>().GetProperty("const_value");
        auto pointer = syntropy::MetaClass::GetClass<Foo>().GetProperty("pointer");
        auto pointer_to_const = syntropy::MetaClass::GetClass<Foo>().GetProperty("pointer_to_const");
        auto const_pointer = syntropy::MetaClass::GetClass<Foo>().GetProperty("const_pointer");

        float x = 100;
        float* p = &x;
        const float* q = &x;

        auto b = value->Write(meta_foo, x);
        auto a = value->Read(meta_foo, x);

        auto d = const_value->Write(meta_foo, x);    // Do nothing
        auto c = const_value->Read(meta_foo, x);

        auto f = pointer->Write(meta_foo, p);
        auto e = pointer->Read(meta_foo, p);

        auto h = pointer_to_const->Write(meta_foo, q);
        auto g = pointer_to_const->Read(meta_foo, q);

        auto j = const_pointer->Write(meta_foo, p);  // Do nothing
        auto i = const_pointer->Read(meta_foo, p);

        assert(a && b && c && !d && e && f && g && h && i && !j);

    }

    void PropertyTest() {

        Foo foo;

        syntropy::MetaInstance meta_foo(foo);

		auto value = syntropy::MetaClass::GetClass<Foo>().GetProperty("PValue");
        auto const_value = syntropy::MetaClass::GetClass<Foo>().GetProperty("PConstValue");
		auto pointer = syntropy::MetaClass::GetClass<Foo>().GetProperty("PPointer");

		auto blob = syntropy::MetaClass::GetClass<Foo>().GetProperty("Blob");

        float x = 100;
        float* p = &x;
        //const float* q = &x;

		auto a = value->Write(meta_foo, x);
		auto b = value->Read(meta_foo, x);

        auto d = const_value->Write(meta_foo, x);    // Do nothing
        auto c = const_value->Read(meta_foo, x);

		auto e = pointer->Write(meta_foo, p);
		auto f = pointer->Read(meta_foo, p);

		Blob bb;

		bb.blob_ = 55;

		auto z = blob->Write(meta_foo, bb);
		auto w = blob->Read(meta_foo, bb);

        assert(a && b && !d && c && e && f && z && w);

    }

    void Do() {

        FieldTest();
        PropertyTest();

    }

};
