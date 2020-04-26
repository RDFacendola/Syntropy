
/// \file utility
/// \brief Exposes additional utility features.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <utility>

namespace syntropy
{

    /// \brief Don't care functor.
    /// This type can be constructed or called with any number of arguments and does nothing in either case.
    /// \author Raffaele D. Facendola - September 2016
    struct _
    {
        template <typename... TArgs>
        _(TArgs&&...);

        template <typename... TArgs>
        void operator()(TArgs&&...);
    };

    /// \brief Tag type used to declare a type when automatic type deduction cannot be used.
    template <typename TType>
    struct tag_t
    {
        explicit tag_t() = default;
    };

    template <typename TType>
    inline constexpr tag_t<TType> tag{};

    /// \brief Form a l-value reference to a non-const type of rhs.
    template <typename TType>
    std::remove_const_t<TType>& AsNonConst(const TType& rhs);

    /// \brief Form a l-value reference to a non-const type of rhs.
    template <typename TType>
    TType& AsNonConst(TType& rhs);

    /// \brief Form a l-value reference to a non-const type of rhs.
    template <typename TType>
    TType AsNonConst(TType&& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename... TArgs>
    _::_(TArgs&&...)
    {

    }

    template <typename... TArgs>
    void _::operator()(TArgs&&...)
    {

    }

    template <typename TType>
    std::remove_const_t<TType>& AsNonConst(const TType& rhs)
    {
        return const_cast<TType&>(rhs);
    }

    template <typename TType>
    TType& AsNonConst(TType& rhs)
    {
        return rhs;
    }

    template <typename TType>
    TType AsNonConst(TType&& rhs)
    {
        return rhs;
    }

}