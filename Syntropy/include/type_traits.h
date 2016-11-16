
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

namespace syntropy {

    //////////////// META ////////////////

    /// \brief Don't care functor.
    /// This type can be constructed or called with any number of arguments and does nothing in either case.
    /// \author Raffaele D. Facendola - September 2016
    struct _ {

        template <typename... TArgs>
        _(TArgs&&...) {}

        template <typename... TArgs>
        void operator()(TArgs&&...) {}

    };

    //////////////// STREAM CAPABILITIES ////////////////

    /// \brief If TStream& << TType is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TStream, typename TType>
    class is_stream_insertable {

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
    class is_stream_extractable {

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
    struct stream_insert {

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

    /// \brief If TAssignee = TValue is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - September 2016
    template <typename TAssignee, typename TValue>
    class is_assignable{

        template<typename A, typename V>
        static auto test(int) -> decltype(std::declval<A&>() = std::declval<V>(), std::true_type());

        template<typename, typename>
        static auto test(...)->std::false_type;

    public:

        static const bool value = decltype(test<TAssignee, TValue>(0))::value;

    };

    /// \brief Helper value for is_assignable<TAssignee, TValue>.
    template <typename TAssignee, typename TValue>
    constexpr bool is_assignable_v = is_assignable<TAssignee, TValue>::value;

    /// \brief If TCallable(TArgs...) is defined provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - September 2016
    template <typename TCallable, typename... TArgs>
    class is_callable {

        template<typename C, typename... A>
        static auto test(int) -> decltype(std::declval<C&>()(std::declval<A>()...), std::true_type());

        template<typename, typename...>
        static auto test(...)->std::false_type;

    public:

        static const bool value = decltype(test<TCallable, TArgs...>(0))::value;

    };

    /// \brief Helper value for is_callable_v<TCallable, TArgs...>.
    template <typename TCallable, typename... TArgs>
    constexpr bool is_callable_v = is_callable<TCallable, TArgs...>::value;
    
    /// \brief Functor used to call a callable object with any parameters.
    /// \author Raffaele D. Facendola - September 2016
    struct call {

        template <typename TCallable, typename... TArgs>
        void operator()(TCallable& callable, TArgs&&... args) {

            return callable(std::forward<TArgs>(args)...);

        }

    };

    /// \brief If TCallable can be called with the parameters TArgs..., the call is performed, otherwise this function does nothing.
    /// \return Returns the return value if the call could be performed, returns nothing otherwise.
    /// \author Raffaele D. Facendola - September 2016
    template <typename TCallable, typename... TArgs>
    auto conditional_call(TCallable& callable, TArgs&&... args) {

        using caller = std::conditional_t<is_callable_v<TCallable, TArgs...>, call, _>;

        return caller()(callable, std::forward<TArgs>(args)...);

    }

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
    
}