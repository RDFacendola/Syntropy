
/// \file reflection.h
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "unit_test/test_fixture.h"
#include "unit_test/test_case.h"

#include "reflection/reflection.h"
#include "reflection/class.h"

#include <vector>
#include <string>

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

        std::string name_;

        size_t age_;

        size_t GetPawsCount() const;

    private:

        size_t paws_count_;
    };

    /// \brief Interface for animals that can purr.
    struct IPurrable
    {
        virtual void Purr() = 0;
    };

    /// \brief A domestic cat.
    struct Cat : Pet, IPurrable
    {
        Cat();

        void Purr() override;

    private:

    };

    /// \brief A canary.
    struct Canary : Pet
    {
        Canary();
    };

    static std::vector<syntropy::TestCase> GetTestCases();

    TestSyntropyReflection();

    /// \brief Test class names.
    void TestClassNames();

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
