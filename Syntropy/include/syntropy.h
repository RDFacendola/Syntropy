
/// \file syntropy.h
/// \brief Common header for Syntropy library.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <type_traits>
#include <cassert>

#define SYN_UNUSED(x) (x)

#ifdef _MSC_VER
#define SYN_NO_INLINE __declspec(noinline)
#else
#define SYN_NO_INLINE 
#endif

#ifdef _DEBUG
#define SYN_DEBUG_ONLY(x) x
#else
#define SYN_DEBUG_ONLY(x)
#endif

#ifdef _DEBUG
#define SYN_RELEASE_ONLY(x) 
#else
#define SYN_RELEASE_ONLY(x) x
#endif

#define SYN_NON_COPYABLE_CLASS(ClassType) \
\
public: \
ClassType(const ClassType&) = delete; \
ClassType& operator=(const ClassType&) = delete;

#define SYN_SINGLETON_CLASS(ClassType) \
\
SYN_NON_COPYABLE_CLASS(ClassType) \
static ClassType& GetInstance(){ \
    static ClassType instance; \
    return instance; \
}


namespace syntropy {

    /// \brief Performs a cast between two types.
    /// Overload called whenever an up-cast is required.
    template <typename TDestination, typename TSource>
    typename std::enable_if_t<std::is_convertible<std::decay_t<TSource>, std::decay_t<TDestination>>::value, TDestination>
        cast(TSource&& ptr) {

        return static_cast<TDestination>(std::forward<TSource&&>(ptr));							// Safe upcast

    }

    /// \brief Performs a cast between two types.
    /// Overload called whenever a down-cast is required.
    template <typename TDestination, typename TSource>
    typename std::enable_if_t<!std::is_convertible<std::decay_t<TSource>, std::decay_t<TDestination>>::value &&
                               std::is_convertible<std::decay_t<TDestination>, std::decay_t<TSource>>::value, TDestination>
        cast(TSource&& ptr) {

        SYN_DEBUG_ONLY(return dynamic_cast<TDestination>(std::forward<TSource&&>(ptr)));			// RTTI check on debug
        SYN_RELEASE_ONLY(return static_cast<TDestination>(std::forward<TSource&&>(ptr)));			// Unsafe downcast on release

    }

    /// \brief Performs a cast between two types.
    /// Overload called whenever a cast to an unrelated type is requested.
    template <typename TDestination, typename TSource>
    typename std::enable_if_t<!std::is_convertible<std::decay_t<TSource>, std::decay_t<TDestination>>::value &&
                              !std::is_convertible<std::decay_t<TDestination>, std::decay_t<TSource>>::value, TDestination>
        cast(TSource&& ptr) {

        // The purpose of this method is to make clear that a cast between two unrelated types is being requested.
        // Without this, the SFINAE mechanism would only show that no proper overload could be found.

        static_assert(false, "Attempting to cast to an unrelated type");
                
    }

}