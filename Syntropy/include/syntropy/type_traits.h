
/// \file type_traits.h
/// \brief This header is part of syntropy module. It contains additional type traits to complement standard ones.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <typeinfo>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <array>

namespace std
{
    /// \brief (C++20) If the type T is a reference type, provides the member typedef type which is the type referred to by T with its topmost cv-qualifiers removed. Otherwise type is T with its topmost cv-qualifiers removed.
    template <typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
}

namespace syntropy
{
    /************************************************************************/
    /* UTILITIES                                                            */
    /************************************************************************/

    /// \brief Type equal to TType without references, extents and qualifiers.
    template <typename TType>
    using remove_extents_cvref = std::remove_cv_t<std::remove_all_extents_t<std::remove_reference_t<TType>>>;

    /// \brief Provides a member typedef which is the resulting type after stripping either all references or extents, an indirection level or all qualifiers from TType (in this order).
    template <typename TType>
    struct strip : std::conditional<std::is_array<TType>::value || std::is_reference<TType>::value,
        std::remove_all_extents_t<std::remove_reference_t<TType>>,              // Remove references and extents from the outermost level (mutually exclusive)
        std::conditional_t<std::is_pointer<TType>::value,
        std::remove_pointer_t<TType>,                                           // Remove a pointer level (removes qualifiers as well)
        std::remove_cv_t<TType>>> {};                                           // Remove const and volatile qualifiers from the innermost level

    /// \brief Helper type strip<TType>.
    template <typename TType>
    using strip_t = typename strip<TType>::type;

    /// \brief If the sequence {Ints} is contiguous provides a member constant value equal to true, otherwise value is false.
    template <typename T, T... Ints>
    struct is_contiguous_sequence : std::true_type {};

    /// \brief Specialization for two-element sequences or more.
    template <typename T, T Int1, T Int2, T... Ints>
    struct is_contiguous_sequence<T, Int1, Int2, Ints...>
    {
        static constexpr bool value = (Int1 + T(1) == Int2) && is_contiguous_sequence<T, Int2, Ints...>::value;
    };

    /// \brief Typedef of is_contiguous_sequence when T is std::size_t.
    template <std::size_t... Ints>
    struct is_contiguous_index_sequence : is_contiguous_sequence<std::size_t, Ints...> {};

    /// \brief Helper value for is_contiguous_sequence<T, Ints...>.
    template <typename T, T... Ints>
    constexpr bool is_contiguous_sequence_v = is_contiguous_sequence<T, Ints...>::value;

    /// \brief Helper value for is_contiguous_index_sequence<T, Ints...>.
    template <std::size_t... Ints>
    constexpr bool is_contiguous_index_sequence_v = is_contiguous_index_sequence<Ints...>::value;

    /// \brief Trait used to determine the type of the arguments of a callable object.
    template <typename TCallable>
    struct function_arguments : function_arguments<decltype(&std::decay_t<TCallable>::operator())> {};

    /// \brief Specialization for member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct function_arguments<TReturn(TCallable::*)(TArguments...)>
    {
        using type = std::tuple<TArguments...>;
    };

    /// \brief Specialization for const member functions.
    template <typename TCallable, typename TReturn, typename... TArguments>
    struct function_arguments<TReturn(TCallable::*)(TArguments...) const>
    {
        using type = std::tuple<TArguments...>;
    };

    /// \brief Helper type for FunctionArguments<TFunction>.
    template <typename TFunction>
    using function_arguments_t = typename function_arguments<TFunction>::type;

    /// \brief Trait used to determine the type of the kArgumentIndex-th argument of a callable object.
    template <std::size_t kArgumentIndex, typename TCallable>
    struct function_argument
    {
        using type = std::tuple_element_t<kArgumentIndex, function_arguments_t<TCallable>>;
    };

    /// \brief Helper type for FunctionArgument<kArgumentIndex, TFunction>.
    template <std::size_t kArgumentIndex, typename TFunction>
    using function_argument_t = typename function_argument<kArgumentIndex, TFunction>::type;

