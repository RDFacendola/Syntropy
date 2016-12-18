
/// \file scope_guard.h
/// \brief Contains definition for classes used for systematic error handling via RAII paradigms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

namespace syntropy
{

    /// \brief Guards that executes a routine upon destruction unless it was dismissed.

    /// C++ and Beyond 2012: Systematic Error Handling in C++ - Andrei Alexandrescu
    /// http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
    /// \author Raffaele D. Facendola - December 2016
    template <typename TFunctor>
    class ScopeGuard {

    public:

        /// \brief No default constructor.
        ScopeGuard() = delete;

        /// \brief No copy constructor.
        ScopeGuard(const ScopeGuard &) = delete;

        /// \brief No assignment operator.
        ScopeGuard & operator=(const ScopeGuard &) = delete;

        /// \brief Create a new scope guard.
        /// \tparam TFunctor Type of the functor.
        /// \param functor Functor that wraps the routine to be executed upon this instance's destruction.
        ScopeGuard(TFunctor functor);

        /// \brief Move constructor. The original copy gets dismissed.
        /// \param other Original scope guard to move.
        ScopeGuard(ScopeGuard && other);

        /// \brief Destroy this instance and calls the functor unless the guard has been dismissed.
        ~ScopeGuard();

        /// \brief Dismiss the scope guard. The routine won't be called upon guard's destruction anymore.
        void Dismiss();

    private:

        TFunctor functor_;          ///< \brief Functor wrapping the routine to be executed upon guard destruction (if not dismissed)

        bool dismissed_;            ///< \brief Whether the scoper guard was dismissed.

    };

    /// \brief Create a new scope guard.
    /// \tparam TFunctor Type of the functor the ScopeGuard will call upon destruction.
    template <typename TFunctor>
    ScopeGuard<TFunctor> MakeScopeGuard(TFunctor functor);

}

namespace syntropy
{
    // Implementation

    //////////////// SCOPE GUARD ////////////////

    template <typename TFunctor>
    ScopeGuard<TFunctor>::ScopeGuard(TFunctor functor)
        : functor_(std::move(functor))
        , dismissed_(false) 
    {

    }

    template <typename TFunctor>
    ScopeGuard<TFunctor>::ScopeGuard(ScopeGuard && other)
        : functor_(std::move(other.functor_))
        , dismissed_(other.dismissed_)
    {
        other.Dismiss();
    }

    template <typename TFunctor>
    ScopeGuard<TFunctor>::~ScopeGuard() 
    {
        if (!dismissed_) 
        {
            functor_();     //Call the functor if not dismissed
        }
    }

    template <typename TFunctor>
    void ScopeGuard<TFunctor>::Dismiss()
    {
        dismissed_ = true;
    }

    template <typename TFunctor>
    ScopeGuard<TFunctor> MakeScopeGuard(TFunctor functor) 
    {
        return ScopeGuard<TFunctor>(std::move(functor));
    }

}
