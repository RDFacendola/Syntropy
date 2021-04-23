
/// \file compare.inl
///
/// \author Raffaele D. Facendola - March 2021.

// ===========================================================================

#include "syntropy/language/templates/invoke.h"

// ===========================================================================

namespace Syntropy::ComparisonsADL
{
    /************************************************************************/
    /* COMPARE EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Type alias for the AreEqual extension functor.
    template <typename TType, typename UType>
    using AreEqualExtension
        = Comparisons::Extensions::AreEqual<TType, UType>;

    /// \brief Type alias for the AreEquivalent extension functor.
    template <typename TType, typename UType>
    using AreEquivalentExtension
        = Comparisons::Extensions::AreEquivalent<TType, UType>;

    /// \brief Type alias for the Compare extension functor.
    template <typename TType, typename UType>
    using CompareExtension
        = Comparisons::Extensions::Compare<TType, UType>;

    /// \brief Invoke the AreEqual function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeAreEqualExtension(Immutable<TType> lhs, Immutable<UType> rhs)
        noexcept -> decltype(AreEqualExtension<TType, UType>{}(lhs, rhs))
    {
        return AreEqualExtension<TType, UType>{}(lhs, rhs);
    }

    /// \brief Invoke the AreEquivalent function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeAreEquivalentExtension(Immutable<TType> lhs, Immutable<UType> rhs)
        noexcept -> decltype(AreEquivalentExtension<TType, UType>{}(lhs, rhs))
    {
        return AreEquivalentExtension<TType, UType>{}(lhs, rhs);
    }

    /// \brief Invoke the Compare function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeCompareExtension(Immutable<TType> lhs, Immutable<UType> rhs)
        noexcept -> decltype(CompareExtension<TType, UType>{}(lhs, rhs))
    {
        return CompareExtension<TType, UType>{}(lhs, rhs);
    }

    /************************************************************************/
    /* COMPARE                                                              */
    /************************************************************************/

    /// \brief Invoke the AreEqual function, trying different implementations.
    template <typename TType, typename UType>
    constexpr decltype(auto)
    InvokeAreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        auto extension = [](auto&& lhs, auto&& rhs)
            -> decltype(InvokeAreEqualExtension(lhs, rhs))
        {
            return InvokeAreEqualExtension(lhs, rhs);
        };

        auto member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(lhs.IsEqual(rhs))
        {
            return lhs.IsEqual(rhs);
        };

        auto non_member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(AreEqual(lhs, rhs))
        {
            return AreEqual(lhs, rhs);
        };

        auto fallback = [](auto& lhs, auto&& rhs)
            -> decltype(lhs == rhs)
        {
            return (lhs == rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function,
                                    fallback)(lhs, rhs);
    }

    /// \brief Invoke the AreEquivalent function, trying different
    ///        implementations.
    template <typename TType, typename UType>
    constexpr decltype(auto)
    InvokeAreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        auto extension = [](auto&& lhs, auto&& rhs)
            -> decltype(InvokeAreEquivalentExtension(lhs, rhs))
        {
            return InvokeAreEquivalentExtension(lhs, rhs);
        };

        auto member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(lhs.IsEquivalent(rhs))
        {
            return lhs.IsEquivalent(rhs);
        };

        auto non_member_function = [](auto&& lhs, auto&& rhs)
            -> decltype(AreEquivalent(lhs, rhs))
        {
            return AreEquivalent(lhs, rhs);
        };

        auto fallback = [](auto&& lhs, auto&& rhs)
            -> decltype(lhs == rhs)
        {
            return (lhs == rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function,
                                    fallback)(lhs, rhs);
    }

    /// \brief Invoke the Compare function, trying different implementations.
    template <typename TType, typename UType>
    constexpr decltype(auto)
    InvokeCompare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        auto extension = [](auto&& lhs, auto&& rhs)
            -> decltype(InvokeCompareExtension(lhs, rhs))
        {
            return InvokeCompareExtension(lhs, rhs);
        };

        auto member_function = [](auto& lhs, auto&& rhs)
            -> decltype(lhs.Compare(rhs))
        {
            return lhs.Compare(rhs);
        };

        auto non_member_function = [](auto& lhs, auto&& rhs)
            -> decltype(Compare(lhs, rhs))
        {
            return Compare(lhs, rhs);
        };

        auto fallback = [](auto& lhs, auto&& rhs)
            -> decltype(lhs <=> rhs)
        {
            return (lhs <=> rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function,
                                    fallback)(lhs, rhs);
    }
}

// ===========================================================================

namespace Syntropy::Comparisons
{
    /************************************************************************/
    /* COMPARE                                                              */
    /************************************************************************/

    template <typename TType, typename UType>
    [[nodiscard]] constexpr decltype(auto)
    AreEqual(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        return ComparisonsADL::InvokeAreEqual(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr decltype(auto)
    AreEquivalent(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        return ComparisonsADL::InvokeAreEquivalent(lhs, rhs);
    }

    template <typename TType, typename UType>
    [[nodiscard]] constexpr decltype(auto)
    Compare(Immutable<TType> lhs, Immutable<UType> rhs) noexcept
    {
        return ComparisonsADL::InvokeCompare(lhs, rhs);
    }

}

// ===========================================================================
