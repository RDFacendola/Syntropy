
/// \file visitor.inl
///
/// \author Raffaele D. Facendola - 2019

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* VISITOR                                                              */
    /************************************************************************/

    template <typename TVisitable>
    inline void Visitor
    ::Visit(TVisitable visitable) const noexcept
    {
        if constexpr ( ! Templates::IsPolymorphic<TVisitable>
                      || Templates::IsFinal<TVisitable>)
        {
            VirtualVisit(PtrOf(visitable), Reflection::TypeIdOf(visitable));
        }
        else
        {
            // Downcast to the most derived class since TypeIdOf will return
            // the dynamic type of visitable.

            auto visitable_ptr = dynamic_cast<RWTypelessPtr>(PtrOf(visitable));

            VirtualVisit(visitable_ptr, Reflection::TypeIdOf(visitable));
        }
    }

    template <typename TFunction, typename TVisitor>
    inline Bool Visitor
    ::TryVisit(Immutable<TVisitor> visitor,
               RWTypelessPtr visitable,
               Immutable<Reflection::TypeId> type) const noexcept
    {
        using TArgument = Templates::ArgumentOf<0, TFunction>;
        using TVisitable = Templates::QualifiedOf<TArgument>;

        if (type == Reflection::TypeIdOf<TVisitable>())
        {
            visitor(*FromTypelessPtr<TVisitable>(visitable));
            return true;
        }

        return false;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename... TFunctions>
    [[nodiscard]] inline auto
    MakeVisitor(TFunctions... functions) noexcept
    {
        struct LambdaVisitor : public Visitor, public TFunctions...
        {
            // Gather many lambdas in a single callable object.
            LambdaVisitor(TFunctions... functions)
                : TFunctions(Move(functions))...
            {

            }

            // Exposes one operator() for each element to handle.
            using TFunctions::operator()...;

            // Attempt to visit with each of the lambdas.
            void VirtualVisit(RWTypelessPtr visitable,
                              Immutable<Reflection::TypeId> type)
                              const noexcept override
            {
                (TryVisit<TFunctions>(*this, visitable, type) || ...);
            }
        };

        return LambdaVisitor(Move(functions)...);
    }

}

// ===========================================================================