    /// \brief If TType is a specialization of TTemplate is_specialization_v is true, false otherwise.
    template<typename TType, template <typename...> typename TTemplate>
    constexpr bool is_specialization_v = false;

    /// \brief If TType is a specialization of TTemplate is_specialization_v is true, false otherwise.
    template<template <typename...> typename TTemplate, typename... TTypes>
    constexpr bool is_specialization_v<TTemplate<TTypes...>, TTemplate> = true;

    /// \brief Evaluates to false.
    template <typename...>
    inline constexpr bool false_v = false;

    /// \brief Provides a member constant value equal to the index of the first tuple element whose type is equal to TType.
    template <typename TType, typename TTuple>
    struct tuple_element_index;

    /// \brief Partial template specialization when the first element of the tuple is equal to TType.
    template <typename TType, typename... TTypes>
    struct tuple_element_index<TType, std::tuple<TType, TTypes...>>
    {
        static constexpr std::size_t value = 0;
    };

    /// \brief Partial template specialization when the first element in the tuple is not equal to TType. Discard the element and increase the value by one.
    template <typename TType, typename TDiscard, typename... TTypes>
    struct tuple_element_index<TType, std::tuple<TDiscard, TTypes...>>
    {
        static constexpr std::size_t value = 1 + tuple_element_index<TType, std::tuple<TTypes...>>::value;
    };

    /// \brief Helper value for tuple_element_index<TType, TTuple>.
    template <typename TType, typename TTuple>
    constexpr std::size_t tuple_element_index_v = tuple_element_index<TType, TTuple>::value;

    /************************************************************************/
    /* STREAM INSERTABLE \ EXTRACTABLE                                      */
    /************************************************************************/

    /// \brief If TStream& << TType is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TStream, typename TType>
    class is_stream_insertable
    {
        template<typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type());

        template<typename, typename>
        static auto test(...)->std::false_type;

    public:

