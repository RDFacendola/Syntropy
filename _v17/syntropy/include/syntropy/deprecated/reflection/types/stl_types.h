
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It exposes all the types supported by STL to the syntropy reflection system.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include <type_traits>
#include <memory>

#include "syntropy/types/string.h"
#include "syntropy/containers/vector.h"
#include "syntropy/containers/set.h"
#include "syntropy/containers/map.h"

#include "syntropy/reflection/class.h"

namespace Syntropy::reflection
{
    /************************************************************************/
    /* NULLPTR                                                              */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<std::nullptr_t>
    {
        static constexpr const char* name_{ "std::nullptr_t" };
    };

    /************************************************************************/
    /* VECTOR                                                               */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<Vector<TType> >
    {
        static constexpr const char* name_{ "Vector" };
    };

    /************************************************************************/
    /* PAIR                                                                 */
    /************************************************************************/

    template <typename TFirst, typename TSecond>
    struct ClassDeclarationT<std::pair<TFirst, TSecond> >
    {
        static constexpr const char* name_{ "std::pair" };
    };

    /************************************************************************/
    /* SETS                                                                 */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<Set<TType> >
    {
        static constexpr const char* name_{ "Set" };
    };

    template <typename TType>
    struct ClassDeclarationT<Multiset<TType> >
    {
        static constexpr const char* name_{ "Multiset" };
    };

    /************************************************************************/
    /* MAPS                                                                 */
    /************************************************************************/

    template <typename TKey, typename TValue>
    struct ClassDeclarationT<Map<TKey, TValue> >
    {
        static constexpr const char* name_{ "Map" };
    };

    template <typename TKey, typename TValue>
    struct ClassDeclarationT<Multimap<TKey, TValue> >
    {
        static constexpr const char* name_{ "Multimap" };
    };

    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    template <>
    struct ClassDeclarationT<String>
    {
        static constexpr const char* name_{ "Syntropy::String" };
    };

    template <>
    struct ClassDeclarationT<std::wstring>
    {
        static constexpr const char* name_{ "std::wstring" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::char_traits<TType>>
    {
        static constexpr const char* name_{ "std::char_traits" };
    };

    /************************************************************************/
    /* SMART POINTERS                                                       */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<std::unique_ptr<TType> >
    {
        static constexpr const char* name_{ "std::unique_ptr" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::shared_ptr<TType> >
    {
        static constexpr const char* name_{ "std::shared_ptr" };
    };

    /************************************************************************/
    /* ALLOCATION                                                           */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<std::allocator<TType> >
    {
        static constexpr const char* name_{ "std::allocator" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::default_delete<TType> >
    {
        static constexpr const char* name_{ "std::default_delete" };
    };

    /************************************************************************/
    /* HASHING                                                              */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<std::hash<TType> >
    {
        static constexpr const char* name_{ "std::hash" };
    };

    /************************************************************************/
    /* COMPARISON                                                           */
    /************************************************************************/

    template <typename TType>
    struct ClassDeclarationT<std::equal_to<TType> >
    {
        static constexpr const char* name_{ "std::equal_to" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::not_equal_to<TType> >
    {
        static constexpr const char* name_{ "std::not_equal_to" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::greater<TType> >
    {
        static constexpr const char* name_{ "std::greater" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::less<TType> >
    {
        static constexpr const char* name_{ "std::less" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::greater_equal<TType> >
    {
        static constexpr const char* name_{ "std::greater_equal" };
    };

    template <typename TType>
    struct ClassDeclarationT<std::less_equal<TType> >
    {
        static constexpr const char* name_{ "std::less_equal" };
    };
}
