
/// \file view.inl
///
/// \author Raffaele D. Facendola - May 2021.

// ===========================================================================

#include "syntropy/language/support/invoke.h"

// ===========================================================================

namespace Syntropy::SupportADL
{
    /************************************************************************/
    /* SUPPORT EXTENSIONS                                                   */
    /************************************************************************/

    /// \brief Type alias for the ToReadOnly extension functor.
    template <typename TType>
    using ToReadOnlyExtension
        = Support::Extensions::ToReadOnly<TType>;

    /// \brief Type alias for the ToReadWrite extension functor.
    template <typename TType>
    using ToReadWriteExtension
        = Support::Extensions::ToReadWrite<TType>;

    /// \brief Invoke the ToReadOnly function via extension functor.
    template <typename TType>
    [[nodiscard]] auto
    InvokeToReadOnlyExtension(Immutable<TType> rhs)
        noexcept -> decltype(ToReadOnlyExtension<TType>{}(rhs))
    {
        return ToReadOnlyExtension<TType>{}(rhs);
    }

    /// \brief Invoke the ToReadWrite function via extension functor.
    template <typename TType, typename UType>
    [[nodiscard]] auto
    InvokeToReadWriteExtension(Immutable<TType> rhs)
        noexcept -> decltype(ToReadWriteExtension<TType>{}(rhs))
    {
        return ToReadWriteExtension<TType>{}(rhs);
    }

    /************************************************************************/
    /* SUPPORT                                                              */
    /************************************************************************/

    /// \brief Convert rhs to a pointer to a read-only instance of TType.
    template <typename TType>
    [[nodiscard]] constexpr Ptr<TType>
    ToReadOnly(Ptr<TType> rhs) noexcept
    {
        return const_cast<Ptr<TType>>(rhs);
    }

    /// \brief Invoke the ToReadOnly function, trying different
    ///        implementations.
    template <typename TType>
    constexpr auto
    InvokeToReadOnly(Forwarding<TType> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeToReadOnlyExtension(Forward<decltype(rhs)>(rhs)))
        {
            return InvokeToReadOnlyExtension(Forward<decltype(rhs)>(rhs));
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.ToReadOnly())
        {
            return rhs.ToReadOnly();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(ToReadOnly(Forward<decltype(rhs)>(rhs)))
        {
            return ToReadOnly(Forward<decltype(rhs)>(rhs));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(Forward<TType>(rhs));
    }

    /// \brief Convert rhs to a pointer to a read-write instance of TType.
    template <typename TType>
    [[nodiscard]] constexpr RWPtr<TType>
    ToReadWrite(Ptr<TType> rhs) noexcept
    {
        return const_cast<RWPtr<TType>>(rhs);
    }

    /// \brief Invoke the ToReadWrite function, trying different
    ///        implementations.
    template <typename TType>
    constexpr auto
    InvokeToReadWrite(Forwarding<TType> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeToReadWriteExtension(
                Forward<decltype(rhs)>(rhs)))
        {
            return InvokeToReadWriteExtension(Forward<decltype(rhs)>(rhs));
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.ToReadWrite())
        {
            return rhs.ToReadWrite();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(ToReadWrite(Forward<decltype(rhs)>(rhs)))
        {
            return ToReadWrite(Forward<decltype(rhs)>(rhs));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(Forward<TType>(rhs));
    }
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SUPPORT                                                              */
    /************************************************************************/

    template <typename TType>
    [[nodiscard]] constexpr auto
    Support
    ::ToReadOnly(Forwarding<TType> rhs) noexcept
    {
        return SupportADL::InvokeToReadOnly(Forward<TType>(rhs));
    }

    template <typename TType>
    [[nodiscard]] constexpr auto
    Support
    ::ToReadWrite(Forwarding<TType> rhs) noexcept
    {
        return SupportADL::InvokeToReadWrite(Forward<TType>(rhs));
    }

}

// ===========================================================================
