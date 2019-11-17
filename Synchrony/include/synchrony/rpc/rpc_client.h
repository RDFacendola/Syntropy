
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
#include "syntropy/serialization/msgpack/msgpack_stream.h"

#include "synchrony/socket/tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC CLIENT T                                                         */
    /************************************************************************/

    /// \brief Object that is used to send commands to a remote peer.
    /// \author Raffaele D. Facendola - November 2019.
    template <typename TStream>
    class RPCClientT
    {
    public:

        /// \brief Default constructor.
        RPCClientT() = default;

        /// \brief Destroy the RPC client.
        ~RPCClientT();

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

    /// \brief Default RPC client.
    using RPCClient = RPCClientT<syntropy::MsgpackStream>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TStream>
    inline RPCClientT<TStream>::~RPCClientT()
    {
        Stop();
        Join();
    }

    template <typename TStream>
    template <typename... TArguments>
    void RPCClientT<TStream>::Call(const syntropy::HashedString& name, TArguments&&... arguments)
    {
        auto stream = TStream{};

        stream << name.GetString();

        ((stream << arguments), ...);

        auto lock = std::lock_guard<std::mutex>(mutex_);

        send_buffer_ += stream.ToString();
    }

    template <typename TStream>
    inline void RPCClientT<TStream>::Start(TCPSocket& socket)
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

    template <typename TStream>
    inline void RPCClientT<TStream>::Stop()
    {
        is_running_ = false;
    }

    template <typename TStream>
    inline void RPCClientT<TStream>::Join()
    {
        if (thread_)
        {
            thread_->join();
            thread_ = nullptr;
        }
    }

    template <typename TStream>
    inline bool RPCClientT<TStream>::IsRunning() const
    {
        return is_running_;
    }

    template <typename TStream>
    void RPCClientT<TStream>::Run(TCPSocket& socket)
    {
        while (IsRunning())
        {
            auto lock = std::lock_guard<std::mutex>(mutex_);

            if (send_buffer_.size() > 0)
            {
                auto send_range = syntropy::ConstMemoryRange(send_buffer_.data(), send_buffer_.data() + send_buffer_.size());

                if (socket.SendAll(send_range))                 // Send the buffered commands.
                {
                    send_buffer_.clear();
                }
                else if (!socket.IsConnected())
                {
                    Stop();                                     // Disconnected: stop the client and return.
                }
            }
        }
    }
}
