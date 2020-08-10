
/// \file scope_guard.h
/// \brief This header is part of the Syntropy core module. It contains definitions for scope-based guards.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/language/utility.h"
#include "syntropy/core/types.h"

namespace Syntropy
{
    /************************************************************************/
    /* SCOPE GUARD <TFUNCTOR>                                               */
    /************************************************************************/

    /// \brief Guards that executes a routine upon destruction unless it was dismissed.
    /// C++ and Beyond 2012: Systematic Error Handling in C++ - Andrei Alexandrescu
    /// http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
    /// \author Raffaele D. Facendola - December 2016
    template <typename TFunctor>
    class ScopeGuard
    {
    public:

        /// \brief No default constructor.
        ScopeGuard() = delete;

        /// \brief No copy constructor.
        ScopeGuard(const ScopeGuard&) = delete;

        /// \brief No assignment operator.
        ScopeGuard & operator=(const ScopeGuard&) = delete;

        /// \brief Create a new scope guard.
        ScopeGuard(TFunctor functor);

        /// \brief Move constructor. The original copy gets dismissed.
        ScopeGuard(ScopeGuard&& other);

        /// \brief Destroy this instance and calls the functor unless the guard has been dismissed.
        ~ScopeGuard();

        /// \brief Dismiss the scope guard.
        void Dismiss();

    private:

        ///< \brief Functor wrapping the routine to be executed upon guard destruction (if not dismissed)
        TFunctor functor_;

        ///< \brief Whether the scope guard was dismissed.
        Bool dismissed_{ false };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new scope guard.
    template <typename TFunctor>
    ScopeGuard<TFunctor> MakeScopeGuard(TFunctor functor);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ScopeGuard<TFunctor>.

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>::ScopeGuard(TFunctor functor)
        : functor_(std::move(functor))
        , dismissed_(false) 
    {

    }

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>::ScopeGuard(ScopeGuard && other)
        : functor_(std::move(other.functor_))
        , dismissed_(other.dismissed_)
    {
        other.Dismiss();
    }

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>::~ScopeGuard() 
    {
        if (!dismissed_) 
        {
            functor_();
        }
    }

    template <typename TFunctor>
    inline  void ScopeGuard<TFunctor>::Dismiss()
    {
        dismissed_ = true;
    }

    // Non-member function.

    template <typename TFunctor>
    inline  ScopeGuard<TFunctor> MakeScopeGuard(TFunctor functor)
    {
        return ScopeGuard<TFunctor>(std::move(functor));
    }

}
