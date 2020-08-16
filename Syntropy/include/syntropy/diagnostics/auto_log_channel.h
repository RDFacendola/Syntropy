
/// \file auto_log_channel.h
/// \brief This header is part of the Syntropy diagnostic module. It contains definition for self-registering log channels.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <algorithm>

#include "syntropy/core/smart_pointers.h"

#include "syntropy/diagnostics/severity.h"
#include "syntropy/diagnostics/verbosity.h"
#include "syntropy/diagnostics/log_event.h"

#include "syntropy/core/vector.h"

namespace Syntropy
{
    /************************************************************************/
    /* AUTO LOG CHANNEL                                                     */
    /************************************************************************/

    /// \brief Represents the interface for a self-registering log channel.
    /// \author Raffaele D. Facendola - May 2020.
    class AutoLogChannel
    {
    public:

        /// \ brief Apply a function to each self-registering log channel.
        template <typename TFunction>
        static void ForEach(TFunction&& function);

        /// \brief No copy-constructor.
        AutoLogChannel(const AutoLogChannel&) = delete;

        /// \brief No move-constructor.
        AutoLogChannel(AutoLogChannel&&) = delete;

        /// \brief No copy-assignment.
        AutoLogChannel& operator=(const AutoLogChannel&) = delete;

        /// \brief No move-assignment.
        AutoLogChannel& operator=(AutoLogChannel&&) = delete;

        /// \brief Default destructor.
        virtual ~AutoLogChannel() = default;

        /// \brief Send a log event to the channel.
        virtual void Send(const LogEvent& log_event);

        /// \brief Flush the channel, committing any cached state.
        virtual void Flush();

    protected:

        /// \brief Create a new self-registering log channels.
        /// Protected to prevent direct instantiation.
        AutoLogChannel();

    private:

        /// \brief Tag used to construct the head of the linked list.
        struct HeadTag {};

        /// \brief Get the first element in a linked list to which every other self-registering log channel is linked to.
        static AutoLogChannel& GetLinkedList();

        /// \brief Create the head of the log channel list.
        AutoLogChannel(HeadTag);

        /// \brief Link this channel to the others and return the next log channel after this one.
        RWPointer<AutoLogChannel> LinkBefore();

        /// \brief Next log channel.
        RWPointer<AutoLogChannel> next_log_channel_{ nullptr };

    };

    /************************************************************************/
    /* AUTO LOG CHANNEL T <TLOG CHANNEL>                                    */
    /************************************************************************/

    /// \brief Represents a self-registering log channel wrapping a concrete log channel class.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TLogChannel>
    class AutoLogChannelT : public AutoLogChannel
    {
    public:

        /// \brief Create a new log channel forwarding arguments to the the underlying channel.
        template <typename... TArguments>
        AutoLogChannelT(TArguments&&... arguments);

        /// \brief No copy-constructor.
        AutoLogChannelT(const AutoLogChannelT&) = delete;

        /// \brief No move-constructor.
        AutoLogChannelT(AutoLogChannelT&&) = delete;

        /// \brief No copy-assignment.
        AutoLogChannelT& operator=(const AutoLogChannelT&) = delete;

        /// \brief No move-assignment.
        AutoLogChannelT& operator=(AutoLogChannelT&&) = delete;

        /// \brief Default destructor.
        virtual ~AutoLogChannelT() = default;

        virtual void Send(const LogEvent& log_event) override;

        virtual void Flush() override;

    private:

        /// \brief Underlying log channel.
        TLogChannel log_channel_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering log channel.
    /// \usage const auto auto_log_channel = MakeAutoLogChannel<ChannelType>(arg0, arg1, ...).
    template <typename TLogChannel, typename... TArguments>
    AutoLogChannelT<TLogChannel> MakeAutoLogChannel(TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoLogChannel.

    template <typename TFunction>
    inline void AutoLogChannel::ForEach(TFunction&& function)
    {
        // Skip the very first log channel as it is the sentinel to which every other log channel is linked to.

        for (auto log_channel = GetLinkedList().next_log_channel_; log_channel; log_channel = log_channel->next_log_channel_)
        {
            function(*log_channel);
        }
    }

    inline AutoLogChannel::AutoLogChannel()
        : next_log_channel_(LinkBefore())
    {

    }

    inline AutoLogChannel::AutoLogChannel(HeadTag)
    {

    }

    inline void AutoLogChannel::Send(const LogEvent& log_event)
    {

    }

    inline void AutoLogChannel::Flush()
    {

    }

    inline AutoLogChannel& AutoLogChannel::GetLinkedList()
    {
        static auto linked_list = AutoLogChannel{ HeadTag{} };      // Default constructor will call this function recursively while constructing the head, leading to a deadlock.

        return linked_list;
    }

    inline RWPointer<AutoLogChannel> AutoLogChannel::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next = linked_list.next_log_channel_;

        linked_list.next_log_channel_ = this;

        return next;
    }

    // AutoLogChannelT<TLogChannel>.

    template <typename TLogChannel>
    template <typename... TArguments>
    inline AutoLogChannelT<TLogChannel>::AutoLogChannelT(TArguments&&... arguments)
        : log_channel_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TLogChannel>
    inline void AutoLogChannelT<TLogChannel>::Send(const LogEvent& log_event)
    {
        log_channel_.Send(log_event);
    }

    template <typename TLogChannel>
    inline void AutoLogChannelT<TLogChannel>::Flush()
    {
        log_channel_.Flush();
    }

    // Non-member functions.

    template <typename TLogChannel, typename... TArguments>
    inline AutoLogChannelT<TLogChannel> MakeAutoLogChannel(TArguments&&... arguments)
    {
        return AutoLogChannelT<TLogChannel>(std::forward<TArguments>(arguments)...);
    }

}
