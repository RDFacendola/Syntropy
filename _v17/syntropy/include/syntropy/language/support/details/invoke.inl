
/// \file invoke.inl
///
/// \author Raffaele D. Facendola - December 2020.

// ===========================================================================

namespace Syntropy::Support
{
    /************************************************************************/
    /* INVOKE                                                               */
    /************************************************************************/

    template <typename TCallable, typename... TArguments>
    constexpr Templates::InvokeResultOf<TCallable, TArguments...>
    Invoke(Forwarding<TCallable> callable,
           Forwarding<TArguments>... arguments) noexcept
    {
        return std::invoke(Forward<TCallable>(callable),
                           Forward<TArguments>(arguments)...);
    }

    template <typename... TFunctions>
    [[nodiscard]] constexpr auto
    InvokeAny(TFunctions... functions) noexcept
    {
        return Details::InvocationSet<TFunctions...>{ Move(functions)... };
    }
}

// ===========================================================================
