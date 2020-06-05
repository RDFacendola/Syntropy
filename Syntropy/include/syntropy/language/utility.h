
/// \file utility.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <utility>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <utility>

#include "syntropy/language/type_traits.h"

namespace syntropy
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
    /* BOOLEAN                                                              */
    /************************************************************************/

    /// \brief Wraps a boolean value.
    /// This class is constructible from boolean values only and is meant to
    /// be used to prevent ambiguities in function overload resolution.
    /// \author Raffaele D. Facendola - June 2020.
    class Boolean
    {
    public:

        /// \brief Create a new boolean wrapper.
        template <typename TBool, typename = EnableIfT<IsSameV<TBool, Bool>>>
        Boolean(TBool value);

        /// \brief Unwrap the underlying value.
        operator Bool() const;

    private:

        /// \brief Underlying value.
        Bool value_{ false };

    };

    /************************************************************************/
    /* INTEGER                                                              */
    /************************************************************************/

    /// \brief Wraps an integer value.
    /// This class is constructible from integer values only and is meant to
    /// be used to prevent ambiguities in function overload resolution.
    /// \author Raffaele D. Facendola - June 2020.
    class Integer
    {
    public:

        /// \brief Create a new integer wrapper.
        template <typename TInt, typename = EnableIfT<IsIntegralV<TInt>>>
        Integer(TInt value);

        /// \brief Unwrap the underlying value.
        operator Int() const;

    private:

        /// \brief Underlying value.
        Int value_{ false };

    };

    /************************************************************************/
    /* FLOATING                                                             */
    /************************************************************************/

    /// \brief Wraps a floating-point value.
    /// This class is constructible from float values only and is meant to
    /// be used to prevent ambiguities in function overload resolution.
    /// \author Raffaele D. Facendola - June 2020.
    class Floating
    {
    public:

        /// \brief Create a new floating-point wrapper.
        template <typename TFloat, typename = EnableIfT<IsFloatingPointV<TFloat>>>
        Floating(TFloat value);

        /// \brief Unwrap the underlying value.
        operator Float() const;

    private:

        /// \brief Underlying value.
        Float value_{ false };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Forms lvalue reference to const type of rhs.
    /// \remarks Identical to C++17 std::as_const, wrapped for better consistency with AsNonConst.
    template <typename TType>
    constexpr std::add_const_t<TType>& AsConst(TType& rhs) noexcept;

    /// \brief Const rvalue reference deleted to disallow rvalue arguments.
    template <typename TType>
    constexpr void AsConst(const TType&&) = delete;

    /// \brief Forms lvalue reference to non-const type of rhs.
    /// \remarks Useful to write non-const getters from const ones without duplicating the implementation. Other usages are discouraged.
    template <typename TType>
    constexpr TType& AsNonConst(const TType& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Boolean.

    template <typename TBool, typename>
    inline Boolean::Boolean(TBool value)
        : value_(value)
    {

    }

    inline Boolean::operator Bool() const
    {
        return value_;
    }

    // Integer.

    template <typename TInt, typename>
    inline Integer::Integer(TInt value)
        : value_(value)
    {

    }

    inline Integer::operator Int() const
    {
        return value_;
    }

    // Floating.

    template <typename TFloat, typename>
    inline Floating::Floating(TFloat value)
        : value_(value)
    {

    }

    inline Floating::operator Float() const
    {
        return value_;
    }

    // Non-member functions.

    template <typename TType>
    constexpr std::add_const_t<TType>& AsConst(TType& rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr TType& AsNonConst(const TType& rhs) noexcept
    {
        return const_cast<TType&>(rhs);
    }

}