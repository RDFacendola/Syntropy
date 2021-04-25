
/// \file invoke_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/invoke.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* INVOKE TEST FIXTURE                                                  */
    /************************************************************************/

    /// \brief Invoke test fixture.
    struct InvokeTestFixture
    {
        // Type aliases.

        using Int = Syntropy::Int;
        using Float = Syntropy::Float;
        using Bool = Syntropy::Bool;

        /// \brief Functor type.
        struct Functor
        {
            Int operator()(Int)
            {
                return 0;
            };
        };

        /// \brief Immutable functor type.
        struct ImmutableFunctor
        {
            Float operator()(Int, Float) const
            {
                return 0.0f;
            }
        };

        /// \brief Generic non-functor type.
        struct NonFunctor
        {

        };

        /// \brief Generic class type.
        struct Some
        {

        };

        /// \brief Generic member function.
        Float MemberFunction(Float, Float) const
        {
            return 0.0f;
        };

        /// \brief Generic static function.
        static Int StaticFunction(Int, Int)
        {
            return 0;
        }
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& invoke_unit_test
        = Syntropy::MakeAutoUnitTest<InvokeTestFixture>(
            u8"invoke.templates.language.syntropy")

    .TestCase(u8"InvokeArgumentOf of a function object return the list of "
              u8"types necessary to perform the invocation.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Functor = InvokeTestFixture::Functor;
        using ImmutableFunctor = InvokeTestFixture::ImmutableFunctor;

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<Functor>),
                           (TypeList<Int>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<ImmutableFunctor>),
                           (TypeList<Int, Float>));

        Invoke(Functor{}, Int{});
        Invoke(ImmutableFunctor{}, Int{}, Float{});
    })

    .TestCase(u8"InvokeArgumentOf a static or member function returns the "
              u8"list of types necessary to perform the invocation.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Some = InvokeTestFixture::Some;

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(*)(Float, Float)>),
                           (TypeList<Float, Float>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(Some::*)(Int)>),
                           (TypeList<Some, Int>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(Some::*)(Int) &>),
                           (TypeList<Mutable<Some>, Int>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(Some::*)(Int) const>),
                           (TypeList<Immutable<Some>, Int>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(Some::*)(Int) &&>),
                           (TypeList<Movable<Some>, Int>));

        SYNTROPY_UNIT_SAME((InvokeArgumentsOf<void(Some::*)(Int) const &&>),
                           (TypeList<Immovable<Some>, Int>));

        Invoke(&InvokeTestFixture::StaticFunction, Int{}, Int{});
        Invoke(&InvokeTestFixture::MemberFunction, fixture, Float{}, Float{});

    })

    .TestCase(u8"Callable object provide indexed access to their invocation"
              u8"argument types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Functor = InvokeTestFixture::Functor;
        using ImmutableFunctor = InvokeTestFixture::ImmutableFunctor;
        using Some = InvokeTestFixture::Some;

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<0, Functor>),(Int));

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<1, ImmutableFunctor>), (Float));

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<0, void(*)(Int, Float)>),
                           (Int));

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<0, void(Some::*)(Int, Float)>),
                           (Some));

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<1, void(Some::*)(Int, Float)>),
                           (Int));

        SYNTROPY_UNIT_SAME((InvokeArgumentOf<2, void(Some::*)(Int, Float)>),
                           (Float));
    })

    .TestCase(u8"InvokeResultOf is equal to the result type of a callable "
              u8"object invocation.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        using Functor = InvokeTestFixture::Functor;
        using ImmutableFunctor = InvokeTestFixture::ImmutableFunctor;
        using Some = InvokeTestFixture::Some;

        SYNTROPY_UNIT_SAME((InvokeResultOf<Functor, Int>),
                           (Int));

        SYNTROPY_UNIT_SAME((InvokeResultOf<ImmutableFunctor, Int, Float>),
                           (Float));

        SYNTROPY_UNIT_SAME((InvokeResultOf<void(*)(Int, Float)>), (void));
        SYNTROPY_UNIT_SAME((InvokeResultOf<Int(Some::*)(Float)>), (Int));
        SYNTROPY_UNIT_SAME((InvokeResultOf<Float(Some::*)(Int)>), (Float));
        SYNTROPY_UNIT_SAME((InvokeResultOf<Bool(Some::*)()>), (Bool));
    });

}

// ===========================================================================
