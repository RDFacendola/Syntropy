
/// \file multi_interface.h
/// \brief This file is part of syntropy patterns. It contains mix-in used to extend objects dynamically by adding and retrieving heterogeneous interfaces at runtime.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <typeindex>
#include "syntropy/containers/map.h"
#include <type_traits>
#include <functional>

#include "syntropy/diagnostics/foundation/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* MULTI INTERFACE MIXIN                                                */
    /************************************************************************/

    /// \brief Trait used to accept any interface.
    template <typename TInterface>
    struct any_interface : std::true_type {};

    /// \brief Represents a composite of heterogeneous interfaces.
    /// This class is intended to be used as a mixin in order to dynamically extend the behavior of other classes by plugging interfaces at runtime and retrieving them by type.
    /// \tparam is_interface Type trait used to statically check requirements for interfaces T that can be added to this class. Must provide a boolean member 'value' equal to true if T is a valid interface for this class and false otherwise.
    /// \author Raffaele D. Facendola - April 2017
    template <template<typename> typename is_interface = any_interface>
    class MultiInterfaceMixin
    {
    public:

        /// \brief Add a new interface to the container.
        /// If another interface was registered with the same type the behavior of this method is undefined.
        /// \tparam TInterface Type of the interface to add.
        /// \tparam TConcrete Type of interface to instantiate. Must be equal to TInterface or a derived class.
        /// \param arguments Arguments to pass to TConcrete constructor.
        /// \return Returns a reference to the new interface.
        template <typename TInterface, typename TConcrete = TInterface, typename... TArguments>
        TInterface& AddInterface(TArguments&&... arguments)
        {
            static_assert(is_interface<TInterface>::value, "TInterface doesn't meet interface requirements.");

            static_assert(std::is_base_of_v<TInterface, TConcrete>, "TConcrete must be equal to or derive from TInterface.");

            static_assert(std::is_constructible_v<TConcrete, TArguments...>, "TConcrete must be constructible from TArguments.");

            auto[it, success] = interfaces_.emplace(typeid(TInterface), [interface = TConcrete(Forward<TArguments>(arguments)...)]() mutable
            {
                return &interface;
            });

            SYNTROPY_ASSERT(success);

            return *reinterpret_cast<TInterface*>(it->second());
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        TInterface* GetInterface()
        {
            return const_cast<TInterface*>(const_cast<const decltype(this)>(this)->GetInterface<TInterface>());
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        const TInterface* GetInterface() const
        {
            if constexpr(!is_interface<TInterface>::value)
            {
                return nullptr;         // No need to search if the provided type doesn't meet the requirements for interfaces.
            }
            else
            {
                auto it = interfaces_.find(typeid(TInterface));

                return it != std::end(interfaces_) ? reinterpret_cast<const TInterface*>(it->second()) : nullptr;
            }
        }

    protected:

        /// \brief This class is a mixin and cannot be instantiated directly.
        MultiInterfaceMixin() = default;

    private:

        Map<std::type_index, std::function<void*()>> interfaces_;         ///< \brief Interfaces registered so far. Interface object are store as part of the closure.
    };
}