        static const bool value = decltype(test<TStream, TType>(0))::value;

    };

    /// \brief If TStream& >> TType& is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TStream, typename TType>
    class is_stream_extractable
    {
        template<typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() >> std::declval<T&>(), std::true_type());

        template<typename, typename>
        static auto test(...)->std::false_type;

    public:

        static const bool value = decltype(test<TStream, TType>(0))::value;

    };

    /// \brief Helper value for is_stream_insertable<TStream, TType>.
    template <typename TStream, typename TType>
    constexpr bool is_stream_insertable_v = is_stream_insertable<TStream, TType>::value;

    /// \brief Helper value for is_stream_extractable<TStream, TType>.
    template <typename TStream, typename TType>
    constexpr bool is_stream_extractable_v = is_stream_extractable<TStream, TType>::value;

    /************************************************************************/
    /* CONTAINERS                                                           */
    /************************************************************************/

    /// \brief If TType represents a set provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - November 2016
    template <typename TType>
    struct is_set : std::false_type {};

    template <typename TType>
    struct is_set<std::set<TType>> : std::true_type {};

    template <typename TType>
    struct is_set<std::unordered_set<TType>> : std::true_type {};

    template <typename TType>
    struct is_set<std::multiset<TType>> : std::true_type {};

    template <typename TType>
    struct is_set<std::unordered_multiset<TType>> : std::true_type {};

    /// \brief Helper value for is_set<TType>.
    template <typename TType>
    constexpr bool is_set_v = is_set<TType>::value;

    /// \brief If TType represents a map provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - November 2016
    template <typename TType>
    struct is_map : std::false_type {};

    template <typename TKey, typename TValue>
    struct is_map<std::map<TKey, TValue>> : std::true_type {};

    template <typename TKey, typename TValue>
    struct is_map<std::unordered_map<TKey, TValue>> : std::true_type {};

    template <typename TKey, typename TValue>
    struct is_map<std::multimap<TKey, TValue>> : std::true_type {};

    template <typename TKey, typename TValue>
    struct is_map<std::unordered_multimap<TKey, TValue>> : std::true_type {};

    /// \brief Helper value for is_map<TType>.
    template <typename TType>
    constexpr bool is_map_v = is_map<TType>::value;

    /************************************************************************/
    /* CLASS NAMES                                                          */
    /************************************************************************/

    /// \brief Provides a member typedef which is the same as TType except that any pointer, qualifiers, references and extents are removed recursively.
    /// For example: class_name<int ** const *[1][3]> has a member type 'int'.
    template <typename TType, typename = void>
    struct class_name
    {
        using type = TType;
    };

    template <typename TType>
    struct class_name<TType, std::enable_if_t<!std::is_same_v<strip_t<TType>, TType>>> : class_name<strip_t<TType>> {};

    /// \brief Helper type for class_name<TType>.
    template <typename TType>
    using class_name_t = typename class_name<TType>::type;

    template <typename TType>
    constexpr bool is_class_name_v = std::is_same<class_name_t<TType>, TType>::value;

    /************************************************************************/
    /* ARRAYS                                                               */
    /************************************************************************/

    namespace details
    {
        /// \brief Builds the sequence of extents for the array TType.
        template <typename TType, size_t N, size_t... Extents>
        struct array_extents_helper : array_extents_helper<TType, N - 1, std::extent_v<TType, N - 1>, Extents...> {};

        template <typename TType, size_t... Extents>
        struct array_extents_helper<TType, 0, Extents...>
        {
            static constexpr std::array<size_t, sizeof...(Extents)> value = { Extents... };
        };
    }

    /// \brief Provides a member array value with the same rank as TType, where the Nth element contains the extents of TType in the Nth dimension.
    template <typename TType, typename = void>
    struct array_extents : details::array_extents_helper<TType, std::rank_v<TType> > {};

    /// \brief Helper constant for array_extents.
    template <template <typename> typename TPredicate, typename TType>
    constexpr size_t array_extents_v = array_extents<TType>::value;

    /************************************************************************/
    /* POINTERS                                                             */
    /************************************************************************/

    namespace details
    {
        template <typename TType, size_t count = 0>
        struct indirection_levels_helper : std::integral_constant<size_t, count> {};

        /// \brief Specialization for pointers, increases the indirection levels by 1.
        template <typename TType, size_t count>
        struct indirection_levels_helper<TType*, count> : indirection_levels_helper<std::remove_cv_t<TType>, count + 1> {};
    }

    /// \brief Provides a member value equal to the levels of indirection in TType.
    /// Arrays and references are not considered indirection levels.
    /// For example: indirection_levels<int ** const *[1][3]> has a member value equal to 3.
    template <typename TType>
    struct indirection_levels : details::indirection_levels_helper<remove_extents_cvref<TType>> {};

    /// \brief Helper constant for indirectin_levels.
    template <typename TType>
    constexpr size_t indirection_levels_v = indirection_levels<TType>::value;

    /// \brief Provides a member value whose bits represent the result of TPredicate for each level of indirection of TType, starting from the innermost level.
    /// For example: predicate_mask<std::is_const, int const ** const> has a member value equal to 101 (7)
    ///              equivalent to:  std::is_const<int const>::value |                                      1 * 2^0
    ///                             (std::is_const<int const *>::value << 1) |                              0 * 2^1
    ///                             (std::is_const<int const ** const>::value << 2) |                       1 * 2^2
    template <template <typename> typename TPredicate, typename TType, size_t mask = 0, typename = void>
    struct predicate_mask : predicate_mask<TPredicate, strip_t<TType>, (mask << 1u) | TPredicate<TType>::value> {};

    /// \brief Specialization for the last level of indirection.
    template <template <typename> typename TPredicate, typename TType, size_t mask>
    struct predicate_mask<TPredicate, TType, mask, std::enable_if_t<indirection_levels_v<TType> == 0 > > : std::integral_constant<size_t, (mask << 1u) | TPredicate<TType>::value> {};

    /// \brief Helper constant for predicate_mask.
    template <template <typename> typename TPredicate, typename TType>
    constexpr size_t predicate_mask_v = predicate_mask<TPredicate, TType>::value;
}