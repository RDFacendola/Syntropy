
/// \file concepts_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/foundation/ordering.h"

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
        // Type aliases.

        using Int = Syntropy::Int;
        using Float = Syntropy::Float;
        using Bool = Syntropy::Bool;
        using Ordering = Syntropy::Ordering;

        template <typename TType>
        using Immutable = Syntropy::Immutable<TType>;

        template <typename TType>
        using Mutable = Syntropy::Mutable<TType>;

        template <typename TType>
        using Immovable = Syntropy::Immovable<TType>;

        // Collapses with Mutable class types. Can't be used anyway due to
        // clang bug with Mutable<T>.
        // template <typename TType>
        // using Movable = Syntropy::Movable<TType>;

        /// \brief Simple base class.
        struct Base{};

        /// \brief Simple derived class.
        struct Intermediate : Base{};

        /// \brief Simple derived class.
        struct Derived : Intermediate{};

        /// \brief Generic class type.`
        struct Some{};

        /// \brief Unrelated class type.
        struct Unrelated{};

        /// \brief Type class that can be converted to Some type.
        struct Convertible
        {
            operator Some() const
            {
                return {};
            }
        };

        /// \brief Polymorphic class type.
        struct Polymorphic
        {
            virtual ~Polymorphic() = default;
        };

        /// \brief Final class type.
        struct Final final{};

        /// \brief Non-final class type.
        struct NonFinal{};

        /// \brief Default constructible class type.
        struct DefaultConstructible{};

        /// \brief Constructible class type.
        struct Constructible
        {
            Constructible(Int, Float)
            {

            }
        };

        /// \brief Movable class type.
        struct Movable
        {
            Movable(Movable&&)
            {

            }

            Mutable<Movable>
            operator=(Movable&&)
            {
                return *this;
            }
        };

        /// \brief Copyable and movable class type.
        struct Copyable
        {
            Copyable(Immutable<Copyable>)
            {

            }

            Mutable<Copyable>
            operator=(Immutable<Copyable>)
            {
                return *this;
            }
        };

        /// \brief Non-move-constructible class type.
        struct NonMoveConstructible
        {
            NonMoveConstructible(NonMoveConstructible&&) = delete;

            Mutable<NonMoveConstructible>
            operator=(NonMoveConstructible&&)
            {
                return *this;
            }
        };

        /// \brief Non-move-constructible class type.
        struct NonMoveAssignable
        {
            NonMoveAssignable(NonMoveAssignable&&)
            {

            }

            Mutable<NonMoveAssignable>
            operator=(NonMoveAssignable&&) = delete;
        };

        /// \brief Non-copy-constructible class type.
        struct NonCopyConstructible
        {
            NonCopyConstructible(Immovable<NonCopyConstructible>) = delete;

            Mutable<NonCopyConstructible>
            operator=(Immovable<NonCopyConstructible>)
            {
                return *this;
            }
        };

        /// \brief Non-copy-assignable class type.
        struct NonCopyAssignable
        {
            NonCopyAssignable(Immovable<NonCopyAssignable>)
            {

            }

            Mutable<NonCopyAssignable>
            operator=(Immovable<NonCopyAssignable>) = delete;
        };

        /// \brief Trivially copyable class type.
        struct TriviallyCopyable
        {
            TriviallyCopyable()
            {

            }

            ~TriviallyCopyable() = default;

            TriviallyCopyable(Immutable<TriviallyCopyable>) = default;

            TriviallyCopyable(TriviallyCopyable&&) = default;

            Mutable<TriviallyCopyable>
            operator=(Immutable<TriviallyCopyable>) = default;

            Mutable<TriviallyCopyable>
            operator=(TriviallyCopyable&&) = default;
        };

        /// \brief Trivial class type.
        struct Trivial
        {
            Trivial() = default;

            ~Trivial() = default;

            Trivial(Immutable<Trivial>) = default;

            Trivial(Trivial&&) = default;

            Mutable<Trivial>
            operator=(Immutable<Trivial>) = default;

            Mutable<Trivial>
            operator=(Trivial&&) = default;
        };

        /// \brief Equality-comparable class type.
        struct EqualityComparable
        {
            Bool operator==(Immutable<EqualityComparable>) const
            {
                return true;
            }
        };

        /// \brief Non-equality-comparable  class type.
        struct NonEqualityComparable
        {
            Bool operator==(Immutable<NonEqualityComparable>) = delete;
        };

        /// \brief Equality-comparable class type.
        struct Comparable
        {
            Bool operator==(Immutable<Comparable>) const
            {
                return true;
            }

            Ordering operator<=>(Immutable<Comparable>) const
            {
                return Ordering::kEquivalent;
            }
        };

        /// \brief Equality-comparable class type.
        struct NonComparable
        {
            Bool operator==(Immutable<Comparable>) const = delete;

            Ordering operator<=>(Immutable<Comparable>) const = delete;
        };

        /// \brief Generic class template.
        template <typename T>
        struct Template{};

        /// \brief Unrelated class template.
        template <typename T>
        struct UnrelatedTemplate{};
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& concepts_unit_test
        = Syntropy::UnitTest::MakeAutoUnitTest<ConceptsTestFixture>(
            u8"concepts.templates.language.syntropy")

    .TestCase(u8"Types are same-as themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, ReadOnly<Int>>), true);
    })

    .TestCase(u8"Types with different qualifiers are not the same.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<ReadWrite<Int>, ReadOnly<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, ReadWrite<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, Immovable<Int>>), false);
    })

    .TestCase(u8"Types are not same-as other types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Float>), false);
    })

    .TestCase(u8"Reference-types are not same-as their value-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immovable<Int>>), false);
    })

    .TestCase(u8"Value-types are not same-as their reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, Int>), false);
    })

    .TestCase(u8"Types are convertible to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int>), true);
    })

    .TestCase(u8"Types are not convertible to other non-related types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Bool, Ptr<Bool>>), false);
    })

    .TestCase(u8"Value-types are convertible to their respective immutable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immutable<Int>>), true);
    })

    .TestCase(u8"Value-types are not convertible to their respective mutable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Mutable<Int>>), false);
    })

    .TestCase(u8"Value-types are convertible to their respective movable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Movable<Int>>), true);
    })

    .TestCase(u8"Value-types are convertible to their respective immovable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immovable<Int>>), true);
    })

    .TestCase(u8"Reference-types are convertible to their respective "
              u8"value-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Mutable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immutable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Movable<Int>, Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Immovable<Int>, Int>), true);
    })

    .TestCase(u8"Reference-types are convertible to their respective "
              u8"immutable reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Mutable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Movable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Immutable<Int>>), true);
    })

    .TestCase(u8"Immutable reference-types are not convertible to their "
              u8"respective mutable reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Movable<Int>>), false);
    })

    .TestCase(u8"Value-types are not reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, Immovable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadOnly<Int>, Movable<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<ReadWrite<Int>, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadWrite<Int>, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadWrite<Int>, Immovable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<ReadWrite<Int>, Movable<Int>>), false);
    })

    .TestCase(u8"Reference-types are not value-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, ReadOnly<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immutable<Int>, ReadWrite<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, ReadOnly<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Mutable<Int>, ReadWrite<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, ReadOnly<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Immovable<Int>, ReadWrite<Int>>), false);

        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, ReadOnly<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Movable<Int>, ReadWrite<Int>>), false);
    })

    .TestCase(u8"Read-only value-types are read-only.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReadOnlyValue<ReadOnly<Int>>), true);
    })

    .TestCase(u8"Read-write value-types are not read-only.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReadOnlyValue<ReadWrite<Int>>), false);
    })

    .TestCase(u8"Read-write value-types are read-write.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReadWriteValue<ReadWrite<Int>>), true);
    })

    .TestCase(u8"Read-only value-types are not read-write.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsReadWriteValue<ReadOnly<Int>>), false);
    })

    .TestCase(u8"Immutable reference-types are immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmutableReference<Immutable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than immutable reference-types are not "
              u8"immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmutableReference<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmutableReference<Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmutableReference<Immovable<Int>>), false);
    })

    .TestCase(u8"Mutable reference-types are mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMutableReference<Mutable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than mutable reference-types are not "
              u8"mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMutableReference<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutableReference<Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutableReference<Immovable<Int>>), false);
    })

    .TestCase(u8"Movable reference-types are movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMovableReference<Movable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than movable reference-types are not "
              u8"movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsMovableReference<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovableReference<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovableReference<Immovable<Int>>), false);
    })

    .TestCase(u8"Immovable reference-types are immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmovableReference<Immovable<Int>>), true);
    })

    .TestCase(u8"Reference types other than immovable reference-types are not "
              u8"immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsImmovableReference<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmovableReference<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsImmovableReference<Movable<Int>>), false);
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

    .TestCase(u8"Classes are base of all their direct derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Base = ConceptsTestFixture::Base;
        using Derived = ConceptsTestFixture::Intermediate;

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, Derived>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Immutable<Base>, Derived>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, Immutable<Derived>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Immutable<Base>, Immutable<Derived>>), true);
    })

    .TestCase(u8"Classes are base of all their indirect derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Base = ConceptsTestFixture::Base;
        using Derived = ConceptsTestFixture::Derived;

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Derived>), true);
    })

    .TestCase(u8"Classes are not base of other unrelated classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Base = ConceptsTestFixture::Base;
        using Unrelated = ConceptsTestFixture::Unrelated;

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Unrelated>), false);
    })

    .TestCase(u8"Virtual classes are polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Polymorphic = ConceptsTestFixture::Polymorphic;

        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Polymorphic>), true);
        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Immutable<Polymorphic>>), true);
    })

    .TestCase(u8"Non-virtual classes are not polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Base = ConceptsTestFixture::Base;
        using Derived = ConceptsTestFixture::Derived;

        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Base>), false);
        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Derived>), false);
        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Immutable<Base>>), false);
        SYNTROPY_UNIT_EQUAL((IsPolymorphic<Immutable<Derived>>), false);
    })

    .TestCase(u8"Final classes are final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Final = ConceptsTestFixture::Final;

        SYNTROPY_UNIT_EQUAL((IsFinal<Final>), true);
        SYNTROPY_UNIT_EQUAL((IsFinal<Immutable<Final>>), true);
    })

    .TestCase(u8"Non-final classes are not final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonFinal = ConceptsTestFixture::NonFinal;

        SYNTROPY_UNIT_EQUAL((IsFinal<NonFinal>), false);
        SYNTROPY_UNIT_EQUAL((IsFinal<Immutable<NonFinal>>), false);
    })

    .TestCase(u8"Fundamental types are constructible from an empty list of "
              u8"arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConstructible<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructible<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructible<Float>), true);
    })

    .TestCase(u8"Default constructible types are constructible from an empty "
              u8"list of arguments",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using DefaultConstructible = ConceptsTestFixture::DefaultConstructible;

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<DefaultConstructible>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<Immutable<DefaultConstructible>>), true);
    })

    .TestCase(u8"Types are constructible from their constructor arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Constructible = ConceptsTestFixture::Constructible;

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<Constructible, Int, Float>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<Immutable<Constructible>, Int, Float>), true);
    })

    .TestCase(u8"Types are not constructible from arguments that match no "
              u8"constructor.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Constructible = ConceptsTestFixture::Constructible;

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<Constructible, Float>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsConstructible<Constructible, Float, Int, Int>), false);
    })

    .TestCase(u8"Movable types are move-constructible and move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Movable = ConceptsTestFixture::Movable;

        SYNTROPY_UNIT_EQUAL((IsMovable<Movable>), true);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immutable<Movable>>), true);
    })

    .TestCase(u8"Non-move-constructible types are not movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonMoveConstructible = ConceptsTestFixture::NonMoveConstructible;

        SYNTROPY_UNIT_EQUAL(
            (IsMovable<NonMoveConstructible>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsMovable<Immutable<NonMoveConstructible>>), false);
    })

    .TestCase(u8"Non-move-assignable types are not movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonMoveAssignable = ConceptsTestFixture::NonMoveAssignable;

        SYNTROPY_UNIT_EQUAL((IsMovable<NonMoveAssignable>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immutable<NonMoveAssignable>>), false);
    })

    .TestCase(u8"Copyable types are move-constructible and move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Copyable = ConceptsTestFixture::Copyable;

        SYNTROPY_UNIT_EQUAL((IsMovable<Copyable>), true);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immutable<Copyable>>), true);
    })

    .TestCase(u8"Copyable types are copy-constructible and copy-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Copyable = ConceptsTestFixture::Copyable;

        SYNTROPY_UNIT_EQUAL((IsCopyable<Copyable>), true);
        SYNTROPY_UNIT_EQUAL((IsCopyable<Immutable<Copyable>>), true);
    })

    .TestCase(u8"Non-copy-constructible types are not copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonCopyConstructible = ConceptsTestFixture::NonCopyConstructible;

        SYNTROPY_UNIT_EQUAL(
            (IsCopyable<NonCopyConstructible>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsCopyable<Immutable<NonCopyConstructible>>), false);
    })

    .TestCase(u8"Non-copy-assignable types are not copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonCopyAssignable = ConceptsTestFixture::NonCopyAssignable;

        SYNTROPY_UNIT_EQUAL((IsCopyable<NonCopyAssignable>), false);
        SYNTROPY_UNIT_EQUAL((IsCopyable<Immutable<NonCopyAssignable>>), false);
    })

    .TestCase(u8"Movable types can be move-assigned.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Movable = ConceptsTestFixture::Movable;

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Movable, Syntropy::Movable<Movable>>), true);
    })

    .TestCase(u8"Copyable types can be move-assigned.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Copyable = ConceptsTestFixture::Copyable;

        SYNTROPY_UNIT_EQUAL((IsAssignable<Copyable, Movable<Copyable>>), true);
    })

    .TestCase(u8"Copyable types can be copy-assigned.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Copyable = ConceptsTestFixture::Copyable;

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Copyable, Immutable<Copyable>>), true);
    })

    .TestCase(u8"Types cannot be assigned from unrelated types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Some = ConceptsTestFixture::Some;
        using Unrelated = ConceptsTestFixture::Unrelated;

        SYNTROPY_UNIT_EQUAL((IsAssignable<Some, Unrelated>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignable<Some, Immutable<Unrelated>>), false);
        SYNTROPY_UNIT_EQUAL((IsAssignable<Some, Movable<Unrelated>>), false);
    })

    .TestCase(u8"Immutable reference-types cannot be assigned.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Some = ConceptsTestFixture::Some;

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Immutable<Some>, Some>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Immutable<Some>, Immutable<Some>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Immutable<Some>, Mutable<Some>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Immutable<Some>, Immovable<Some>>), false);
    })

    .TestCase(u8"Types can be assigned from types that are implicitly "
              u8"convertible to them.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Some = ConceptsTestFixture::Some;
        using Convertible = ConceptsTestFixture::Convertible;

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Some, Convertible>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Some, Mutable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Some, Immutable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Some, Movable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Some, Immovable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Mutable<Some>, Convertible>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Mutable<Some>, Mutable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Mutable<Some>, Immutable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Mutable<Some>, Movable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignable<Mutable<Some>, Immovable<Convertible>>), true);
    })

    .TestCase(u8"Types that are trivially-move-constructible, "
              u8"trivially-move-assignable, trivially-copy-constructible, "
              u8"trivially-copy-assignable and trivially-destructible are "
              u8"trivially copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using TriviallyCopyable = ConceptsTestFixture::TriviallyCopyable;

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<TriviallyCopyable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Immutable<TriviallyCopyable>>), true);
    })

    .TestCase(u8"Non-trivial-copyable types are not trivially-copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Copyable = ConceptsTestFixture::Copyable;

        SYNTROPY_UNIT_EQUAL((IsTriviallyCopyable<Copyable>), false);
        SYNTROPY_UNIT_EQUAL((IsTriviallyCopyable<Immutable<Copyable>>), false);
    })

    .TestCase(u8"Types that are trivially-copyable and "
              u8"trivially-default-constructible are trivial.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Trivial = ConceptsTestFixture::Trivial;

        SYNTROPY_UNIT_EQUAL((IsTrivial<Trivial>), true);

        SYNTROPY_UNIT_EQUAL((IsTrivial<Immutable<Trivial>>), true);
    })

    .TestCase(u8"Trivially-copyable types that are not "
              u8"trivially-default-constructible are not trivial.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using TriviallyCopyable = ConceptsTestFixture::TriviallyCopyable;

        SYNTROPY_UNIT_EQUAL((IsTrivial<TriviallyCopyable>), false);
        SYNTROPY_UNIT_EQUAL((IsTrivial<Immutable<TriviallyCopyable>>), false);
    })

    .TestCase(u8"Types that can be compared for equality are "
              u8"equality-comparable",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using EqualityComparable = ConceptsTestFixture::EqualityComparable;

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<EqualityComparable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<EqualityComparable>>), true);
    })

    .TestCase(u8"Types that cannot be compared for equality are not "
              u8"equality-comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonEqualityComparable
            = ConceptsTestFixture::NonEqualityComparable;

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<NonEqualityComparable>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<NonEqualityComparable>>), false);
    })

    .TestCase(u8"Types that can be three-way comparable are comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Comparable = ConceptsTestFixture::Comparable;

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Comparable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<Comparable>>), true);
    })

    .TestCase(u8"Types that cannot be three-way comparable are "
              u8"non-comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using NonComparable = ConceptsTestFixture::NonComparable;

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<NonComparable>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<NonComparable>>), false);
    })

    .TestCase(u8"Types that are template specialization of a template type "
              u8"are template-specialization-of.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<ConceptsTestFixture::Template<Int>,
                                        ConceptsTestFixture::Template>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<
                Immutable<ConceptsTestFixture::Template<Int>>,
                ConceptsTestFixture::Template>),
            true);
    })

    .TestCase(u8"Types that are not template specialization of a template "
              u8"type are not template-specialization-of.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<
                ConceptsTestFixture::UnrelatedTemplate<Int>,
                ConceptsTestFixture::Template>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<
                Immutable<ConceptsTestFixture::UnrelatedTemplate<Int>>,
                ConceptsTestFixture::Template>),
            false);
    })

    .TestCase(u8"Empty TypeList are typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((IsTypeList<Immutable<TypeList<>>>), true);
    })

    .TestCase(u8"Non-empty TypeList are typelists",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<Int, Float>>), true);
    })

    .TestCase(u8"Types other than TypeList are not typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsTypeList<Int>), false);
    })

    .TestCase(u8"Sequence types are sequences.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsSequence<Sequence<1, 3, 5>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsSequence<Immutable<Sequence<1, 3, 5>>>), true);
    })

    .TestCase(u8"Non sequence types are not sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsSequence<Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSequence<Immutable<Int>>), false);
    })

    .TestCase(u8"Contiguous sequence types are contiguous sequences.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 2, 3>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1, 2, 3>>>), true);
    })

    .TestCase(u8"Zero-element sequences are not contiguous sequences",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<>>>), false);
    })

    .TestCase(u8"One-element sequences are contiguous sequences",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1>>>), true);
    })

    .TestCase(u8"Non-contiguous sequence types are not contiguous sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 3, 2>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1, 3, 2>>>), false);
    });

}

// ===========================================================================
