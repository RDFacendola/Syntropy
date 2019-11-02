
/// \file rpc_server.h
/// \brief This header is part of the synchrony remote procedures call system. It contains server side interfaces for remote procedures call.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <memory>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <functional>
#include <sstream>
#include <tuple>

#include "syntropy/type_traits.h"
#include "syntropy/patterns/tuple.h"

#include "syntropy/containers/hashed_string.h"

#include "synchrony/socket/tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC SERVER                                                           */
    /************************************************************************/

    /// \brief Exposes functions that can be called from a remote peer.
    /// \author Raffaele D. Facendola - October 2019.
    class RPCServer
    {
    public:

        /// \brief Default constructor.
        RPCServer() = default;

        /// \brief Destroy the RPC server.
        ~RPCServer();

        /// \brief Bind a new procedure to the server by name.
        /// If another procedure is registered with the same name the behavior of this function is undefined.
        /// \param name Name of the procedure to bind.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCServer& Bind(const syntropy::HashedString& name, TProcedure&& procedure);

        /// \brief Start the RPC server asynchronously.
        void Start(TCPSocket& socket);

        /// \brief Stop the RPC server.
        /// If the server isn't running this method does nothing.
        void Stop();

        /// \brief Wait until the RPC server is stopped.
        void Join();

        /// \brief Check whether the RPC server is running.
        bool IsRunning() const;

    private:

        /// \brief Stored routine.
        using RemoteProcedure = std::function<void(std::stringstream& stream)>;

        /// \brief RPC server loop.
        void Run(TCPSocket& socket);

        /// \brief Deserialize a procedure encoded in a remote stream.
        void DeserializeStream(std::string& stream);

        /// \brief Deserialize a single procedure encoded in a stream.
        /// \return 
        bool DeserializeProcedure(std::stringstream& stream);

        /// \brief Receiving thread.
        std::unique_ptr<std::thread> thread_;

        /// \brief Whether the RPC server is running.
        std::atomic_bool is_running_{ false };

        /// \brief Procedures bound to the server.
        std::unordered_map<syntropy::HashedString, RemoteProcedure> procedures_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline RPCServer::~RPCServer()
    {
        Stop();
        Join();
    }

    template <typename TProcedure>
    inline RPCServer& RPCServer::Bind(const syntropy::HashedString& name, TProcedure&& procedure)
    {
        procedures_[name] = [this, procedure = std::move(procedure)](std::stringstream& stream)
        {
            // Deserialize the procedure arguments from the stream.

            auto arguments = syntropy::function_arguments_t<TProcedure>{};

            auto deserialize_argument = [&stream](auto&& argument)
            {
                if (!stream.fail())
                {
                    stream >> argument;
                }
            };

            syntropy::LockstepApply(deserialize_argument, arguments);

            // Call the procedure.

            if (!stream.fail())
            {
                std::apply(procedure, std::move(arguments));
            }
        };

        return *this;
    }

    inline void RPCServer::Start(TCPSocket& socket)
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

    inline void RPCServer::Stop()
    {
        is_running_ = false;
    }

    inline void RPCServer::Join()
    {
        if (thread_)
        {
            thread_->join();
            thread_ = nullptr;
        }
    }

    inline bool RPCServer::IsRunning() const
    {
        return is_running_;
    }

}
