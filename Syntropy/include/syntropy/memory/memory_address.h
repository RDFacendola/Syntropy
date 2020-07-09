
/// \file memory_address.h
/// \brief This header is part of the Syntropy memory module. It contains the definition of the address type and related functions.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include <iterator>
#include <ostream>

#include "syntropy/core/types.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY ADDRESS <IS CONST>                                            */
    /************************************************************************/

    /// \brief Represents a memory address.
    /// This type is meant to be a strongly-typed replacement for void* and intptr_t.
    /// \tparam kIsConst Whether the memory address points to a constant location or not.
    /// \author Raffaele D. Facendola - August 2018
    template <Bool kIsConst>
    class MemoryAddressT
    {
    public:

        /// \brief Select either a const or non-const type according to kIsConst.
        template <typename TType>
        using TSelect = ConditionalT<kIsConst, const TType, TType>;

        /// \brief Type alias for the typeless underlying value.
        using TRawPointer = TSelect<void>*;

        /// \brief Type alias for the underlying value type.
        using TValue = TSelect<Byte>;

        /// \brief Type alias for the underlying pointer type.
        using TPointer = TSelect<Byte>*;

        /// \brief  STL defines.
        using difference_type = Bytes;
        using value_type = TValue;
        using pointer = TValue*;
        using reference = TValue&;
        using iterator_category = std::random_access_iterator_tag;

        /// \brief Create a en empty address.
        constexpr MemoryAddressT() = default;

        /// \brief Create a new address.
        constexpr MemoryAddressT(TRawPointer address);

        /// \brief Create a en empty address.
        constexpr MemoryAddressT(std::nullptr_t);

        /// \brief Create a new address.
        constexpr MemoryAddressT(intptr_t address);

        /// \brief Default copy-constructor.
        template <Bool kIsRHSConst>
        constexpr MemoryAddressT(const MemoryAddressT<kIsRHSConst>& rhs);

        /// \brief Default assignment operator.
        MemoryAddressT& operator=(const MemoryAddressT&) = default;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr operator TRawPointer() const noexcept;

        /// \brief Convert the address to an signed numeric value.
        /// \return Returns an signed number that represents the underlying address.
        constexpr operator intptr_t() const noexcept;

        /// \brief Check whether the memory address points to a valid address or to nullptr.
        /// \return Returns true if the stored address is nullptr, returns false otherwise.
        constexpr operator Bool() const noexcept;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr TValue& operator*() const noexcept;

        /// \brief Get the underlying pointer.
        /// \return Returns the underlying pointer.
        constexpr TPointer operator->() const noexcept;

        /// \brief Get the underlying strongly-type pointer.
        /// \return Returns the underlying strongly-type pointer.
        template <typename TType>
        constexpr TSelect<TType>* As() const noexcept;

        /// \brief Reinterpret the memory address and emplace a value at pointed location.
        template <typename TType>
        constexpr void Emplace(TType&& value);

        /// \brief Move the address forward.
        /// \return Returns a reference to this element.
        constexpr MemoryAddressT& operator+=(const Bytes& rhs) noexcept;

        /// \brief Move the address backwards.
        /// \return Returns a reference to this element.
        constexpr MemoryAddressT& operator-=(const Bytes& rhs) noexcept;

        /// \brief Check whether this address is aligned to the provided amount.
        /// \param Returns true if the address is aligned to alignment, returns false otherwise.
        constexpr Bool IsAlignedTo(Alignment alignment) const noexcept;

        /// \brief Align this address up to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or greater than this address.
        constexpr MemoryAddressT GetAligned(Alignment alignment) const noexcept;

        /// \brief Align this address down to the provided alignment.
        /// \return Returns a new address which is guaranteed to be aligned to alignment and equal or less than this address.
        constexpr MemoryAddressT GetAlignedDown(Alignment alignment) const noexcept;

    private:

        ///< \brief Underlying address.
        TPointer address_ = nullptr;

    };

    /************************************************************************/
    /* TYPE ALIAS                                                           */
    /************************************************************************/

    /// \brief Type alias for non-const memory addresses.
    using MemoryAddress = MemoryAddressT<false>;

    /// \brief Type alias for const memory address.
    using ConstMemoryAddress = MemoryAddressT<true>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Increment the address by one Byte. Prefix.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& operator++(MemoryAddressT<kIsConst>& rhs);

    /// \brief Increment the address by one Byte. Postfix.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator++(MemoryAddressT<kIsConst>& rhs, int);

    /// \brief Decrement the address by one Byte. Prefix.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& operator--(MemoryAddressT<kIsConst>& rhs);

    /// \brief Decrement the address by one Byte. Postfix.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator--(MemoryAddressT<kIsConst>& rhs, int);

    /// \brief Equality comparison for MemoryAddress.
    /// \return Returns true if lhs and rhs refer to the same address, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator==(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Inequality comparison for MemoryAddress.
    /// \return Returns true if lhs and rhs refer to different address, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator!=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Greater-than comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is strictly greater than rhs, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator>(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Less-than comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is strictly less than rhs, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator<(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Greater-or-equal comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is equal or greater than rhs, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator>=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Less-or-equal comparison for MemoryAddress.
    /// \return Returns true if lhs refers to an address that is equal or less than rhs, returns false otherwise.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator<=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsLHSConst>& rhs) noexcept;

    /// \brief Move an address forward.
    /// \return Returns an address which is equal to lhs moved forward by rhs bytes.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator+(const MemoryAddressT<kIsConst>& lhs, const Bytes& rhs) noexcept;

    /// \brief Move an address backward.
    /// \return Returns an address which is equal to lhs moved backward by rhs bytes.
    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator-(const MemoryAddressT<kIsConst>& lhs, const Bytes& rhs) noexcept;

    /// \brief Get the difference of two memory addresses.
    /// \return Returns a the signed difference between lhs and rhs.
    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bytes operator-(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept;

    /// \brief Make a new memory address deducing its constness.
    template <typename TPointer, Bool kIsConst = std::is_const_v<TPointer>>
    constexpr MemoryAddressT<kIsConst> MakeMemoryAddress(TPointer* pointer);

    /// \brief Make a new constant memory address.
    template <typename TPointer>
    constexpr ConstMemoryAddress MakeConstMemoryAddress(TPointer* pointer);

    /// \brief Stream insertion for MemoryAddress.
    template <Bool kIsConst>
    std::ostream& operator<<(std::ostream& lhs, const MemoryAddressT<kIsConst>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // MemoryAddressT<kIsConst>.

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::MemoryAddressT(MemoryAddressT::TRawPointer address)
        : address_(reinterpret_cast<TPointer>(address))
    {

    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::MemoryAddressT(std::nullptr_t)
        : MemoryAddressT()
    {

    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::MemoryAddressT(intptr_t address)
        : address_(reinterpret_cast<TPointer>(address))
    {

    }

    template <Bool kIsConst>
    template <Bool kIsRHSConst>
    constexpr MemoryAddressT<kIsConst>::MemoryAddressT(const MemoryAddressT<kIsRHSConst>& rhs)
        : address_(&(*rhs))
    {

    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::operator typename MemoryAddressT<kIsConst>::TRawPointer() const noexcept
    {
        return address_;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::operator intptr_t() const noexcept
    {
        return reinterpret_cast<intptr_t>(address_);
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>::operator Bool() const noexcept
    {
        return !!address_;
    }

    template <Bool kIsConst>
    constexpr typename MemoryAddressT<kIsConst>::TValue& MemoryAddressT<kIsConst>::operator*() const noexcept
    {
        return *address_;
    }

    template <Bool kIsConst>
    constexpr typename MemoryAddressT<kIsConst>::TPointer MemoryAddressT<kIsConst>::operator->() const noexcept
    {
        return address_;
    }

    template <Bool kIsConst>
    template <typename TType>
    constexpr MemoryAddressT<kIsConst>::TSelect<TType>* MemoryAddressT<kIsConst>::As() const noexcept
    {
        return reinterpret_cast<TSelect<TType>*>(address_);
    }

     template <Bool kIsConst>
     template <typename TType>
     constexpr void MemoryAddressT<kIsConst>::Emplace(TType&& value)
     {
         *As<TType>() = value;
     }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& MemoryAddressT<kIsConst>::operator+=(const Bytes& rhs) noexcept
    {
        address_ = address_ + ToInt(rhs);
        return *this;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& MemoryAddressT<kIsConst>::operator-=(const Bytes& rhs) noexcept
    {
        address_ = address_ - ToInt(rhs);
        return *this;
    }

    template <Bool kIsConst>
    constexpr Bool MemoryAddressT<kIsConst>::IsAlignedTo(Alignment alignment) const noexcept
    {
        return (intptr_t(*this) & (ToInt(alignment) - 1)) == 0;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> MemoryAddressT<kIsConst>::GetAligned(Alignment alignment) const noexcept
    {
        auto alignment_mask = ToInt(alignment) - 1;

        return (intptr_t(*this) + alignment_mask) & ~alignment_mask;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> MemoryAddressT<kIsConst>::GetAlignedDown(Alignment alignment) const noexcept
    {
        auto alignment_mask = ToInt(alignment) - 1;

        return intptr_t(*this) & ~alignment_mask;
    }

    // Non-member functions.

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& operator++(MemoryAddressT<kIsConst>& rhs)
    {
        rhs += Bytes{ 1 };

        return rhs;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator++(MemoryAddressT<kIsConst>& rhs, int)
    {
        auto copy = rhs;

        ++rhs;

        return copy;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst>& operator--(MemoryAddressT<kIsConst>& rhs)
    {
        rhs -= Bytes{ 1 };

        return rhs;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator--(MemoryAddressT<kIsConst>& rhs, int)
    {
        auto copy = rhs;

        --rhs;

        return copy;
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator==(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return intptr_t(lhs) == intptr_t(rhs);
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator!=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator>(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return intptr_t(lhs) > intptr_t(rhs);
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator<(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return intptr_t(lhs) < intptr_t(rhs);
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator>=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return intptr_t(lhs) >= intptr_t(rhs);
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bool operator<=(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return intptr_t(lhs) <= intptr_t(rhs);
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator+(const MemoryAddressT<kIsConst>& lhs, const Bytes& rhs) noexcept
    {
        return MemoryAddressT<kIsConst>(lhs) += rhs;
    }

    template <Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> operator-(const MemoryAddressT<kIsConst>& lhs, const Bytes& rhs) noexcept
    {
        return MemoryAddressT<kIsConst>(lhs) -= rhs;
    }

    template <Bool kIsLHSConst, Bool kIsRHSConst>
    constexpr Bytes operator-(const MemoryAddressT<kIsLHSConst>& lhs, const MemoryAddressT<kIsRHSConst>& rhs) noexcept
    {
        return Bytes{ intptr_t(lhs) - intptr_t(rhs) };
    }

    template <typename TPointer, Bool kIsConst>
    constexpr MemoryAddressT<kIsConst> MakeMemoryAddress(TPointer* pointer)
    {
        return pointer;
    }

    template <typename TPointer>
    constexpr ConstMemoryAddress MakeConstMemoryAddress(TPointer* pointer)
    {
        return pointer;
    }

    template <Bool kIsConst>
    inline std::ostream& operator<<(std::ostream& lhs, const MemoryAddressT<kIsConst>& rhs)
    {
        return lhs << intptr_t(rhs);
    }

}

