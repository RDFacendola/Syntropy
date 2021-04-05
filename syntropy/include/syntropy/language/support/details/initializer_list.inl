
/// \file initializer_list.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* INITIALIZER LIST                                                     */
    /************************************************************************/

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

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

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

// ===========================================================================
