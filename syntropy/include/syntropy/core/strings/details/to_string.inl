
/// \file to_string.inl
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

namespace Syntropy::StringsADL
{
    /************************************************************************/
    /* STRING EXTENSIONS                                                    */
    /************************************************************************/

    /// \brief Type alias for the ToString extension functor.
    template <typename TType>
    using ToStringExtension
        = Syntropy::Strings::Extensions::ToString<TType>;

    /// \brief Invoke the ToString function via extension functor.
    template <typename TType>
    [[nodiscard]] auto
    InvokeToStringExtension(Immutable<TType> rhs)
        noexcept -> decltype(ToStringExtension<TType>{}(rhs))
    {
        return ToStringExtension<TType>{}(rhs);
    }

    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    /// \brief Convert a string literal to a string.
    template <Int TSize>
    inline String
    ToString(StringLiteral<TSize> rhs) noexcept
    {
        return { rhs };
    }

    /// \brief Convert a boolean to a string.
    template <Templates::IsBoolean TBoolean>
    inline Immutable<String>
    ToString(TBoolean rhs) noexcept
    {
        static const auto kTrueString = String{u8"true"};
        static const auto kFalseString = String{u8"false"};

        return rhs ? kTrueString : kFalseString;
    }

    /// \brief Convert an integral to a string.
    template <Templates::IsIntegral TIntegral>
    inline String
    ToString(TIntegral rhs) noexcept
    {
        return u8"[integral]";  // #TODO @rfacendola Implement me!
    }

    /// \brief Convert a boolean to a string.
    template <Templates::IsReal TReal>
    inline String
    ToString(TReal rhs) noexcept
    {
        return u8"[real]";      // #TODO @rfacendola Implement me!
    }

    /// \brief Invoke the ToString function, trying different implementations.
    template <typename TType>
    constexpr decltype(auto)
    InvokeToString(Immutable<TType> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeToStringExtension(rhs))
        {
            return InvokeToStringExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.ToString())
        {
            return rhs.ToString();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(ToString(rhs))
        {
            return ToString(rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function)(rhs);
    }
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    /// \brief Convert rhs to a string.
    template <typename TType>
    [[nodiscard]] constexpr decltype(auto)
    Strings
    ::ToString(Immutable<TType> rhs) noexcept
    {
        return StringsADL::InvokeToString(rhs);
    }

}

// ===========================================================================
