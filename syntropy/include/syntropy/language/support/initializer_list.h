
/// \file initializer_list.h
/// \brief This header is part of the Syntropy language module.
///        It contains definition for initializer lists.
///
/// \author Raffaele D. Facendola - Jun 2020.

#pragma once

#include <initializer_list>

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* INITIALIZER LIST                                                     */
    /************************************************************************/

    /// \brief A lightweight proxy object that provides access
    ///        to an array of const objects of type TElement.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename TElement>
    class InitializerList
    {
        template <typename TElement>
        friend constexpr RWPtr<TElement>
        begin(Immutable<InitializerList<TElement>> rhs) noexcept;

        template <typename TElement>
        friend constexpr RWPtr<TElement>
        end(Immutable<InitializerList<TElement>> rhs) noexcept;

    public:

        /// \brief Create a new empty list.
        constexpr InitializerList() noexcept = default;

        /// \brief Create a list from a pointer to the first and
        ///        past the last element.
        constexpr
        InitializerList(RWPtr<TElement> begin, RWPtr<TElement> end) noexcept;

        /// \brief Create an initializer list from a standard initializer list.
        constexpr
        InitializerList(Immutable<std::initializer_list<TElement>>
            initializer_list) noexcept;

    private:

        /// \brief Pointer to the first element in the list.
        RWPtr<TElement> begin_{ nullptr };

        /// \brief Pointer past the last element in the list.
        RWPtr<TElement> end_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TElement>
    constexpr RWPtr<TElement>
    begin(Immutable<InitializerList<TElement>> rhs) noexcept;

    template <typename TElement>
    constexpr RWPtr<TElement>
    end(Immutable<InitializerList<TElement>> rhs) noexcept;

    /// \brief Check whether an initializer list is empty.
    ///
    /// \return Returns true if the list is empty, returns false otherwise.
    template <typename TElement>
    [[nodiscard]] constexpr Bool
    IsEmpty(Immutable<InitializerList<TElement>> rhs) noexcept;

    /// \brief Get the number of elements in an initializer list.
    template <typename TElement>
    [[nodiscard]] constexpr Int
    Count(Immutable<InitializerList<TElement>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // InitializerList<TElement>.
    // ==========================

    template <typename TElement>
    constexpr InitializerList<TElement>
    ::InitializerList(RWPtr<TElement> begin, RWPtr<TElement> end) noexcept
        : begin_(begin)
        , end_(end)
    {

    }

    template <typename TElement>
    constexpr InitializerList<TElement>
    ::InitializerList(Immutable<std::initializer_list<TElement>>
        initializer_list) noexcept
        : begin_(initializer_list.begin())
        , end_(initializer_list.end())
    {

    }

    // Non-member functions.
    // =====================

    template <typename TElement>
    constexpr RWPtr<TElement>
    begin(Immutable<InitializerList<TElement>> rhs) noexcept
    {
        return rhs.begin_;
    }

    template <typename TElement>
    constexpr RWPtr<TElement>
    end(Immutable<InitializerList<TElement>> rhs) noexcept
    {
        return rhs.end_;
    }

    template <typename TElement>
    constexpr Bool
    IsEmpty(Immutable<InitializerList<TElement>> rhs) noexcept
    {
        return (Count(rhs) == 0);
    }

    template <typename TElement>
    constexpr Int
    Count(Immutable<InitializerList<TElement>> rhs) noexcept
    {
        return ToInt(End(rhs) - Begin(rhs));
    }

}
