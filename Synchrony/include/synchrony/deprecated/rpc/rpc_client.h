
/// \file rpc_client.h
/// \brief This header is part of the synchrony remote procedures call system. It contains client side interfaces for remote procedures call.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <functional>

#include "syntropy/types/label.h"
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
        void Call(const syntropy::Label& name, TArguments&&... arguments);

        /// \brief Bind a new procedure that is called whenever an error occurs.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCClientT& BindError(TProcedure&& procedure);

        /// \brief Bind a new procedure that is called whenever the server is disconnected.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCClientT& BindDisconnected(TProcedure&& procedure);

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

        /// \brief Type of an RPC server event.
        using RemoteEvent = std::function<void(void)>;

        /// \brief Type of the underlying buffer to send over
        using SendBuffer = typename TStream::TString;

        /// \brief RPC client loop.
        void Run(TCPSocket& socket);

        /// \brief Called whenever an error occurs while sending data.
        void OnError();

        /// \brief Called whenever the RPC client gets disconnected.
        void OnDisconnected();

        /// \brief Receiving thread.
        std::unique_ptr<std::thread> thread_;

        /// \brief Whether the RPC client is running.
        std::atomic_bool is_running_{ false };

        /// \brief Handlers to events that are called whenever a client error occurs.
        Vector<RemoteEvent> error_handlers_;

        /// \brief Handlers to events that are called whenever the client is disconnected.
        Vector<RemoteEvent> disconnected_handlers_;

        /// \brief Used to synchronize the sender thread.
        std::mutex mutex_;

        /// \brief Stream to send.
        SendBuffer send_buffer_;
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
    void RPCClientT<TStream>::Call(const syntropy::Label& name, TArguments&&... arguments)
    {
        auto stream = TStream{};

        stream << name.GetString();

        ((stream << arguments), ...);

        auto lock = std::lock_guard<std::mutex>(mutex_);

        send_buffer_ += stream.ToString();
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCClientT<TStream>& RPCClientT<TStream>::BindError(TProcedure&& procedure)
    {
        error_handlers_.emplace_back(std::move(procedure));

        return *this;
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCClientT<TStream>& RPCClientT<TStream>::BindDisconnected(TProcedure&& procedure)
    {
        disconnected_handlers_.emplace_back(std::move(procedure));

        return *this;
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

                if (auto send_result = socket.SendAll(send_range);
                    send_result == TCPSendResult::kOk)                              // Send the buffered commands.
                {
                    send_buffer_.clear();
                }
                else if (send_result == TCPSendResult::kDisconnected)               // Disconnected.
                {
                    OnDisconnected();
                }
                else if (send_result == TCPSendResult::kError)                      // Error.
                {
                    OnError();
                }
            }
        }
    }

    template <typename TStream>
    inline void RPCClientT<TStream>::OnError()
    {
        for (auto&& error_handler : error_handlers_)
        {
            error_handler();
        }
    }

    template <typename TStream>
    inline void RPCClientT<TStream>::OnDisconnected()
    {
        for (auto&& disconnected_handler : disconnected_handlers_)
        {
            disconnected_handler();
        }

        Stop();
    }

}
