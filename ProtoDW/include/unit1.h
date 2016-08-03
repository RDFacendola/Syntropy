
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

class FooBar {
public:
};

class Unrelated {
public:
};

class Foo : public Bar, public FooBar {

public:

    float meh;
    float* pmeh;

    void SetFoo(int foo) { SYN_UNUSED(foo); }

    int GetFoo() const { return 0;  }

    void DoFoo();

private:

    int a_;
    int b_;

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
class syntropy::MetaClassDefinition<FooBar> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("syntropy::FooBar") {}


};

template <>
class syntropy::MetaClassDefinition<Foo> : public syntropy::MetaClassDeclaration {

public:

    MetaClassDefinition() :
        MetaClassDeclaration("syntropy::Foo"){

        DefineBaseClass<Bar>();
        DefineBaseClass<FooBar>();       

        DefineProperty("meh", &Foo::meh);
        //DefineProperty("pmeh", &Foo::pmeh);
        //DefineProperty("Foo", &Foo::GetFoo, &Foo::SetFoo);
        //DefineProperty("FooRO", &Foo::GetFoo);

    }


};

class Tester {

public:

    void Do() {

		float val = 65;

		syntropy::Any first(val);
		syntropy::Any firstcopy(first);

		float value = *(firstcopy.As<float>());

		SYN_UNUSED(value);

        //Foo foo;

        //foo.meh = 500;

        //float value;

        //auto& foo_class = syntropy::MetaClass::GetClass<Foo>();

        //auto meh_property = foo_class.GetProperty("meh");

        ////meh_property->Write(foo, 47);
        //meh_property->Read(foo, value);

    }

};
