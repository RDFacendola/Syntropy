
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
        
    //////////////// REMOVE ALL CV POINTER ////////////////

    /// \brief Provides a member typedef which is the same a TType except that any pointer and qualifiers are removed recursively.
    template <typename TType>
    struct remove_all_cv_pointer {

        using type = typename TType;

    };

    /// \brief Helper type form remove_all_cv_pointer<TType>.
    template <typename TType>
    using remove_all_cv_pointer_t = typename remove_all_cv_pointer<TType>::type;

    template <typename TType>
    struct remove_all_cv_pointer<TType*> : remove_all_cv_pointer<TType> {};

    template <typename TType>
    struct remove_all_cv_pointer<TType const> : remove_all_cv_pointer<TType> {};

    template <typename TType>
    struct remove_all_cv_pointer<TType volatile> : remove_all_cv_pointer<TType> {};

    template <typename TType>
    struct remove_all_cv_pointer<TType const volatile> : remove_all_cv_pointer<TType> {};
    
    //////////////// DROP ////////////////

    /// \brief Provides a member typedef which is the same a TType except that any pointer, extents, references and qualifiers are removed recursively.
    template <typename TType>
    struct drop {

        using type = typename remove_all_cv_pointer_t<std::remove_all_extents_t<std::remove_reference_t<TType>>>;
        
    };
    
    /// \brief Helper type for drop<TType>.
    template <typename TType>
    using drop_t = typename drop<TType>::type;  
    
    //////////////// REPLACE ////////////////

    /// \brief Provides a member typedef which is the same a TType except that the class name is replaced with TReplace if TType is non-reference non-array type, otherwise the type is TReplace.
    /// For example: replace_cv_pointer<int ** const *, float> has a member type 'float ** const *'.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TType, typename TReplace>
    struct replace_cv_pointer {
        
        using type = typename TReplace;

    };

    /// \brief Helper type replace_cv_pointer<TType, TReplace>.
    template <typename TType, typename TReplace>
    using replace_cv_pointer_t = typename replace_cv_pointer<TType, TReplace>::type;

    template <typename TType, typename TReplace>
    struct replace_cv_pointer<TType*, TReplace> {

        using type = std::add_pointer_t<typename replace_cv_pointer_t<TType, TReplace>>;

    };

    template <typename TType, typename TReplace>
    struct replace_cv_pointer<TType const, TReplace> {

        using type = std::add_const_t<typename replace_cv_pointer_t<TType, TReplace>>;

    };

    template <typename TType, typename TReplace>
    struct replace_cv_pointer<TType volatile, TReplace> {

        using type = std::add_volatile_t<typename replace_cv_pointer_t<TType, TReplace>>;

    };

    template <typename TType, typename TReplace>
    struct replace_cv_pointer<TType const volatile, TReplace> {

        using type = std::add_cv_t<typename replace_cv_pointer_t<TType, TReplace>>;

    };

    /// \brief Provides a member typedef which is the same a TType except that the class name is replaced with TReplace.
    /// For example: replace_cv_pointer<int ** const *[1][3], float> has a member type 'float ** const *[1][3]'.
    /// \author Raffaele D. Facendola - August 2016
    template <typename TType, typename TReplace>
    struct replace {

        using type = typename replace_cv_pointer_t<TType, TReplace>;

    };

    /// \brief Helper type replace<TType, TReplace>.
    template <typename TType, typename TReplace>
    using replace_t = typename replace<TType, TReplace>::type;
    
    template <typename TType, typename TReplace>
    struct replace<TType&, TReplace> {

        using type = std::add_lvalue_reference_t<typename replace_t<TType, TReplace>>;

    };

    template <typename TType, typename TReplace>
    struct replace<TType&&, TReplace> {

        using type = std::add_rvalue_reference_t<typename replace_t<TType, TReplace>>;

    };

    template <typename TType, typename TReplace>
    struct replace<TType[], TReplace> {

        using type = typename replace_t<TType, TReplace>[];

    };

    template <typename TType, typename TReplace, size_t N>
    struct replace<TType[N], TReplace> {

        using type = typename replace_t<TType, TReplace>[N];

    };

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