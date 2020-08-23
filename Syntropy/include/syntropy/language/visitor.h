
/// \file visitor.h
/// \brief Contains definition for classes used to implement visitors and visitable objects and related functionalities.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <typeinfo>

#include "syntropy/language/fundamentals.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/language/type_traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new visitor that responds to different types specified by a list of lambdas.
    /// \usage auto visitor = MakeVistor([](int element){...}, [](Float element){...}, [](char element){...});
    ///        visitor.Visit(42).
    template <typename... TFunctions>
    auto MakeVisitor(TFunctions... functions);

    /************************************************************************/
    /* VISITOR                                                              */
    /************************************************************************/

    /// \brief Base class for generic visitors.
    /// Based on: https://foonathan.net/blog/2017/12/21/visitors.html.
    /// \author Raffaele D. Facendola - August 2019.
    /// \remarks This class doesn't support polymorphism.
    class Visitor
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~Visitor() = default;

        /// \brief Visit an element.
        template <typename TVisitable>
        void Visit(TVisitable visitable) const;

    protected:

        /// \brief Attempt to visit an element via a visitor functor.
        /// \return Returns true if the visit was successful, returns false otherwise.
        template <typename TFunction, typename TVisitor>
        Bool TryVisit(const TVisitor& visitor, RWTypelessPtr visitable, const std::type_info& type) const;

    private:

        /// \brief Visit an element.
        virtual void VirtualVisit(RWTypelessPtr visitable, const std::type_info& type) const = 0;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.

    template <typename... TFunctions>
    inline auto MakeVisitor(TFunctions... functions)
    {
        struct LambdaVisitor : public Visitor, public TFunctions...
        {
            // Gather many lambdas in a single callable object.
            LambdaVisitor(TFunctions... functions)
                : TFunctions(std::move(functions))...
            {

            }

            // Exposes one operator() for each element to handle.
            using TFunctions::operator()...;

            // Attempt to visit with each of the lambdas.
            void VirtualVisit(RWTypelessPtr visitable, const std::type_info& type) const override
            {
                (TryVisit<TFunctions>(*this, visitable, type) || ...);
            }
        };

        return LambdaVisitor(std::move(functions)...);
    }

    // Visitor.

    template <typename TVisitable>
    inline void Visitor::Visit(TVisitable visitable) const
    {
        if constexpr (!IsPolymorphicV<TVisitable> || IsFinalV<TVisitable>)
        {
            VirtualVisit(&visitable, typeid(visitable));
        }
        else
        {
            VirtualVisit(dynamic_cast<RWTypelessPtr>(&visitable), typeid(visitable));       // Downcast to the most derived class since typeid will return the dynamic type of visitable.
        }
    }

    template <typename TFunction, typename TVisitor>
    inline Bool Visitor::TryVisit(const TVisitor& visitor, RWTypelessPtr visitable, const std::type_info& type) const
    {
        using TArgument = FunctionArgumentT<0, TFunction>;
        using TVisitable = RemoveReferenceT<TArgument>;

        if (type == typeid(TVisitable))
        {
            visitor(*static_cast<TVisitable*>(visitable));
            return true;
        }

        return false;
    }

}