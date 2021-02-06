
/// \file visitor.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for visitors and visitable objects.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <typeinfo>

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/reflection/type_id.h"

// ===========================================================================

namespace syntropy
{
    /************************************************************************/
    /* VISITOR                                                              */
    /************************************************************************/

    /// \brief Base class for generic visitors.
    ///
    /// Based on: https://foonathan.net/blog/2017/12/21/visitors.html.
    ///
    /// \author Raffaele D. Facendola - August 2019.
    /// \remarks This class doesn't support polymorphism.
    class Visitor
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~Visitor() noexcept = default;

        /// \brief Visit an element.
        template <typename TVisitable>
        void Visit(TVisitable visitable) const noexcept;

    protected:

        /// \brief Attempt to visit an element via a visitor functor.
        ///
        /// \return Returns true if the visit was successful,
        ///         returns false otherwise.
        template <typename TFunction, typename TVisitor>
        Bool
        TryVisit(Immutable<TVisitor> visitor,
                 RWTypelessPtr visitable,
                 Immutable<Reflection::TypeId> type) const noexcept;

    private:

        /// \brief Visit an element.
        virtual void
        VirtualVisit(RWTypelessPtr visitable,
                     Immutable<Reflection::TypeId> type) const noexcept = 0;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new visitor that responds to different types
    ///        specified by a list of lambdas.
    ///
    /// \usage auto visitor = MakeVistor([](Int element){...},
    ///                                  [](Float element){...},
    ///                                  [](Bool element){...});
    ///        visitor.Visit(42).
    template <typename... TFunctions>
    [[nodiscard]] auto
    MakeVisitor(TFunctions... functions) noexcept;

}

// ===========================================================================

#include "visitor.inl"

// ===========================================================================
