
/// \file event_details.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains implementation details of events and listeners.
///
/// \author Raffaele D. Facendola - 2017.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/foundation/unique_ptr.h"

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* EVENT HANDLER                                                        */
    /************************************************************************/

    /// \brief Represents the set of events a listener refers to.
    /// \author Raffaele D. Facendola - January 2021.
    class EventHandler
    {
        friend class EventChain;

    public:

        /// \brief Default constructor.
        EventHandler() noexcept = default;

        /// \brief No copy-constructor.
        EventHandler(Immutable<EventHandler> rhs) noexcept = delete;

        /// \brief No move-constructor.
        EventHandler(Movable<EventHandler> rhs) noexcept = delete;

        /// \brief Virtual destructor.
        virtual
        ~EventHandler() noexcept;

        /// \brief No copy-assignment operator.
        Mutable<EventHandler>
        operator=(Immutable<EventHandler> rhs) noexcept = delete;

        /// \brief No move-assignment operator.
        Mutable<EventHandler>
        operator=(Movable<EventHandler> rhs) noexcept = delete;

        /// \brief Destroy this handler.
        void Destroy() noexcept;

    private:

        /// \brief Clone this instance, linking it to the same listener chain.
        [[nodiscard]] virtual RWUniquePtr<EventHandler>
        CloneListener() noexcept;

        /// \brief Next event.
        RWUniquePtr<EventHandler> next_event_;

        /// \brief Previous event.
        RWPtr<EventHandler> previous_event_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER HANDLER                                                     */
    /************************************************************************/

    /// \brief Represents the set of listeners subscribed to an event.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename... TArguments>
    class ListenerHandler : public EventHandler
    {
        template <typename... UArguments>
        friend class ListenerChain;

    public:

        /// \brief Default constructor.
        ListenerHandler() noexcept = default;

        /// \brief No copy-constructor.
        ListenerHandler(Immutable<ListenerHandler> rhs) noexcept = delete;

        /// \brief No move-constructor.
        ListenerHandler(Movable<ListenerHandler> rhs) noexcept = delete;

        /// \brief Virtual destructor.
        virtual ~ListenerHandler() noexcept;

        /// \brief No copy-assignment operator.
        Mutable<ListenerHandler>
        operator=(Immutable<ListenerHandler> rhs) noexcept = delete;

        /// \brief No move-assignment operator.
        Mutable<ListenerHandler>
        operator=(Movable<ListenerHandler> rhs) noexcept = delete;

    private:

        [[nodiscard]] virtual RWUniquePtr<EventHandler>
        CloneListener() noexcept override;

        /// \brief Clone this instance.
        [[nodiscard]] virtual RWUniquePtr<ListenerHandler<TArguments...>>
        Clone() const noexcept;

        /// \brief Notify the delegate.
        virtual void
        Notify(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Next listener.
        RWPtr<ListenerHandler> next_listener_{ nullptr };

        /// \brief Previous listener.
        RWPtr<ListenerHandler> previous_listener_{ nullptr };

    };

    /************************************************************************/
    /* LISTENER HANDLER DELEGATE                                            */
    /************************************************************************/

    /// \brief Represents a delegate called whenever an event is signaled
    ///        to a listener.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TDelegate, typename... TArguments>
    class ListenerHandlerDelegate : public ListenerHandler<TArguments...>
    {
    public:

        /// \brief Create a new delegate bound to a functor.
        template <typename UDelegate>
        ListenerHandlerDelegate(Forwarding<UDelegate> delegate);

        /// \brief No copy-constructor.
        ListenerHandlerDelegate(Immutable<ListenerHandlerDelegate> rhs)
        noexcept = delete;

        /// \brief No move-constructor.
        ListenerHandlerDelegate(Movable<ListenerHandlerDelegate> rhs)
        noexcept = delete;

        /// \brief Default virtual destructor.
        virtual
        ~ListenerHandlerDelegate() noexcept = default;

        /// \brief No copy-assignment operator.
        Mutable<ListenerHandlerDelegate>
        operator=(Immutable<ListenerHandlerDelegate> rhs) noexcept = delete;

        /// \brief No move-assignment operator.
        Mutable<ListenerHandlerDelegate>
        operator=(Movable<ListenerHandlerDelegate> rhs) noexcept = delete;

    private:

        [[nodiscard]] virtual RWUniquePtr<ListenerHandler<TArguments...>>
        Clone() const noexcept override;

        virtual void
        Notify(Immutable<TArguments>... arguments) const noexcept override;

        /// \brief Handler function.
        TDelegate delegate_;

    };

    /************************************************************************/
    /* EVENT CHAIN                                                          */
    /************************************************************************/

    /// \brief List of events a listener is subscribed to.
    /// \author Raffaele D. Facendola - January 2021.
    class EventChain
    {
    public:

        /// \brief Default constructor.
        EventChain() noexcept = default;

        /// \brief Create a new event chain with a single event.
        EventChain(RWUniquePtr<EventHandler> event) noexcept;

        /// \brief Copy-constructor.
        EventChain(Immutable<EventChain> rhs) noexcept;

        /// \brief Move-constructor.
        EventChain(Movable<EventChain> rhs) noexcept;

        /// \brief Destructor.
        ~EventChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<EventChain>
        operator=(Immutable<EventChain> rhs) noexcept;

        /// \brief Move-assignment operator.
        Mutable<EventChain>
        operator=(Movable<EventChain> rhs) noexcept;

        /// \brief Append an event chain to this one's,
        ///        transferring its ownership.
        Mutable<EventChain>
        operator+=(Movable<EventChain> rhs) noexcept;

    private:

        /// \brief Link two event chains together and return the event chain
        ///        that was unlinked as a result.
        ///
        /// The behavior of this method is undefined if rhs is linked to an
        /// existing event chain.
        static RWUniquePtr<EventHandler>
        Link(Mutable<EventHandler> lhs,
             RWUniquePtr<EventHandler> rhs) noexcept;

        /// \brief Access the last event accessible from lhs.
        [[nodiscard]] static Mutable<EventHandler>
        GetTail(Mutable<EventHandler> lhs) noexcept;

        /// \brief Dummy event linked to each event a listener
        ///        is subscribed to.
        EventHandler events_;

    };

    /************************************************************************/
    /* LISTENER CHAIN                                                       */
    /************************************************************************/

    /// \brief List of listeners subscribed to an event.
    /// \author Raffaele D. Facendola - January 2021.
    template <typename... TArguments>
    class ListenerChain
    {
    public:

        /// \brief Default constructor.
        ListenerChain() noexcept = default;

        /// \brief Copy-constructor.
        ListenerChain(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Move-constructor.
        ListenerChain(Movable<ListenerChain> rhs) noexcept;

        /// \brief Destructor.
        ~ListenerChain() noexcept;

        /// \brief Copy-assignment operator.
        Mutable<ListenerChain>
        operator=(Immutable<ListenerChain> rhs) noexcept;

        /// \brief Move-assignment operator.
        Mutable<ListenerChain>
        operator=(Movable<ListenerChain> rhs) noexcept;

        /// \brief Notify the listener chain starting from this element.
        void
        operator()(Immutable<TArguments>... arguments) const noexcept;

        /// \brief Emplace a new listener to the chain and return the event
        ///        chain associated to it.
        template <typename TDelegate>
        [[nodiscard]] EventChain
        Emplace(Forwarding<TDelegate> delegate) noexcept;

    private:

        /// \brief Link two listener chains together and return the listener
        ///        chain that was unlinked as a result.
        ///
        /// The behavior of this method is undefined if rhs is linked to an
        /// existing listener chain.
        static RWPtr<ListenerHandler<TArguments...>>
        Link(Mutable<ListenerHandler<TArguments...>> lhs,
             RWPtr<ListenerHandler<TArguments...>> rhs) noexcept;

        /// \brief Access the last listener accessible from lhs.
        [[nodiscard]] static Mutable<ListenerHandler<TArguments...>>
        GetTail(Mutable<ListenerHandler<TArguments...>> lhs) noexcept;

        /// \brief Destroy all listeners accessible from lhs.
        static void
        Destroy(RWPtr<ListenerHandler<TArguments...>> lhs) noexcept;

        /// \brief Dummy listener linked to each listener subscribed to
        ///        an event.
        ListenerHandler<TArguments...> listeners_;

    };

}

// ===========================================================================

#include "event.details.inl"

// ===========================================================================
