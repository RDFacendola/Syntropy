
/// \file address.h
///
/// \brief This header is part of Syntropy memory module.
///        It contains definitions for numeric address values.
///
/// \author Raffaele D. Facendola - August 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/algorithms/compare.h"

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
        template <typename UTraits>
        friend constexpr Int ToInt(BaseAddress<UTraits> rhs) noexcept;

    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Create a null-address.
        constexpr
        BaseAddress() noexcept = default;

        /// \brief Create a null-address.
        constexpr
        BaseAddress(Null) noexcept;

        /// \brief Create an address from a pointer.
        constexpr
        BaseAddress(TPointer pointer) noexcept;

        /// \brief Create an address from its numeric value.
        explicit constexpr
        BaseAddress(Int address) noexcept;

        /// \brief Default copy-constructor.
        constexpr
        BaseAddress(Immutable<BaseAddress>) noexcept = default;

        /// \brief Check whether the address refers to a valid location.
        [[nodiscard]] explicit constexpr
        operator Bool() const noexcept;

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
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator+(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    /// \brief Move the address backwards.
    template <typename TTraits>
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator-(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    /// \brief Bit-wise mask of a numeric address.
    template <typename TTraits>
    [[nodiscard]] constexpr BaseAddress<TTraits>
    operator&(BaseAddress<TTraits> lhs, Int rhs) noexcept;

    // Comparison.
    // ===========

    /// \brief Check whether two address are equivalent.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept;

    /// \brief Compare two addresses.
    template <typename TTraits, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(BaseAddress<TTraits> lhs, BaseAddress<UTraits> rhs) noexcept;

    // Conversions.
    // ============

    /// \brief Get the numeric value of an address.
    template <typename TTraits>
    [[nodiscard]] constexpr Int
    ToInt(BaseAddress<TTraits> rhs) noexcept;

    /// \brief Get the address to a read-only memory location.
    [[nodiscard]] Address
    ToAddress(TypelessPtr rhs) noexcept;

    /// \brief Get the address to a read-write memory location.
    [[nodiscard]] RWAddress
    ToAddress(RWTypelessPtr rhs) noexcept;

    /// \brief Convert an address to a strongly-typed read-only
    ///        instance of TType.
    template <typename TType>
    [[nodiscard]] Ptr<TType>
    FromAddress(Address rhs) noexcept;

    /// \brief Convert an address to a strongly-typed read-write
    ///        instance of TType.
    /// \remarks If the address doesn't refer to a read-writable memory
    ///          location, accessing the returned value results in undefined
    ///          behavior.
    template <typename TType>
    [[nodiscard]] RWPtr<TType>
    FromAddress(RWAddress rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to an address to a read-only memory location.
    template <typename TTraits>
    [[nodiscard]] constexpr Address
    ToReadOnly(BaseAddress<TTraits> rhs) noexcept;

    /// \brief Convert rhs to an address to a read-write memory location.
    /// \remarks If rhs doesn't refer to a read-write memory location,
    ///          accessing the returned value results in undefined behavior.
    template <typename TTraits>
    [[nodiscard]] constexpr RWAddress
    ToReadWrite(BaseAddress<TTraits> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create an address by deducing templates from arguments.
    template <typename TReference>
    [[nodiscard]] constexpr Address
    MakeAddress(Immutable<TReference> rhs) noexcept;

    /// \brief Create an address by deducing templates from arguments.
    template <typename TReference>
    [[nodiscard]] constexpr RWAddress
    MakeAddress(Mutable<TReference> rhs) noexcept;

}

// ===========================================================================

#include "address.inl"

// ===========================================================================
