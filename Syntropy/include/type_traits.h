
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
        
    //////////////// DROP ////////////////

    /// \brief Provides a member typedef which is the same a TType except that any pointer, qualifiers and references are removed recursively.
    template <typename TType>
    struct drop {

        using type = TType;

    };
    
    /// \brief Helper type for drop<TType>.
    template <typename TType>
    using drop_t = typename drop<TType>::type;  

    /// \brief Specialization of drop<TType> used to strip pointers from TType.
    template <typename TType>
    struct drop<TType*> : drop<TType> {};

    /// \brief Specialization of drop<TType> used to strip const qualifier from TType.
    template <typename TType>
    struct drop<TType const> : drop<TType> {};

    /// \brief Specialization of drop<TType> used to strip volatile qualifier from TType.
    template <typename TType>
    struct drop<TType volatile> : drop<TType> {};

    /// \brief Specialization of drop<TType> used to strip const volatile qualifier from TType.
    /// \remarks This specialization is needed otherwise drop<TType const> and drop<TType volatile> would be ambiguous when TType has both const and volatile qualifiers.
    template <typename TType>
    struct drop<TType const volatile> : drop<TType> {};

    /// \brief Specialization of drop<TType> used to strip l-value references from TType.
    template <typename TType>
    struct drop<TType &> : drop<TType> {};

    /// \brief Specialization of drop<TType> used to strip r-value references from TType.
    template <typename TType>
    struct drop<TType &&> : drop<TType> {};

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