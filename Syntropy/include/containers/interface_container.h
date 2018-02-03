
/// \file interface_container.h
/// \brief This file contains classes used to extend the behavior of objects by adding and retrieving heterogeneous interfaces at runtime.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <vector>
#include <typeindex>
#include <algorithm>
#include <memory>

#include "type_traits.h"

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

        /// \brief Default constructor.
        InterfaceContainer() = default;

        /// \brief No copy constructor.
        InterfaceContainer(const InterfaceContainer&) = delete;

        /// \brief Default ove constructor.
        InterfaceContainer(InterfaceContainer&& other) = default;

        /// \brief No assignment operator.
        InterfaceContainer& operator=(const InterfaceContainer&) = delete;

        /// \brief Default destructor.
        ~InterfaceContainer() = default;

        /// \brief Add a new interface to the container.
        /// If an interface was already registered with the same TInterface, this method does nothing.
        /// \tparam TInterface Type of the interface to add.
        /// \tparam TConcrete Type of interface to instantiate. Must be equal to TInterface or a derived class.
        /// \param arguments Arguments to pass to TConcrete constructor.
        /// \return If the method succeeds returns a pointer to the new interface. If another interface was already registered with the same type, returns nullptr.
        template <typename TInterface, typename TConcrete = TInterface, typename... TArguments>
        TInterface* AddInterface(TArguments&&... arguments)
        {
            static_assert(std::is_base_of<TInterface, TConcrete>::value, "TConcrete must be equal to or derive from TInterface.");

            if (!GetInterface<TInterface>())
            {
                return interfaces_.emplace_back(tag_t<TInterface>{}, tag_t<TConcrete>{}, std::forward<TArguments>(arguments)...).As<TInterface>();
            }
            else
            {
                return nullptr;             // Another interface was already registered.
            }
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        TInterface* GetInterface()
        {
            return const_cast<TInterface*>(static_cast<const InterfaceContainer*>(this)->GetInterface<TInterface>());
        }

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        const TInterface* GetInterface() const
        {
            auto it = std::find_if(std::begin(interfaces_), std::end(interfaces_), [](const auto& interface)
            {
                return interface.Is<TInterface>();
            });

            return it != std::end(interfaces_) ? it->As<TInterface>() : nullptr;
        }

    private:

        /// \brief Wraps any interface.
        class AnyInterface
        {
        public:

            /// \brief Create a new interface.
            /// \tparam TInterface Registered interface type.
            /// \tparam TConcrete Actual interface to instantiate.
            /// \param arguments Arguments used to construct the interface.
            template <typename TInterface, typename TConcrete = TInterface, typename... TArguments>
            AnyInterface(tag_t<TInterface>, tag_t<TConcrete>, TArguments&&... arguments)
                : holder_(std::make_unique<HolderT<TConcrete>>(std::forward<TArguments>(arguments)...))
                , type_(typeid(TInterface))
            {
                static_assert(std::is_base_of<TInterface, TConcrete>::value, "TConcrete must be equal to or derive from TInterface.");
            }

            /// \brief Move constructor.
            AnyInterface(AnyInterface&& other)
                : holder_(std::move(other.holder_))
                , type_(std::move(other.type_))
            {

            }

            /// \brief No copy constructor.
            AnyInterface(const AnyInterface&) = delete;

            /// \brief No assignment operator.
            AnyInterface& operator=(const AnyInterface&) = delete;

            /// \brief Default destructor.
            ~AnyInterface() = default;

            /// \brief Check whether this interface can be statically casted to TClass.
            /// \return Returns true if this interface can be statically casted to TClass. Returns false otherwise.
            template <typename TClass>
            bool Is() const
            {
                return type_ == typeid(TClass);
            }

            /// \brief Cast the content of this interface to TClass.
            /// \return Returns a pointer to this interface, casted to TClass.
            template <typename TClass>
            TClass* As() const
            {
                SYNTROPY_ASSERT(Is<TClass>());      // Better safe than sorry.

                return reinterpret_cast<TClass*>(holder_->GetInterface());
            }

        private:

            /// \brief Holds an interface. Used for type-erasure purposes.
            struct Holder
            {
                /// \brief Get a raw pointer to the interface.
                /// \brief Returns a raw pointer to the interface.
                virtual void* GetInterface() = 0;

                /// \brief Get a raw pointer to the interface.
                /// \brief Returns a raw pointer to the interface.
                virtual const void* GetInterface() const = 0;
            };

            /// \brief Container for a concrete interface.
            template <typename TConcrete>
            struct HolderT : Holder
            {

                /// \brief Create a new concrete interface.
                /// \param arguments Arguments used to construct the interface.
                template <typename... TArguments>
                HolderT(TArguments&&... arguments)
                    : interface_(std::forward<TArguments>(arguments)...)
                {

                }

                virtual void* GetInterface() override
                {
                    return std::addressof(interface_);
                }

                virtual const void* GetInterface() const override
                {
                    return std::addressof(interface_);
                }

                TConcrete interface_;               ///< \brief Actual interface.
            };

            std::unique_ptr<Holder> holder_;    /// \brief Holds the concrete interface.

            std::type_index type_;              /// \brief Type of the interface stored inside the container

        };

        std::vector<AnyInterface> interfaces_;          ///< \brief Interfaces registered so far.

    };
}

