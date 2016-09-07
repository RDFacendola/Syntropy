
/// \file type_traits
/// \brief Exposes additional type traits.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <typeinfo>

namespace syntropy {

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
    
    //////////////// IDENTITY ////////////////
    
    /// \brief Provides a member typedef which is the same a TType. Used to disable type deduction on template parameters.
    template <typename TType>
    struct identity {

        using type = TType;

    };

    /// \brief Helper type for identity<TType>
    template <typename TType>
    using identity_t = typename identity<TType>::type;
 
    //////////////// TYPES ////////////////

    /// \brief Functor used to an object that can be used to identify and compare the class of TInstance.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TType, typename TInstance>
    struct class_get {};

    template <typename TInstance>
    struct class_get<std::type_info, TInstance> {

        const std::type_info& operator()() const noexcept {

            return typeid(class_name_t<TInstance>);

        }

    };

    /// \brief Functor used to an object that can be used to identify and compare the type of TInstance.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TType, typename TInstance>
    struct type_get {};

    template <typename TInstance>
    struct type_get<std::type_info, TInstance> {
    
        const std::type_info& operator()() const noexcept {

            return typeid(TInstance);

        }

    };
    
    //////////////// QUALIFIERS ////////////////

    /// \brief Enumeration class for const qualifier presence.
    enum class ConstQualifier : bool {

        kConst = true,
        kNone = false

    };

    /// \brief Enumeration class for volatile qualifier presence.
    enum class VolatileQualifier : bool {

        kVolatile = true,
        kNone = false

    };

}