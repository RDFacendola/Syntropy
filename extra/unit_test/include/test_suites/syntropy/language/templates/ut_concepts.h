
/// \file ut_concepts.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* CONCEPTS TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Concepts test fixture.
    struct ConceptsTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_concepts
        = Syntropy::UnitTest::MakeAutoUnitTest<ConceptsTestFixture>(
            u8"concepts.templates.language.syntropy")

    .TestCase(u8"Empty TypeList are typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>, TypeList<>>), true);
    })

    .TestCase(u8"Non-empty TypeList are typelists",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<Int, Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<Int>, TypeList<Float>>), true);
    })

    .TestCase(u8"Types other than TypeList are not typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<Int>), false);
        SYNTROPY_UNIT_EQUAL((IsTypeList<Float, TypeList<>>), false);
    })

    .TestCase(u8"Types are equal to themselves",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Int, Int>), true);
    })

    .TestCase(u8"Types are not equal to other types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Float>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Float, Int>), false);
    })

    .TestCase(u8"Reference types are not equal to value-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immovable<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Mutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Movable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immovable<Int>, Int>), false);
    })

    .TestCase(u8"Value types are not equal to reference types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, Int>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, Int, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, Int, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, Int, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, Int, Immovable<Int>>), false);
    })

    .TestCase(u8"Types are convertible to themselves",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Int>), true);
    })

    .TestCase(u8"Types are not convertible to non-related types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Bool, Ptr<Bool>>), false);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Float, Float, Ptr<Bool>>), false);
    })

    .TestCase(u8"Value types are convertible to immutable reference types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immutable<Int>>), true);
    })

    .TestCase(u8"Value types are not convertible to mutable reference types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Mutable<Int>>), false);
    })

    .TestCase(u8"Value types are convertible to rvalue reference types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immovable<Int>>), true);
    })

    .TestCase(u8"Reference types are convertible to value types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Mutable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immutable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Movable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immovable<Int>, Int>), true);
    })

    .TestCase(u8"Mutable reference types are convertible to immutable reference types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Mutable<Int>, Immutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Movable<Int>, Immutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immovable<Int>, Immutable<Int>>), true);
    })

    .TestCase(u8"Immutable reference types are not convertible to mutable reference types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Immutable<Int>, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immovable<Int>, Movable<Int>>), false);
    })

    .TestCase(u8"Immutable reference types are immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmutable<Immutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsImmutable<Immutable<Int>, Immutable<Int>>), true);
    })

    .TestCase(u8"Reference types other than immutable reference types are not immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmutable<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmutable<Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmutable<Immovable<Int>>), false);
    })

    .TestCase(u8"Mutable reference types are mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>, Mutable<Int>>), true);
    })

    .TestCase(u8"Reference types other than mutable reference types are not mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMutable<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Immovable<Int>>), false);
    })

    .TestCase(u8"Movable reference types are movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>, Movable<Int>>), true);
    })

    .TestCase(u8"Reference types other than movable reference types are not movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMovable<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immovable<Int>>), false);
    })

    .TestCase(u8"Immovable reference types are immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmovable<Immovable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsImmovable<Immovable<Int>, Immovable<Int>>), true);
    })

    .TestCase(u8"Reference types other than immovable reference types are not immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmovable<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmovable<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmovable<Movable<Int>>), false);
    })

    .TestCase(u8"Bool types are boolean.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsBoolean<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Mutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Immutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Movable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Immovable<Bool>>), true);
    })

    .TestCase(u8"Non-bool types are not boolean.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsBoolean<Int>), false);
    })

    .TestCase(u8"Integral types are integral.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsIntegral<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Mutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Immutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Immovable<Int>>), true);

        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix8>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix16>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix32>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix64>), true);
    })

    .TestCase(u8"Non-integral types are not integral.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsIntegral<Bool>), false);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Float>), false);
    })

    .TestCase(u8"Float types are real.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReal<Float>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Mutable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Immutable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Movable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Immovable<Float>>), true);
    })

    .TestCase(u8"Non-float types are not real.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReal<Int>), false);
    })

    .TestCase(u8"Classes are derived from all their direct bases.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct BaseA{};
        struct BaseB{};
        struct Derived : BaseA, BaseB{};

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseB>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA, BaseB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Immutable<Derived>, BaseA>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Immutable<BaseB>>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Mutable<BaseA>, BaseB>), true);
    })

    .TestCase(u8"Classes are derived from all their indirect bases.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct BaseA{};
        struct BaseB{};
        struct IntermediateA : BaseA{};
        struct IntermediateB : BaseB{};
        struct Derived : IntermediateA, IntermediateB{};

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseB>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA, BaseB>), true);
    })

    .TestCase(u8"Classes are not derived from all other unrelated classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};
        struct Unrelated{};

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Base, Derived>), false);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Unrelated>), false);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Base, Unrelated>), false);
    })

    .TestCase(u8"Classes are base of all their direct derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct DerivedA : Base{};
        struct DerivedB : Base{};

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedB>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA, DerivedB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Immutable<Base>, DerivedA>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Immutable<DerivedB>>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Mutable<DerivedA>, DerivedB>), true);
    })

    .TestCase(u8"Classes are base of all their indirect derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Intermediate : Base{};
        struct DerivedA : Intermediate{};
        struct DerivedB : Intermediate{};

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedB>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA, DerivedB>), true);
    })

    .TestCase(u8"Classes are not base of other unrelated classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};
        struct Unrelated{};

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Unrelated>), false);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Derived, Unrelated>), false);
    })

    .TestCase(u8"Virtual classes are polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Polymorphic{ virtual ~Polymorphic() = default; };

        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Polymorphic>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Immutable<Polymorphic>>), true);
    })

    .TestCase(u8"Non-virtual classes are not polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};

        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Base>), false);
        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Derived>), false);
    })

    .TestCase(u8"Final classes are final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Final final{};

        SYNTROPY_UNIT_EQUAL((IsFinal<Final>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsFinal<Immutable<Final>>), true);
    })

    .TestCase(u8"Non-final classes are not final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonFinal {};

        SYNTROPY_UNIT_EQUAL((IsFinal<NonFinal>), false);
    })

    .TestCase(u8"Fundamental types are standard-layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Float>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immovable<Float>>), true);
    })

    .TestCase(u8"POD types are standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct POD { Int Foo; };

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<POD>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<POD>>), true);
    })

    .TestCase(u8"Polymorphic types are not standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Polymorphic{ virtual ~Polymorphic() = default; };

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Polymorphic>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<Polymorphic>>), false);
    })

    .TestCase(u8"Types with different access control are not standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct AccessControl{ public: Int foo_; private: Int bar_; };

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<AccessControl>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<AccessControl>>), false);
    })

    .TestCase(u8"Polymorphic types are not standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Polymorphic{ virtual ~Polymorphic() = default; };

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Polymorphic>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<Polymorphic>>), false);
    })

    .TestCase(u8"Default constructible types are constructible from an empty list of arguments",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructible{};

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<DefaultConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Immutable<DefaultConstructible>>), true);
    })

    .TestCase(u8"Fundamental types are constructible from an empty list of arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Float>), true);
    })

    .TestCase(u8"Types are constructible from their constructor arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Constructible{ Constructible(Int, Float){} };

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Constructible, Int, Float>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Immutable<Constructible>, Int, Float>), true);
    })

    .TestCase(u8"Types are not constructible from arguments that match no constructor.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Constructible{ Constructible(Int, Float){} };

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Constructible, Float>), false);
        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Constructible, Float, Int, Int>), false);
    })

    .TestCase(u8"Default constructible types are default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructible{};

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<DefaultConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Immutable<DefaultConstructible>>), true);
    })

    .TestCase(u8"Non-default constructible types are not default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonDefaultConstructible{ NonDefaultConstructible(Int){} };

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<NonDefaultConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Immutable<NonDefaultConstructible>>), false);
    })

    .TestCase(u8"Fundamental types are default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Float>), true);
    })

    .TestCase(u8"Default constructible types are default initializable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructible{};

        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<DefaultConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Immutable<DefaultConstructible>>), true);
    })

    .TestCase(u8"Fundamental types are default initializable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Float>), true);
    })

    .TestCase(u8"Copy constructible types are copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyConstructible{ CopyConstructible(Immutable<CopyConstructible>){} };

        SYNTROPY_UNIT_EQUAL((IsCopyConstructible<CopyConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsCopyConstructible<Immutable<CopyConstructible>>), true);
    })

    .TestCase(u8"Non-copy constructible types are not copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonCopyConstructible{ NonCopyConstructible(Immutable<NonCopyConstructible>) = delete; };

        SYNTROPY_UNIT_EQUAL((IsCopyConstructible<NonCopyConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsCopyConstructible<Immutable<NonCopyConstructible>>), false);
    })

    .TestCase(u8"Move constructible types are move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveConstructible{ MoveConstructible(Movable<MoveConstructible>){} };

        SYNTROPY_UNIT_EQUAL((IsMoveConstructible<MoveConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsMoveConstructible<Immutable<MoveConstructible>>), true);
    })

    .TestCase(u8"Non-move constructible types are not move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonMoveConstructible{ NonMoveConstructible(Immutable<NonMoveConstructible>) = delete; };

        SYNTROPY_UNIT_EQUAL((IsMoveConstructible<NonMoveConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsMoveConstructible<Immutable<NonMoveConstructible>>), false);
    })

    .TestCase(u8"Copy-assignable types are copy-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsCopyAssignable<CopyAssignable>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsCopyAssignable<Immutable<CopyAssignable>>), true);
    })

    .TestCase(u8"Non-copy-assignable types are not copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonCopyAssignable
        {
            Mutable<NonCopyAssignable>
            operator=(Immutable<NonCopyAssignable>) = delete;
        };

        SYNTROPY_UNIT_EQUAL((IsCopyAssignable<NonCopyAssignable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsCopyAssignable<Immutable<NonCopyAssignable>>), false);
    })

    .TestCase(u8"Move-assignable types are move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsMoveAssignable<MoveAssignable>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsMoveAssignable<Immutable<MoveAssignable>>), true);
    })

    .TestCase(u8"Non-move-assignable types are not move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonMoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<NonMoveAssignable>
            operator=(NonMoveAssignable&&) = delete;
        };

        SYNTROPY_UNIT_EQUAL((IsMoveAssignable<NonMoveAssignable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsMoveAssignable<Immutable<NonMoveAssignable>>), false);
    })

    .TestCase(u8"Copy-assignable types can be assignable from themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<CopyAssignable, CopyAssignable>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<CopyAssignable, Immutable<CopyAssignable>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<CopyAssignable, Mutable<CopyAssignable>>), true);
    })

    .TestCase(u8"Move-assignable types can be assigned from themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<MoveAssignable, MoveAssignable>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<MoveAssignable, Movable<MoveAssignable>>), true);
    })

    .TestCase(u8"Types cannot be assigned from unrelated types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct SomeType{};
        struct UnrelatedType{};

        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, UnrelatedType>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, Immutable<UnrelatedType>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, Movable<UnrelatedType>>), false);
    })

    .TestCase(u8"Types can be assigned from types that are implicitly convertible to them.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Convertible{};
        struct SomeType{ SomeType(Immutable<Convertible>){} };

        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, Convertible>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, Immutable<Convertible>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<SomeType, Movable<Convertible>>), true);
    })

    .TestCase(u8"Immutable reference types are not assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<Immutable<CopyAssignable>, CopyAssignable>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<Immutable<CopyAssignable>, Immutable<CopyAssignable>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableFrom<Immutable<CopyAssignable>, Mutable<CopyAssignable>>), false);
    })

    .TestCase(u8"Copy-assignable types can be assigned to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<CopyAssignable, CopyAssignable>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<CopyAssignable>, CopyAssignable>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<CopyAssignable>, CopyAssignable>), true);

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<CopyAssignable, Mutable<CopyAssignable>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<CopyAssignable>, Mutable<CopyAssignable>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<CopyAssignable>, Mutable<CopyAssignable>>), true);
    })

    .TestCase(u8"Move-assignable types can be assigned to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<MoveAssignable, MoveAssignable>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Movable<MoveAssignable>, MoveAssignable>), true);

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<MoveAssignable, Mutable<MoveAssignable>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Movable<MoveAssignable>, Mutable<MoveAssignable>>), true);
    })

    .TestCase(u8"Types cannot be assigned from unrelated types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct SomeType{};
        struct UnrelatedType{};

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<UnrelatedType, SomeType>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<UnrelatedType>, SomeType>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<UnrelatedType>, SomeType>), false);

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<UnrelatedType, Mutable<SomeType>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<UnrelatedType>, Mutable<SomeType>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<UnrelatedType>, Mutable<SomeType>>), false);
    })

    .TestCase(u8"Types can be assigned from types that are implicitly convertible to them.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Convertible{};
        struct SomeType{ SomeType(Immutable<Convertible>){} };

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Convertible, SomeType>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<Convertible>, SomeType>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<Convertible>, SomeType>), true);

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Convertible, Mutable<SomeType>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<Convertible>, Mutable<SomeType>>), true);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<Convertible>, Mutable<SomeType>>), true);
    })

    .TestCase(u8"Nothing can be assigned to immutable reference types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        SYNTROPY_UNIT_EQUAL((IsAssignableTo<CopyAssignable, Immutable<CopyAssignable>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Immutable<CopyAssignable>, Immutable<CopyAssignable>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignableTo<Mutable<CopyAssignable>, Immutable<CopyAssignable>>), false);
    })

    .TestCase(u8"Types with public destructor are destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Destructible
        {
            ~Destructible() = default;
        };

        SYNTROPY_UNIT_EQUAL((IsDestructible<Destructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Immutable<Destructible>>), true);
    })

    .TestCase(u8"Fundamental types are destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsDestructible<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDestructible<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDestructible<Float>), true);
    })

    .TestCase(u8"Types with private destructor are not destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonDestructible
        {
        private:
            ~NonDestructible() = default;
        };

        SYNTROPY_UNIT_EQUAL((IsDestructible<NonDestructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Immutable<NonDestructible>>), false);
    })

    // #TODO Add trivial concepts and subsequent test cases.

    .TestCase(u8"",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;
    })

    ;

}

// ===========================================================================
