
/// \file rpc_client.h
/// \brief This header is part of the synchrony remote procedures call system. It contains client side interfaces for remote procedures call.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <sstream>
#include <mutex>

#include "syntropy/containers/hashed_string.h"

#include "synchrony/socket/tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC CLIENT                                                           */
    /************************************************************************/

    /// \brief Object that is used to send commands to a remote peer.
    /// \author Raffaele D. Facendola - November 2019.
    class RPCClient
    {
    public:

        /// \brief Default constructor.
        RPCClient() = default;

        /// \brief Destroy the RPC client.
        ~RPCClient();

        /// \brief Call a remote procedure on the server.
        /// If the client is not running the behavior of this method is undefined.
        /// \param name Procedure to call.
        /// \param arguments Arguments passed to the function.
        template <typename... TArguments>
        void Call(const syntropy::HashedString& name, TArguments&&... arguments);

        /// \brief Start the RPC client asynchronously.
        void Start(TCPSocket& socket);

        /// \brief Stop the RPC client.
        /// If the client isn't running this method does nothing.
        void Stop();

        /// \brief Wait until the RPC client is stopped.
        void Join();

        /// \brief Check whether the RPC client is running.
        bool IsRunning() const;

    private:

        /// \brief RPC client loop.
        void Run(TCPSocket& socket);

        /// \brief Receiving thread.
        std::unique_ptr<std::thread> thread_;

        /// \brief Whether the RPC server is running.
        std::atomic_bool is_running_{ false };

        /// \brief Used to synchronize the sender thread.
        std::mutex mutex_;

        /// \brief Stream to send.
        std::string send_buffer_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline RPCClient::~RPCClient()
    {
        Stop();
        Join();
    }

    template <typename... TArguments>
    void RPCClient::Call(const syntropy::HashedString& name, TArguments&&... arguments)
    {
        auto stream = std::stringstream{};

        stream << name << " ";

        ((stream << arguments, stream << " "), ...);

        auto lock = std::lock_guard<std::mutex>(mutex_);

        send_buffer_ += stream.str();
    }

    inline void RPCClient::Start(TCPSocket& socket)
    {
        bool expected = false;

        if (is_running_.compare_exchange_weak(expected, true))
        {
            thread_ = std::make_unique<std::thread>([this, &socket]() mutable
            {
                Run(socket);
            });
        }
    }

    inline void RPCClient::Stop()
    {
        is_running_ = false;
    }

    inline void RPCClient::Join()
    {
        if (thread_)
        {
            thread_->join();
            thread_ = nullptr;
        }
    }

    inline bool RPCClient::IsRunning() const
    {
        return is_running_;
    }

}
