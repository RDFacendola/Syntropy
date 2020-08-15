
/// \file utility.h
/// \brief This header is part of Syntropy language module. It contains support and utility definitions.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <utility>

#include "syntropy/core/types.h"
#include "syntropy/language/type_traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* DEFAULT CONSTRUCT T                                                  */
    /************************************************************************/

    /// \brief Empty tag type used to disambiguate between functions that forward tuples to construct objects.
    /// \author Raffaele D. Facendola - April 2020.
    struct DefaultConstructT
    { 
        /// \brief Default constructor.
        explicit DefaultConstructT() = default;
    };

    /// \brief Constant of the tag type DefaultConstructT used to create objects via default-constructor.
    inline constexpr DefaultConstructT kDefaultConstruct = DefaultConstructT{};

    /************************************************************************/
    /* TYPE TAG T                                                           */
    /************************************************************************/

    /// \brief Empty tag type used to propagate a type when explicit type is not supported (e.g. empty template constructors).
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TType>
    struct TypeT
    {
        /// \brief Default constructor.
        explicit TypeT() = default;
    };

    /// \brief Constant of the tag type TypeT used to propagate types.
    template <typename TType>
    inline constexpr TypeT<TType> kType = TypeT<TType>{};

    /************************************************************************/
    /* DON'T CARE T                                                         */
    /************************************************************************/

    /// \brief Empty tag type which can be constructed or called with any number of arguments doing nothing.
    /// Can be used with lambdas to discard any number of arguments.
    /// \author Raffaele D. Facendola - September 2016.
    struct DontCareT
    {
        /// \brief Construct from any argument number and types.
        template <typename... TArguments>
        constexpr DontCareT(TArguments&&...) {};

        /// \brief Assign from any argument number and types.
        template <typename... TArguments>
        constexpr void operator=(TArguments&&...) const {};
    };

    /// \brief Constant of the tag type DontCareT used to discard any number of arguments.
    inline constexpr DontCareT kDontCare = DontCareT{};

    /************************************************************************/
    /* EXPLICIT <TO, FROM...>                                               */
    /************************************************************************/

    /// \brief Type that bounds to an explicit type only.
    /// This type is meant to be used when implicit type conversion would
    /// cause overload resolution to be ambiguous.
    template <typename TTo, typename... TFrom>
    class Explicit
    {
    public:

        /// \brief Create a new explicit wrapper.
        template <typename UTo, typename = EnableIfT<(IsSameV<UTo, TFrom> || ...)>>
        constexpr Explicit(const UTo& value) noexcept;

        /// \brief Implicitly convert the wrapper to its underlying type.
        constexpr operator TTo() const noexcept;

    private:

        /// \brief Actual value.
        TTo value_{};

    };

    /// \brief Explicit boolean value.
    using ExplicitBool = Explicit<Bool, Bool>;

    /// \brief Explicit integer type.
    using ExplicitInt = Explicit<Int, Int, int>;

    /// \brief Explicit floating-point type.
    using ExplicitFloat = Explicit<Float, Float>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Converts any type TType to a reference type, making it
    /// possible to use member functions in decltype expressions without 
    /// the need to go through constructors.
    template<class TType>
    typename AddRValueReferenceT<TType> Declval() noexcept;

    /// \brief Convert an lvalue reference to a reference to a read-only value.
    template <typename TType>
    [[nodiscard]] constexpr AddConstT<TType>& ReadOnly(TType& rhs) noexcept;

    /// \brief Convert an lvalue reference to a reference to a read-write value.
    /// If rhs doesn't refer to a read-write value, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] constexpr TType& ReadWrite(const TType& rhs) noexcept;

    /// \brief Convert a pointer to memory location to a pointer to a read-only memory location.
    template <typename TType>
    [[nodiscard]] constexpr Pointer<const TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert a pointer to memory location to a pointer to a read-write memory location.
        /// If rhs doesn't refer to a read-write value, the behavior of this method is undefined.
    template <typename TType>
    [[nodiscard]] constexpr Pointer<TType> ReadWrite(Pointer<const TType> rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Explicit<TTo, TFrom...>.

    template <typename TTo, typename... TFrom>
    template <typename UTo, typename>
    constexpr Explicit<TTo, TFrom...>::Explicit(const UTo& value) noexcept
        : value_(static_cast<TTo>(value))
    {

    }

    template <typename TTo, typename... TFrom>
    constexpr Explicit<TTo, TFrom...>::operator TTo() const noexcept
    {
        return value_;
    }

    // Non-member functions.

    template<class TType>
    typename AddRValueReferenceT<TType> Declval() noexcept
    {
        return std::declval<TType>();
    }

    template <typename TType>
    constexpr AddConstT<TType>& ReadOnly(TType& rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr TType& ReadWrite(const TType& rhs) noexcept
    {
        return const_cast<TType&>(rhs);
    }

    template <typename TType>
    constexpr Pointer<const TType> ReadOnly(Pointer<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Pointer<TType> ReadWrite(Pointer<const TType> rhs) noexcept
    {
        return &(const_cast<TType&>(*rhs));
    }

}