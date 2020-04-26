
/// \file reflection.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/unit_test/test_fixture.h"
#include "syntropy/unit_test/test_case.h"

#include "syntropy/reflection/reflection.h"
#include "syntropy/reflection/class.h"

#include "syntropy/containers/vector.h"
#include "syntropy/types/string.h"

/************************************************************************/
/* TEST SYNTROPY REFLECTION                                             */
/************************************************************************/

/// \brief Test suite used to test reflection functionalities within Syntropy.
class TestSyntropyReflection : public syntropy::TestFixture
{
public:

    /// \brief Base class for pets.
    struct Pet
    {
        Pet(size_t paws_count);

        virtual ~Pet() = default;

        syntropy::String name_;

        size_t age_;

        size_t GetPawsCount() const;

    private:

        size_t paws_count_;
    };

    /// \brief Interface for animals that can purr.
    struct IPurrable
    {
        virtual ~IPurrable() = default;

        virtual void Purr() = 0;
    };

    /// \brief A domestic cat.
    struct Cat : Pet, IPurrable
    {
        Cat();

        virtual ~Cat() = default;

        void Purr() override;

    private:

    };

    /// \brief A canary.
    struct Canary : Pet
    {
        Canary();
    };

    static syntropy::Vector<syntropy::TestCase> GetTestCases();

    TestSyntropyReflection();

    /// \brief Test class names.
    void TestClassNames();

    /// \brie Test class.
    void TestDynamicClass();

    /// \brief Test class attributes.
    void TestClassAttributes();

    /// \brief Test class inheritance.
    void TestClassInheritance();

    /// \brief Test class instancing.
    void TestClassInstancing();

    /// \brief Test class properties.
    void TestClassProperties();

    /// \brief Test class properties reads.
    void TestPropertyRead();

    /// \brief Test class properties writes.
    void TestPropertyWrite();

    /// \brief Test class properties move (outward).
    void TestPropertyMove();

private:

    const syntropy::reflection::Class* pet_class_;

    const syntropy::reflection::Class* purrable_interface_;

    const syntropy::reflection::Class* cat_class_;

    const syntropy::reflection::Class* canary_class_;
};
