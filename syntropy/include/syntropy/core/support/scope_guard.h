
/// \file scope_guard.h
///
/// \brief This header is part of the Syntropy core module.
//         It contains definitions for scope-based guards.
///
/// \author Raffaele D. Facendola - Dec 2016

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SCOPE GUARD                                                          */
    /************************************************************************/

    /// \brief Guard that executes a routine upon destruction unless
    ///        it was dismissed.
    ///
    /// C++ and Beyond 2012: Systematic Error Handling in C++
    ///  - Andrei Alexandrescu
    ///
    /// http://channel9.msdn.com/Shows/Going+Deep/
    /// C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
    ///
    /// \author Raffaele D. Facendola - December 2016
    template <typename TFunctor>
    class ScopeGuard
    {
    public:

        /// \brief No default constructor.
        ScopeGuard() = delete;

        /// \brief No copy constructor.
        ScopeGuard(Immutable<ScopeGuard>) = delete;

        /// \brief No assignment operator.
        ScopeGuard& operator=(Immutable<ScopeGuard>) = delete;

        /// \brief Create a new scope guard.
        ScopeGuard(TFunctor functor) noexcept;

        /// \brief Move constructor. The original copy gets dismissed.
        ScopeGuard(Movable<ScopeGuard> other) noexcept;

        /// \brief Destroy this instance and calls the functor unless
        ///        the guard has been dismissed.
        ~ScopeGuard();

        /// \brief Dismiss the scope guard.
        void Dismiss() noexcept;

    private:

        /// \brief Functor wrapping the routine to be executed upon guard
        ///        destruction (if not dismissed)
        TFunctor functor_;

        ///< \brief Whether the scope guard was dismissed.
        Bool dismissed_{ false };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new scope guard.
    template <typename TFunctor>
    [[nodiscard]] ScopeGuard<TFunctor>
    MakeScopeGuard(TFunctor functor) noexcept;

}

// ===========================================================================

#include "scope_guard.inl"

// ===========================================================================
