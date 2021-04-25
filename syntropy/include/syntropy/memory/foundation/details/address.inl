
/// \file address.inl
///
/// \author Raffaele D. Facendola - August 2020.

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE ADDRESS                                                         */
    /************************************************************************/

    template <typename TTraits>
    constexpr BaseAddress<TTraits>
    ::BaseAddress(Null) noexcept
    {

    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits>
    ::BaseAddress(TPointer pointer) noexcept
        : address_(reinterpret_cast<Int>(pointer))
    {

    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits>
    ::BaseAddress(Int address) noexcept
        : address_(address)
    {

    }

    template <typename TTraits>
    [[nodiscard]] constexpr
    BaseAddress<TTraits>
    ::operator Bool() const noexcept
    {
        return !!address_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Arithmetic.
    // ===========

    template <typename TTraits>
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator+(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) + rhs) };
    }

    template <typename TTraits>
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator-(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) - rhs) };
    }

    template <typename TTraits>
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator&(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) & rhs) };
    }

    // Comparison.
    // ===========

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(BaseAddress<TTraits> lhs,
                    BaseAddress<UTraits> rhs) noexcept
    {
        return ToInt(lhs) == ToInt(rhs);
    }

    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept
    {
        return ToInt(lhs) <=> ToInt(rhs);
    }

    // Conversions.
    // ============

    template <typename TTraits>
    [[nodiscard]] constexpr Int
    ToInt(BaseAddress<TTraits> rhs) noexcept
    {
        return rhs.address_;
    }

    [[nodiscard]] inline Address
    ToAddress(TypelessPtr rhs) noexcept
    {
        using Syntropy::ToInt;

        return Address{ ToInt(reinterpret_cast<std::intptr_t>(rhs)) };
    }

    [[nodiscard]] inline RWAddress
    ToAddress(RWTypelessPtr rhs) noexcept
    {
        using Syntropy::ToInt;

        return RWAddress{ ToInt(reinterpret_cast<std::intptr_t>(rhs)) };
    }

    template <typename TType>
    [[nodiscard]] inline Ptr<TType>
    FromAddress(Address rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(ToInt(rhs));
    }

    template <typename TType>
    [[nodiscard]] inline RWPtr<TType>
    FromAddress(RWAddress rhs) noexcept
    {
        return reinterpret_cast<RWPtr<TType>>(ToInt(rhs));
    }

    // Access.
    // =======

    template <typename TTraits>
    [[nodiscard]] constexpr Address
    ToReadOnly(BaseAddress<TTraits> rhs) noexcept
    {
        return Address{ ToInt(rhs) };
    }

    template <typename TTraits>
    [[nodiscard]] constexpr RWAddress
    ToReadWrite(BaseAddress<TTraits> rhs) noexcept
    {
        return RWAddress{ ToInt(rhs) };
    }

    // Utilities.
    // ==========

    template <typename TReference>
    [[nodiscard]] constexpr Address
    MakeAddress(Immutable<TReference> rhs) noexcept
    {
        return Address{ PtrOf(rhs) };
    }

    template <typename TReference>
    [[nodiscard]] constexpr RWAddress
    MakeAddress(Mutable<TReference> rhs) noexcept
    {
        return RWAddress{ PtrOf(rhs) };
    }

}

// ===========================================================================
