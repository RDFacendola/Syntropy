
/// \file type_traits
/// \brief Exposes additional type traits.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <typeinfo>

namespace syntropy {

    template <typename TStream, typename TType>
    class is_stream_insertable {

        template<typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:

        static const bool value = decltype(test<TStream, TType>(0))::value;

    };

    //template <typename TStream, typename TType>
    //using is_stream_insertable_v = is_stream_insertable<TStream, TType>::value;

    template <typename TStream, typename TType>
    class is_stream_extractable {

        template<typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() >> std::declval<T&>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:

        static const bool value = decltype(test<TStream, TType>(0))::value;

    };

    //template <typename TStream, typename TType>
    //using is_stream_extractable_v = is_stream_extractable<TStream, TType>::value;

    template <typename TType>
    struct drop {

        using type = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<TType>>>;

    };

    template <typename TType>
    using drop_t = typename drop<TType>::type;

    template <typename TType>
    struct identity {

        using type = TType;

    };

    template <typename TType>
    using identity_t = typename identity<TType>::type;
 
    enum class ConstQualifier : bool {

        kConst = true,
        kNonConst = false

    };

    template <typename TType>
    struct type_base_of {};

    template <typename TType, typename TInstance>
    struct type_get {};

    template <typename TInstance>
    struct type_get<std::type_info, TInstance> {
    
        const std::type_info& operator()() const noexcept {

            return typeid(TInstance);

        }

    };

}