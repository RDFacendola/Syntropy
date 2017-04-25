
/// \file interface_container.h
/// \brief This file contains classes used to extend the behavior of objects by adding and retrieving heterogeneous interfaces at runtime.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <type_traits>
#include <vector>
#include <typeindex>
#include <algorithm>
#include <memory>

namespace syntropy
{

    /// \brief Represents a composite of heterogeneous interfaces.
    /// Used to dynamically extend the behavior of other objects by plugging interfaces at runtime and retrieving them by type.
    /// \author Raffaele D. Facendola - April 2017
    template <typename TAllocator = void>
    class InterfaceContainer
    {
    public:

        /// \brief Default constructor.
        InterfaceContainer() = default;

        /// \brief No copy constructor.
        InterfaceContainer(const InterfaceContainer&) = delete;

        /// \brief Move constructor.
        InterfaceContainer(InterfaceContainer&& other);

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
        TInterface* AddInterface(TArguments&&... arguments);

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        TInterface* GetInterface();

        /// \brief Get an interface by type.
        /// \tparam TInterface Type of the interface to get.
        /// \return Returns a pointer to the requested interface. If no such interface exists returns nullptr instead.
        template <typename TInterface>
        const TInterface* GetInterface() const;

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
            AnyInterface(std::identity<TInterface>, std::identity<TConcrete>, TArguments&&... arguments);

            /// \brief Move constructor.
            AnyInterface(AnyInterface&& other);

            /// \brief No copy constructor.
            AnyInterface(const AnyInterface&) = delete;

            /// \brief No assignment operator.
            AnyInterface& operator=(const AnyInterface&) = delete;

            /// \brief Default destructor.
            ~AnyInterface() = default;

            /// \brief Check whether this interface can be statically casted to TClass.
            /// \return Returns true if this interface can be statically casted to TClass. Returns false otherwise.
            template <typename TClass>
            bool Is() const;

            /// \brief Cast the content of this interface to TClass.
            /// \return Returns a pointer to this interface, casted to TClass.
            template <typename TClass>
            TClass* As() const;

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
                HolderT(TArguments&&... arguments);

                virtual void* GetInterface() override;

                virtual const void* GetInterface() const override;

                TConcrete interface_;               ///< \brief Actual interface.
            };

            std::unique_ptr<Holder> holder_;    /// \brief Holds the concrete interface.

            std::type_index type_;              /// \brief Type of the interface stored inside the container

        };

        /// \brief Type of the allocator used for internal dynamic memory management.
        using allocator_t = std::conditional_t<std::is_same<TAllocator, void>::value, std::allocator<AnyInterface>, TAllocator>;

        std::vector<AnyInterface, allocator_t> interfaces_;             ///< \brief Interfaces registered so far.

    };

}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* INTERFACE CONTAINER :: ANY INTERFACE :: HOLDER T                     */
    /************************************************************************/

    template <typename TAllocator>
    template <typename TConcrete>
    template <typename... TArguments>
    InterfaceContainer<TAllocator>::AnyInterface::HolderT<TConcrete>::HolderT(TArguments&&... arguments)
        : interface_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TAllocator>
    template <typename TConcrete>
    void* InterfaceContainer<TAllocator>::AnyInterface::HolderT<TConcrete>::GetInterface()
    {
        return std::addressof(interface_);
    }

    template <typename TAllocator>
    template <typename TConcrete>
    const void* InterfaceContainer<TAllocator>::AnyInterface::HolderT<TConcrete>::GetInterface() const
    {
        return std::addressof(interface_);
    }

    /************************************************************************/
    /* INTERFACE CONTAINER :: ANY INTERFACE                                 */
    /************************************************************************/

    template <typename TAllocator>
    template <typename TInterface, typename TConcrete, typename... TArguments>
    InterfaceContainer<TAllocator>::AnyInterface::AnyInterface(std::identity<TInterface>, std::identity<TConcrete>, TArguments&&... arguments)
        : holder_(std::make_unique<HolderT<TConcrete>>(std::forward<TArguments>(arguments)...))
        , type_(typeid(TInterface))
    {
        static_assert(std::is_base_of<TInterface, TConcrete>::value, "TConcrete must be equal to or derive from TInterface.");
    }

    template <typename TAllocator>
    InterfaceContainer<TAllocator>::AnyInterface::AnyInterface(AnyInterface&& other)
        : holder_(std::move(other.holder_))
        , type_(std::move(other.type_))
    {

    }

    template <typename TAllocator>
    template <typename TClass>
    bool InterfaceContainer<TAllocator>::AnyInterface::Is() const
    {
        return type_ == typeid(TClass);
    }

    template <typename TAllocator>
    template <typename TClass>
    TClass* InterfaceContainer<TAllocator>::AnyInterface::As() const
    {
        SYNTROPY_ASSERT(Is<TClass>());      // Better safe than sorry.

        return reinterpret_cast<TClass*>(holder_->GetInterface());
    }

    /************************************************************************/
    /* INTERFACE CONTAINER                                                  */
    /************************************************************************/

    template <typename TAllocator>
    InterfaceContainer<TAllocator>::InterfaceContainer(InterfaceContainer&& other)
        : interfaces_(std::move(other.interfaces_))
    {

    }

    template <typename TAllocator>
    template <typename TInterface, typename TConcrete, typename... TArguments>
    TInterface* InterfaceContainer<TAllocator>::AddInterface(TArguments&&... arguments)
    {
        static_assert(std::is_base_of<TInterface, TConcrete>::value, "TConcrete must be equal to or derive from TInterface.");

        auto interface = GetInterface<TInterface>();

        if (!interface)
        {
            interfaces_.emplace_back
            (
                std::identity<TInterface>{}, 
                std::identity<TConcrete>{}, 
                std::forward<TArguments>(arguments)...
            );

            return interfaces_.back().As<TInterface>();     // Return the new interface.
        }
        else
        {
            return nullptr;                                 // Another interface was already registered.
        }
    }

    template <typename TAllocator>
    template <typename TInterface>
    TInterface* InterfaceContainer<TAllocator>::GetInterface()
    {
        auto it = std::find_if
        (
            std::begin(interfaces_),
            std::end(interfaces_),
            [](const AnyInterface& interface)
            {
                return interface.Is<TInterface>();
            }
        );

        return it != std::end(interfaces_) ?
            it->As<TInterface>() :
            nullptr;
    }

    template <typename TAllocator>
    template <typename TInterface>
    const TInterface* InterfaceContainer<TAllocator>::GetInterface() const
    {
        auto it = std::find_if
        (
            std::begin(interfaces_),
            std::end(interfaces_),
            [](const AnyInterface& interface)
            {
                return interface.Is<TInterface>();
            }
        );

        return it != std::end(interfaces_) ?
            it->As<TInterface>() :
            nullptr;
    }

}

