
/// \file scope_guard.inl
///
/// \author Raffaele D. Facendola - December 2016.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SCOPE GUARD                                                          */
    /************************************************************************/

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>
    ::ScopeGuard(TFunctor functor) noexcept
        : functor_(Move(functor))
        , dismissed_(false)
    {

    }

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>
    ::ScopeGuard(Movable<ScopeGuard> other) noexcept
        : functor_(Move(other.functor_))
        , dismissed_(other.dismissed_)
    {
        other.Dismiss();
    }

    template <typename TFunctor>
    inline ScopeGuard<TFunctor>
    ::~ScopeGuard()
    {
        if (!dismissed_)
        {
            functor_();
        }
    }

    template <typename TFunctor>
    inline  void ScopeGuard<TFunctor>
    ::Dismiss() noexcept
    {
        dismissed_ = true;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TFunctor>
    [[nodiscard]] inline ScopeGuard<TFunctor>
    MakeScopeGuard(TFunctor functor) noexcept
    {
        return ScopeGuard<TFunctor>{ Move(functor) };
    }

}

// ===========================================================================
