
/// \file memory.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <memory>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <memory>
#include <type_traits>
#include <tuple>

#include "syntropy/language/type_traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Creates a TType object initialized with arguments arguments... at given address storage.
    /// \remarks #TODO Update to C++20 std::construct_at.
    template <typename TType, typename... TArguments>
    constexpr Pointer<TType> ConstructAt(Pointer<TType> storage, TArguments&&... arguments);

    /// \brief If T is not an array type, calls the destructor of the object pointed to by p, otherwise recursively destroys all elements;
    /// \remarks #TODO Update to C++20 std::destroy_at.
    template <typename TType>
    constexpr void DestroyAt(Pointer<TType> storage);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename... TArguments, typename = std::enable_if_t<!IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, TArguments&&...arguments);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename TTuple1, typename TTuple2, typename = std::enable_if_t<IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, std::piecewise_construct_t, TTuple1&& tuple1, TTuple2&& tuple2);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename = std::enable_if_t<IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename UType, typename VType, typename = std::enable_if_t<IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, UType&& u, VType&& v);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename UType, typename VType, typename = std::enable_if_t<IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, const std::pair<UType, VType>& uv);

    /// \brief Prepares the argument list needed to create an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::uses_allocator_construction_args.
    template <typename TType, typename TAllocator, typename UType, typename VType, typename = std::enable_if_t<IsSpecializationV<TType, std::pair>>>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, std::pair<UType, VType>&& uv);

    /// \brief Creates an object of the given type TType by means of uses-allocator construction.
    /// \remarks #TODO Update to C++20 std::make_obj_using_allocator.
    template <typename TType, typename TAllocator, typename... TArguments>
    /*constexpr*/ TType MakeObjUsingAllocator(const TAllocator& allocator, TArguments&&... arguments);

    /// \brief Creates an object of the given type TType by means of uses-allocator construction at the uninitialized memory location indicated by storage.
    /// \remarks #TODO Update to C++20 std::uninitialized_construct_using_allocator.
    template <typename TType, typename TAllocator, typename... TArguments>
    /*constexpr*/ Pointer<TType> UninitializedConstructUsingAllocator(Pointer<TType> storage, const TAllocator& allocator, TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Details.

    namespace details
    {
        /// \brief Helper class used to perfectly-forward arguments to ConstructAt.
        /// #TODO Using C++20 template lambdas, this class won't be needed any longer.
        template <typename TType>
        struct ConstructAtHelper
        {
            /// \brief Create a new helper for the provided storage location.
            ConstructAtHelper(Pointer<TType> storage);

            /// \brief Construct an object at registered storage location.
            template <typename... TArguments>
            Pointer<TType> operator()(TArguments&&... arguments);

            /// \brief Storage location used to construct the object into.
            Pointer<TType> storage_{ nullptr };
        };

        // Implementation.

        template <typename TType>
        ConstructAtHelper<TType>::ConstructAtHelper(Pointer<TType> storage)
            : storage_(storage)
        {

        }

        template <typename TType>
        template <typename... TArguments>
        Pointer<TType> ConstructAtHelper<TType>::operator()(TArguments&&... arguments)
        {
            return ConstructAt(storage_, std::forward<TArguments>(arguments)...);
        }

    }

    // Non-member functions.

    template <typename TType, typename... TArguments>
    constexpr Pointer<TType> ConstructAt(Pointer<TType> storage, TArguments&&... arguments)
    {
        return ::new (const_cast<void*>(static_cast<const volatile void*>(storage))) TType(std::forward<TArguments>(arguments)...);
    }

    template <typename TType>
    constexpr void DestroyAt(Pointer<TType> storage)
    {
        if constexpr (std::is_array_v<TType>)
        {
            for (auto&& element : storage)
            {
                DestroyAt(std::addressof(element));
            }
        }
        else
        {
            storage->~TType();
        }
    }

    template <typename TType, typename TAllocator, typename... TArguments, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, TArguments&&...arguments)
    {
        if constexpr (!std::uses_allocator_v<TType, TAllocator>)
        {
            // TType doesn't use the allocator: don't propagate the allocator.

            return std::forward_as_tuple(std::forward<TArguments>(arguments)...);
        }
        else if constexpr (std::is_constructible_v<TType, std::allocator_arg_t, TAllocator&, TArguments...>)
        {
            // Leading-allocator convention: propagate the allocator as the first argument (after the allocator tag).

            return std::tuple<std::allocator_arg_t, const TAllocator&, TArguments&&...>(std::allocator_arg, allocator, std::forward<TArguments>(arguments)...);
        }
        else if constexpr (std::is_constructible_v<TType, TArguments..., TAllocator>)
        {
            // Trailing-allocator convention: propagate the allocator as the last argument.

            return std::forward_as_tuple(std::forward<TArguments>(arguments)..., allocator);
        }
        else
        {
            static_assert(AlwaysFalseV<TType>, "Uses-allocator construction failed: TType is allocator-aware but doesn't use neither leading nor trailing allocator convention.");
        }
    }

    template <typename TType, typename TAllocator, typename TTuple1, typename TTuple2, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, std::piecewise_construct_t, TTuple1&& tuple1, TTuple2&& tuple2)
    {
        using TFirst = decltype(std::declval<TType>().first);
        using TSecond = decltype(std::declval<TType>().second);

        auto forward_tuple1 = [&allocator](auto&&... arguments)
        { 
            return UsesAllocatorConstructionArgs<TFirst>(allocator, std::forward<decltype(arguments)>(arguments)...);
        };

        auto forward_tuple2 = [&allocator](auto&&... arguments)
        {
            return UsesAllocatorConstructionArgs<TSecond>(allocator, std::forward<decltype(arguments)>(arguments)...);
        };

        return std::make_tuple(std::piecewise_construct, std::apply(forward_tuple1, std::forward<TTuple1>(tuple1)), std::apply(forward_tuple2 , std::forward<TTuple2>(tuple2)));
    }

    template <typename TType, typename TAllocator, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator)
    {
        return UsesAllocatorConstructionArgs<TType>(allocator, std::piecewise_construct, std::tuple<>(), std::tuple<>());
    }

    template <typename TType, typename TAllocator, typename UType, typename VType, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, UType&& u, VType&& v)
    {
        return UsesAllocatorConstructionArgs<TType>(allocator, std::piecewise_construct, std::forward_as_tuple(std::forward<UType>(u)), std::forward_as_tuple(std::forward<VType>(v)));
    }

    template <typename TType, typename TAllocator, typename UType, typename VType, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, const std::pair<UType, VType>& uv)
    {
        return UsesAllocatorConstructionArgs<TType>(allocator, std::piecewise_construct, std::forward_as_tuple(uv.first), std::forward_as_tuple(uv.second));
    }

    template <typename TType, typename TAllocator, typename UType, typename VType, typename>
    constexpr decltype(auto) UsesAllocatorConstructionArgs(const TAllocator& allocator, std::pair<UType, VType>&& uv)
    {
        return UsesAllocatorConstructionArgs<TType>(allocator, std::piecewise_construct, std::forward_as_tuple(std::forward<UType>(uv.first)), std::forward_as_tuple(std::forward<VType>(uv.second)));
    }

    template <typename TType, typename TAllocator, typename... TArguments>
    /*constexpr*/ TType MakeObjUsingAllocator(const TAllocator& allocator, TArguments&&... arguments)
    {
        return std::make_from_tuple<TType>(UsesAllocatorConstructionArgs<TType>(allocator, std::forward<TArguments>(arguments)...));
    }

    template <typename TType, typename TAllocator, typename... TArguments>
    /*constexpr*/ Pointer<TType> UninitializedConstructUsingAllocator(Pointer<TType> storage, const TAllocator& allocator, TArguments&&... arguments)
    {
        // Requires C++20.

        //auto construct = [&]<typename... TArguments>(TArguments&&...arguments)
        //{
        //    return ConstructAt(storage, std::forward<TArguments>(arguments)...);
        //};

        auto construct = details::ConstructAtHelper<TType>(storage);

        return std::apply(construct, UsesAllocatorConstructionArgs<TType>(allocator, std::forward<TArguments>(arguments)...));
    }

}