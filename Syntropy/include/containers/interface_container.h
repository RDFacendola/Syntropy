
/// \file interface_container.h
/// \brief This file contains classes used to extend the behavior of objects by adding and retrieving heterogeneous interfaces at runtime.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <typeindex>
#include <algorithm>
#include <unordered_map>
#include <any>
#include <functional>

#include "patterns/utility.h"
#include "diagnostics/assert.h"

namespace syntropy
{

    /// \brief Represents a composite of heterogeneous interfaces.
    /// Used to dynamically extend the behavior of other objects by plugging interfaces at runtime and retrieving them by type.
    /// \author Raffaele D. Facendola - April 2017
    class InterfaceContainer
    {
    public:

        /// \brief Add a new interface to the container.
        /// If an interface was already registered with the same TInterface, this method does nothing.
        /// \tparam TInterface Type of the interface to add.
        /// \tparam TConcrete Type of interface to instantiate. Must be equal to TInterface or a derived class.
        /// \param arguments Arguments to pass to TConcrete constructor.
        /// \return If the method succeeds returns a pointer to the new interface. If another interface was registered with the same type returns nullptr.
        template <typename TInterface, typename TConcrete = TInterface, typename... TArguments>
        TInterface* AddInterface(TArguments&&... arguments)
        {
            static_assert(std::is_base_of<TInterface, TConcrete>::value, "TConcrete must be equal to or derive from TInterface.");

            auto[it, found] = interfaces_.emplace(typeid(TInterface), [interface = TConcrete(std::forward<TArguments>(arguments)...)]() mutable
            {
                return &interface;
            });

            return found ? reinterpret_cast<TInterface*>(it->second()) : nullptr;
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        TInterface* GetInterface()
        {
            return const_cast<TInterface*>(const_cast<const decltype(this)>(this)->GetInterface());
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        const TInterface* GetInterface() const
        {
            auto it = interfaces_.find(typeid(TInterface));

            return it != std::end(interfaces_) ? reinterpret_cast<const TInterface*>(it->second()) : nullptr;
        }

    private:

        using TAnyInterface = std::function<void*()>;                       ///< \brief Function used to access the interface (which is stored as part of the closure).

        std::unordered_map<std::type_index, TAnyInterface> interfaces_;     ///< \brief List of registered interfaces.
    };
}

