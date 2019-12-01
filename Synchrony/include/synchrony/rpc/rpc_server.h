
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
#include <tuple>
#include <chrono>

#include "syntropy/type_traits.h"
#include "syntropy/patterns/tuple.h"
#include "syntropy/serialization/msgpack/msgpack_stream.h"
#include "syntropy/containers/hashed_string.h"
#include "syntropy/memory/bytes.h"

#include "synchrony/socket/tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC SERVER T                                                         */
    /************************************************************************/

    /// \brief Exposes functions that can be called from a remote peer.
    /// \author Raffaele D. Facendola - October 2019.
    template <typename TStream>
    class RPCServerT
    {
    public:

        /// \brief Default constructor.
        RPCServerT() = default;

        /// \brief Destroy the RPC server.
        ~RPCServerT();

        /// \brief Bind a new procedure to the server by name.
        /// If another procedure is registered with the same name the behavior of this function is undefined.
        /// \param name Name of the procedure to bind.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCServerT& Bind(const syntropy::HashedString& name, TProcedure&& procedure);

        /// \brief Bind a new procedure that is called whenever an error occurs.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCServerT& BindError(TProcedure&& procedure);

        /// \brief Bind a new procedure that is called whenever the server is disconnected.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCServerT& BindDisconnected(TProcedure&& procedure);

        /// \brief Start the RPC server asynchronously.
        void Start(TCPSocket& socket);

        /// \brief Stop the RPC server.
        /// If the server isn't running this method does nothing.
        void Stop();

        /// \brief Wait until the RPC server is stopped.
        void Join();

        /// \brief Check whether the RPC server is running.
        bool IsRunning() const;

        /// \brief Set the rate at which data are received from the network.
        void SetReceiveRate(std::chrono::milliseconds receive_rate);

        /// \brief Set the size of the buffer used to receive data from the network.
        void SetReceiveSize(syntropy::Bytes receive_size);

    private:

        /// \brief Type of a remote procedure.
        using RemoteProcedure = std::function<void(TStream& stream)>;

        /// \brief Type of an RPC server event.
        using RemoteEvent = std::function<void(void)>;

        /// \brief Type of the buffer received from the network.
        using ReceiveBuffer = typename TStream::TString;

        /// \brief RPC server loop.
        void Run(TCPSocket& socket);

        /// \brief Deserialize a procedure encoded in a remote stream.
        void DeserializeStream(ReceiveBuffer& stream);

        /// \brief Deserialize a single procedure encoded in a stream.
        /// \return Returns true if there are data yet to process inside the stream, returns false otherwise.
        bool DeserializeProcedure(TStream& stream);

        /// \brief Called whenever an error occurs while receiving remote data.
        void OnError();

        /// \brief Called whenever the RPC server gets disconnected.
        void OnDisconnected();

        /// \brief Receiving thread.
        std::unique_ptr<std::thread> thread_;

        /// \brief Whether the RPC server is running.
        std::atomic_bool is_running_{ false };

        /// \brief Procedures bound to the server.
        std::unordered_map<syntropy::HashedString, RemoteProcedure> procedures_;

        /// \brief Handlers to events that are called whenever a server error occurs.
        std::vector<RemoteEvent> error_handlers_;

        /// \brief Handlers to events that are called whenever the server is disconnected.
        std::vector<RemoteEvent> disconnected_handlers_;

        /// \brief Rate at which data are received from the network.
        std::chrono::milliseconds receive_rate_ = std::chrono::milliseconds{ 1000 };

        /// \brief Size of the buffer used to receive data from the network.
        syntropy::Bytes receive_size_ = syntropy::Bytes{ 1024 };
    };

    /// \brief Default command parser.
    using RPCServer = RPCServerT<syntropy::MsgpackStream>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TStream>
    inline RPCServerT<TStream>::~RPCServerT()
    {
        Stop();
        Join();
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCServerT<TStream>& RPCServerT<TStream>::Bind(const syntropy::HashedString& name, TProcedure&& procedure)
    {
        procedures_[name] = [this, procedure = std::move(procedure)](TStream& stream)
        {
            // Deserialize the procedure arguments from the stream.

            auto arguments = syntropy::function_arguments_t<TProcedure>{};

            auto deserialize_argument = [&stream](auto&& argument)
            {
                stream >> argument;
            };

            syntropy::LockstepApply(deserialize_argument, arguments);

            // Call the procedure.

            if (!stream.IsFail())
            {
                std::apply(procedure, std::move(arguments));
            }
        };

        return *this;
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCServerT<TStream>& RPCServerT<TStream>::BindError(TProcedure&& procedure)
    {
        error_handlers_.emplace_back(std::move(procedure));

        return *this;
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCServerT<TStream>& RPCServerT<TStream>::BindDisconnected(TProcedure&& procedure)
    {
        disconnected_handlers_.emplace_back(std::move(procedure));

        return *this;
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::Start(TCPSocket& socket)
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
    inline void RPCServerT<TStream>::Stop()
    {
        is_running_ = false;
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::Join()
    {
        if (thread_)
        {
            thread_->join();
            thread_ = nullptr;
        }
    }

    template <typename TStream>
    inline bool RPCServerT<TStream>::IsRunning() const
    {
        return is_running_;
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::SetReceiveRate(std::chrono::milliseconds receive_rate)
    {
        receive_rate_ = receive_rate;
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::SetReceiveSize(syntropy::Bytes receive_size)
    {
        receive_size_ = receive_size;
    }

    template <typename TStream>
    void RPCServerT<TStream>::Run(TCPSocket& socket)
    {
        using ReceiveChar = typename ReceiveBuffer::value_type;

        auto receive_buffer = ReceiveBuffer{};
        auto receive_stream = ReceiveBuffer{};

        while (IsRunning())
        {
            receive_buffer.resize(std::size_t(receive_size_));

            auto receive_range = syntropy::MemoryRange(receive_buffer.data(), receive_buffer.data() + std::size_t(receive_size_));

            if (auto receive_result = socket.Receive(receive_range, receive_rate_);
                receive_result == TCPReceiveResult::kOk)                                    // Receive a chunk of data.
            {
                receive_stream.reserve(receive_stream.capacity() + std::size_t(receive_range.GetSize()));

                std::copy(receive_range.Begin().As<ReceiveChar>(), receive_range.End().As<ReceiveChar>(), std::back_inserter(receive_stream));

                DeserializeStream(receive_stream);                                          // Attempt to deserialize a remote procedure.
            }
            else if (receive_result == TCPReceiveResult::kDisconnected)                     // Disconnected.
            {
                OnDisconnected();
            }
            else if (receive_result == TCPReceiveResult::kError)                             // Error.
            {
                OnError();
            }
        }
    }

    template <typename TStream>
    void RPCServerT<TStream>::DeserializeStream(ReceiveBuffer& stream)
    {
        auto reader = TStream{ stream };

        auto reader_position = reader.GetReadPosition();

        for (; DeserializeProcedure(reader); reader_position = reader.GetReadPosition());

        stream.erase(begin(stream), begin(stream) + reader_position);
    }

    template <typename TStream>
    bool RPCServerT<TStream>::DeserializeProcedure(TStream& stream)
    {
        auto procedure_name = std::string{};

        stream >> procedure_name;

        auto procedure_it = procedures_.find(procedure_name);

        if (procedure_it != end(procedures_))
        {
            procedure_it->second(stream);
        }

        auto more = stream.IsEndOfStream() && (stream.IsFail() || (procedure_it == end(procedures_)));

        stream.Recover();

        return !more;
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::OnError()
    {
        for (auto&& error_handler : error_handlers_)
        {
            error_handler();
        }
    }

    template <typename TStream>
    inline void RPCServerT<TStream>::OnDisconnected()
    {
        for (auto&& disconnected_handler : disconnected_handlers_)
        {
            disconnected_handler();
        }

        Stop();
    }
}
