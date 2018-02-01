
/// \file type_traits
/// \brief Exposes additional type traits.
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
    using remove_extents_reference_cv_t = std::remove_cv_t<std::remove_all_extents_t<std::remove_reference_t<TType>>>;

    //////////////// STREAM CAPABILITIES ////////////////

    /// \brief If TStream& << TType is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TStream, typename TType>
    class is_stream_insertable
    {
        template<typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

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
        static auto test(...) -> std::false_type;

    public:

        static const bool value = decltype(test<TStream, TType>(0))::value;

    };

    /// \brief Helper value for is_stream_insertable<TStream, TType>.
    template <typename TStream, typename TType>
    constexpr bool is_stream_insertable_v = is_stream_insertable<TStream, TType>::value;

    /// \brief Helper value for is_stream_extractable<TStream, TType>.
    template <typename TStream, typename TType>
    constexpr bool is_stream_extractable_v = is_stream_extractable<TStream, TType>::value;

    template <typename TType, typename = void>
    struct stream_insert
    {

        std::ostream& operator()(std::ostream& output_stream, const TType& /*source*/) {

            output_stream.setstate(std::ios::failbit);          // The type doesn't support stream insertion.

            return output_stream;

        }

    };

    template <typename TType>
    struct stream_insert<TType, typename std::enable_if_t<is_stream_insertable_v<std::ostream, TType>>> {

        std::ostream& operator()(std::ostream& output_stream, const TType& source) {

            return output_stream << source;

        }

    };

    template <typename TType, typename = void>
    struct stream_extract {

        std::istream& operator()(std::istream& input_stream, TType& /*destination*/) {

            input_stream.setstate(std::ios::failbit);           // The type doesn't support stream extraction.

            return input_stream;

        }

    };

    template <typename TType>
    struct stream_extract<TType, typename std::enable_if_t<is_stream_extractable_v<std::istream, TType>>> {

        std::istream& operator()(std::istream& input_stream, TType& destination) {

            return input_stream >> destination;

        }

    };

    //////////////// OTHER CAPABILITIES ////////////////

	/// \brief If T is a specialization of std::in_place_type_t provides a member constant value equal to true, otherwise value is false.
	template <typename T>
	struct is_in_place_type : std::false_type {};

	/// \brief Template specialization for is_in_place_type when T is a specialization of in_place_type_t.
	template <typename T>
	struct is_in_place_type<std::in_place_type_t<T> > : std::true_type {};

    //////////////// CONTAINERS ////////////////

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

    //////////////// CLASS ////////////////

    /// \brief Provides a member typedef which is the same as TType except that any pointer, qualifiers, references and extents are removed recursively.
    /// For example: class_name<int ** const *[1][3]> has a member type 'int'.
    template <typename TType>
    struct class_name {

        using type = typename std::remove_cv_t<TType>;

    };

    template <typename TType>
    struct class_name<TType*> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType* const> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType* volatile> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType* const volatile> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType&> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType&&> : class_name<TType> {};

    template <typename TType>
    struct class_name<TType[]> : class_name<TType> {};

    template <typename TType, size_t size>
    struct class_name<TType[size]> : class_name<TType> {};
   
    /// \brief Helper type for class_name<TType>.
    template <typename TType>
    using class_name_t = typename class_name<TType>::type;  
    
    template <typename TType>
    constexpr bool is_class_name_v = std::is_same<class_name_t<TType>, TType>::value;

    /// \brief Provides a member typedef which is the same as TType except that the class name is replaced with TReplace.
    /// For example: replace_class_name<int ** const *[1][3], float> has a member type 'float ** const *[1][3]'.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TType, typename TReplace>
    struct replace_class_name {
        
        using type = typename std::conditional_t<std::is_const<TType>::value && std::is_volatile<TType>::value,
                                                 std::add_cv_t<TReplace>,
                                                 std::conditional_t<std::is_const<TType>::value,
                                                                    std::add_const_t<TReplace>,
                                                                    std::conditional_t<std::is_volatile<TType>::value,
                                                                                       std::add_volatile_t<TReplace>,
                                                                                       TReplace>>>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType*, TReplace> {

        using type = std::add_pointer_t<typename replace_class_name<TType, TReplace>::type>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType* const, TReplace> {

        using type = std::add_const_t<std::add_pointer_t<typename replace_class_name<TType, TReplace>::type>>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType* volatile, TReplace> {

        using type = std::add_volatile_t<std::add_pointer_t<typename replace_class_name<TType, TReplace>::type>>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType* const volatile, TReplace> {

        using type = std::add_cv_t<std::add_pointer_t<typename replace_class_name<TType, TReplace>::type>>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType&, TReplace> {

        using type = std::add_lvalue_reference_t<typename replace_class_name<TType, TReplace>::type>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType&&, TReplace> {

        using type = std::add_rvalue_reference_t<typename replace_class_name<TType, TReplace>::type>;

    };

    template <typename TType, typename TReplace>
    struct replace_class_name<TType[], TReplace> {

        using type = typename replace_class_name<TType, TReplace>::type[];

    };

    template <typename TType, typename TReplace, size_t N>
    struct replace_class_name<TType[N], TReplace> {

        using type = typename replace_class_name<TType, TReplace>::type[N];

    };

    /// \brief Helper type replace_class_name<TType, TReplace>.
    template <typename TType, typename TReplace>
    using replace_class_name_t = typename replace_class_name<TType, TReplace>::type;
    
    /// \brief Provides a member typedef which is the resulting type after stripping either all references or extents, an indirection level or all qualifiers from TType (in this order).
    template <typename TType>
    struct strip : std::conditional<std::is_array<TType>::value || std::is_reference<TType>::value,
                                    std::remove_all_extents_t<std::remove_reference_t<TType>>,              // Remove references and extents from the outermost level (mutually exclusive)
                                    std::conditional_t<std::is_pointer<TType>::value,
                                                       std::remove_pointer_t<TType>,                        // Remove a pointer level (removes qualifiers as well)
                                                       std::remove_cv_t<TType>>> {};                        // Remove const and volatile qualifiers from the innermost level

    /// \brief Helper type strip<TType>.
    template <typename TType>
    using strip_t = typename strip<TType>::type;

    namespace details
    {
        template <typename TType, size_t count = 0>
        struct indirection_levels_helper : std::integral_constant<size_t, count> {};

        /// \brief Specialization for pointers, increases the indirection levels by 1.
        template <typename TType, size_t count>
        struct indirection_levels_helper<TType*, count> : indirection_levels_helper<std::remove_cv_t<TType>, count + 1> {};

        /// \brief Builds the sequence of extents for the array TType.
        template <typename TType, size_t N, size_t... Extents>
        struct array_extents_helper : array_extents_helper<TType, N - 1, std::extent_v<TType, N - 1>, Extents...> {};

        template <typename TType, size_t... Extents>
        struct array_extents_helper<TType, 0, Extents...>
        {
            static constexpr std::array<size_t, sizeof...(Extents)> value = { Extents... };
        };
    }

    /// \brief Provides a member value representing the levels of indirection described by TType.
    /// Arrays and references are not considered indirection levels.
    /// For example: indirection_levels<int ** const *[1][3]> has a member value equal to 3.
    template <typename TType>
    struct indirection_levels : details::indirection_levels_helper<remove_extents_reference_cv_t<TType>> {};

    /// \brief Helper constant for indirectin_levels.
    template <typename TType>
    constexpr size_t indirection_levels_v = indirection_levels<TType>::value;

    /// \brief Provides a member value whose bits represent the result of TPredicate for each level of indirection of TType, starting from the innermost level.
    /// For example: predicate_mask<std::is_const, int const ** const> has a member value equal to 101 (7)
    ///              equivalent to:  std::is_const<int const>::value |                                      1 * 2^0
    ///                             (std::is_const<int const *>::value << 1) |                              0 * 2^1
    ///                             (std::is_const<int const ** const>::value << 2) |                       1 * 2^2
    template <template <typename> typename TPredicate, typename TType, size_t mask = 0, typename = void>
    struct predicate_mask : predicate_mask<TPredicate, strip_t<TType>, (mask << 1u) | TPredicate<TType>::value>{};

    /// \brief Specialization for the last level of indirection.
    template <template <typename> typename TPredicate, typename TType, size_t mask>
    struct predicate_mask<TPredicate, TType, mask, std::enable_if_t<indirection_levels_v<TType> == 0 > > : std::integral_constant<size_t, (mask << 1u) | TPredicate<TType>::value> {};

    /// \brief Helper constant for predicate_mask.
    template <template <typename> typename TPredicate, typename TType>
    constexpr size_t predicate_mask_v = predicate_mask<TPredicate, TType>::value;

    /// \brief Provides a member array value with the same rank as TType, where the Nth element contains the extents of TType in the Nth dimension.
    template <typename TType, typename = void>
    struct array_extents : details::array_extents_helper<TType, std::rank_v<TType> > {};

    /// \brief Helper constant for array_extents.
    template <template <typename> typename TPredicate, typename TType>
    constexpr size_t array_extents_v = array_extents<TType>::value;

}