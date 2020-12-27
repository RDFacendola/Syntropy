
/// \file address.h
/// \brief This header is part of Syntropy memory module. It contains definitions for numeric address values.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* BASE ADDRESS                                                         */
    /************************************************************************/

    /// \brief Numeric representation of a memory location address.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename TTraits>
    class BaseAddress
    {
        template <typename TTraits>
        friend constexpr Int ToInt(BaseAddress<TTraits> rhs) noexcept;

    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Create a null-address.
        constexpr BaseAddress() noexcept = default;

        /// \brief Create a null-address.
        constexpr BaseAddress(Null) noexcept;

        /// \brief Create an address from a pointer.
        constexpr BaseAddress(TPointer pointer) noexcept;

        /// \brief Create an address from its numeric value.
        explicit constexpr BaseAddress(Int address) noexcept;

        /// \brief Default copy-constructor.
        constexpr const BaseAddress(Immutable<BaseAddress>) noexcept = default;

        /// \brief Check whether the address refers to a valid location.
        explicit constexpr operator Bool() const noexcept;

    private:

        // Numeric address value.
        Int address_{ 0 };
    };

    /************************************************************************/
    /* ADDRESS                                                              */
    /************************************************************************/

    /// \brief Tag for read-only addresses.
    struct AddressTypeTraits
    {
        /// \brief Pointer to a memory location.
        using TPointer = TypelessPtr;
    };

    /// \brief Represents a read-only memory location address.
    using Address = BaseAddress<AddressTypeTraits>;

    /************************************************************************/
    /* RW ADDRESS                                                           */
    /************************************************************************/

    /// \brief Tag for read-only addresses.
    struct RWAddressTypeTraits
    {
        /// \brief Pointer to a memory location.
        using TPointer = RWTypelessPtr;
    };

    /// \brief Represents a read-only memory location address.
    using RWAddress = BaseAddress<RWAddressTypeTraits>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Arithmetic.
    // ===========

    /// \brief Move the address forwards.
    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator+(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    /// \brief Move the address backwards.
    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator-(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    /// \brief Bit-wise mask of a numeric address.
    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator&(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether two address are equivalent.
    template <typename TTraits, typename UTraits>
    constexpr Bool operator==(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept;

    /// \brief Compare two addresses.
    template <typename TTraits, typename UTraits>
    constexpr Ordering operator<=>(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept;

    // Conversions.
    // ============

    /// \brief Get the numeric value of an address.
    template <typename TTraits>
    constexpr Int ToInt(BaseAddress<TTraits> rhs) noexcept;

    /// \brief Get the address to a read-only memory location.
    Address ToAddress(TypelessPtr rhs) noexcept;

    /// \brief Get the address to a read-write memory location.
    RWAddress ToAddress(RWTypelessPtr rhs) noexcept;

    /// \brief Convert an address to a strongly-typed read-only instance of TType.
    template <typename TType>
    Ptr<TType> FromAddress(Address rhs) noexcept;

    /// \brief Convert an address to a strongly-typed read-write instance of TType.
    /// \remarks If the address doesn't refer to a read-writable memory location, accessing the returned value results in undefined behavior.
    template <typename TType>
    RWPtr<TType> FromAddress(RWAddress rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to an address to a read-only memory location.
    template <typename TTraits>
    constexpr Address ToReadOnly(BaseAddress<TTraits> rhs) noexcept;

    /// \brief Convert rhs to an address to a read-write memory location.
    /// \remarks If rhs doesn't refer to a read-write memory location, accessing the returned value results in undefined behavior.
    template <typename TTraits>
    constexpr RWAddress ToReadWrite(BaseAddress<TTraits> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create an address by deducing templates from arguments.
    template <typename TReference>
    constexpr Address MakeAddress(Immutable<TReference> rhs) noexcept;
    
    /// \brief Create an address by deducing templates from arguments.
    template <typename TReference>
    constexpr RWAddress MakeAddress(Mutable<TReference> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BaseAddress.
    // ============

    template <typename TTraits>
    constexpr BaseAddress<TTraits>::BaseAddress(Null) noexcept
    {

    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits>::BaseAddress(TPointer pointer) noexcept
        : address_(reinterpret_cast<Int>(pointer))
    {

    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits>::BaseAddress(Int address) noexcept
        : address_(address)
    {

    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits>::operator Bool() const noexcept
    {
        return !!address_;
    }

    // Non-member functions.
    // =====================

    // Arithmetic.

    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator+(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) + rhs) };
    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator-(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) - rhs) };
    }

    template <typename TTraits>
    constexpr BaseAddress<TTraits> operator&(BaseAddress<TTraits> lhs, Int rhs) noexcept
    {
        using Syntropy::ToInt;

        return BaseAddress<TTraits>{ ToInt(ToInt(lhs) & rhs) };
    }

    // Comparison.

    template <typename TTraits, typename UTraits>
    constexpr Bool operator==(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept
    {
        return ToInt(lhs) == ToInt(rhs);
    }

    template <typename TTraits, typename UTraits>
    constexpr Ordering operator<=>(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept
    {
        return ToInt(lhs) <=> ToInt(rhs);
    }

    // Conversions.

    template <typename TTraits>
    constexpr Int ToInt(BaseAddress<TTraits> rhs) noexcept
    {
        return rhs.address_;
    }

    inline Address ToAddress(TypelessPtr rhs) noexcept
    {
        using Syntropy::ToInt;

        return Address{ ToInt(reinterpret_cast<std::intptr_t>(rhs)) };
    }

    inline RWAddress ToAddress(RWTypelessPtr rhs) noexcept
    {
        using Syntropy::ToInt;

        return RWAddress{ ToInt(reinterpret_cast<std::intptr_t>(rhs)) };
    }

    template <typename TType>
    inline Ptr<TType> FromAddress(Address rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(ToInt(rhs));
    }

    template <typename TType>
    inline RWPtr<TType> FromAddress(RWAddress rhs) noexcept
    {
        return reinterpret_cast<RWPtr<TType>>(ToInt(rhs));
    }

    // Access.

    template <typename TTraits>
    constexpr Address ToReadOnly(BaseAddress<TTraits> rhs) noexcept
    {
        return Address{ ToInt(rhs) };
    }

    template <typename TTraits>
    constexpr RWAddress ToReadWrite(BaseAddress<TTraits> rhs) noexcept
    {
        return RWAddress{ ToInt(rhs) };
    }

    // Utilities.

    template <typename TReference>
    constexpr Address MakeAddress(Immutable<TReference> rhs) noexcept
    {
        return Address{ PtrOf(rhs) };
    }

    template <typename TReference>
    constexpr RWAddress MakeAddress(Mutable<TReference> rhs) noexcept
    {
        return RWAddress{ PtrOf(rhs) };
    }

}

// ===========================================================================
