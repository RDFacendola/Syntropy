
/// \file to_string.details.h
///
/// \author Raffaele D. Facendola - April 2021

// ===========================================================================

namespace Syntropy::Strings::Details
{
    /************************************************************************/
    /* TO STRING                                                            */
    /************************************************************************/

    template <typename TType>
    inline auto
    InvokeToString(Immutable<TType> rhs, Templates::ExtensionPriority)
        noexcept -> decltype(Extensions::ToString<TType>{}(rhs))
    {
        return Extensions::ToString<TType>{}(rhs);
    }

    template <typename TType>
    inline auto
    InvokeToString(Immutable<TType> rhs, Templates::MemberFunctionPriority)
        noexcept -> decltype(rhs.ToString())
    {
        return rhs.ToString();
    }

    template <typename TType>
    inline auto
    InvokeToString(Immutable<TType> rhs, Templates::NonMemberFunctionPriority)
        noexcept -> decltype(ToString(rhs))
    {
        return ToString(rhs);
    }

    template <Int TSize>
    inline String
    InvokeToString(StringLiteral<TSize> rhs, FallbackPriority) noexcept
    {
        return {rhs};
    }

    template <Templates::IsBoolean TBoolean>
    inline Immutable<String>
    InvokeToString(TBoolean rhs, FallbackPriority) noexcept
    {
        static const auto kTrueString = String{u8"true"};
        static const auto kFalseString = String{u8"false"};

        return rhs ? kTrueString : kFalseString;
    }

    template <Templates::IsIntegral TIntegral>
    inline String
    InvokeToString(TIntegral rhs, FallbackPriority) noexcept
    {
        return u8"[integral]";  // #TODO @rfacendola Implement me!
    }

    template <Templates::IsReal TReal>
    inline String
    InvokeToString(TReal rhs, FallbackPriority) noexcept
    {
        return u8"[real]";      // #TODO @rfacendola Implement me!
    }

    template <typename TType>
    inline void
    InvokeToString(Immutable<TType> rhs, IllFormedPriority) noexcept
    {
        static_assert(Templates::AlwaysFalse<TType>,
                      "The type doesn't provide an implementation for "
                      "ToString().");
    }

    template <typename TType>
    inline auto
    RouteToString(Immutable<TType> rhs)
        noexcept -> decltype(InvokeToString(rhs, Templates::kMaxPriority))
    {
        return InvokeToString(rhs, Templates::kMaxPriority);
    }

}

// ===========================================================================
