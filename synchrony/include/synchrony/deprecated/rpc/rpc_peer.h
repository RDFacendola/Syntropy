
/// \file rpc_peer.h
/// \brief This header is part of the synchrony remote procedures call system. It contains interfaces that can be used both to receive and send remote procedure calls.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <memory>

#include "syntropy/serialization/msgpack/msgpack_stream.h"

#include "synchrony/socket/tcp.h"

#include "synchrony/rpc/rpc_server.h"
#include "synchrony/rpc/rpc_client.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC PEER T                                                           */
    /************************************************************************/

    /// \brief Represents an endpoint that can be used both to send and receive remote commands.
    /// \author Raffaele D. Facendola - November 2019
    template <typename TStream>
    class RPCPeerT
    {
    public:

        /// \brief Create a new RPC peer.
        RPCPeerT(std::unique_ptr<synchrony::TCPSocket> socket);

        /// \brief Destroy the RPC peer.
        ~RPCPeerT() = default;

        /// \brief Start the RPC peer.
        void Start();

        /// \brief Stop the RPC peer.
        void Stop();

        /// \brief Wait until the RPC peer gets stopped.
        void Join();

        /// \brief Check whether the RPC peer is running.
        bool IsRunning() const;

        /// \brief Get the underlying socket.
        const synchrony::TCPSocket& GetSocket() const;

        /// \brief Bind a new procedure to the receiver by name.
        /// If another procedure is registered with the same name the behavior of this function is undefined.
        /// \param name Name of the procedure to bind.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCPeerT& Bind(const Syntropy::Label& name, TProcedure&& procedure);

        /// \brief Call a remote procedure on the remote peer.
        /// If the transceiver is not running the behavior of this method is undefined.
        /// \param name Procedure to call.
        /// \param arguments Arguments passed to the function.
        template <typename... TArguments>
        void Call(const Syntropy::Label& name, TArguments&&... arguments);

        /// \brief Bind a new procedure that is called whenever an error occurs.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCPeerT& BindError(TProcedure&& procedure);

        /// \brief Bind a new procedure that is called whenever the peer is disconnected.
        /// \param procedure Procedure to bind.
        /// \return Returns a reference to this.
        template <typename TProcedure>
        RPCPeerT& BindDisconnected(TProcedure&& procedure);

        /// \brief Set the rate at which data are received from the network.
        void SetReceiveRate(std::chrono::milliseconds receive_rate);

        /// \brief Set the size of the buffer used to receive data from the network.
        void SetReceiveSize(Syntropy::Bytes receive_size);

    private:

        /// \brief Underlying socket.
        std::unique_ptr<synchrony::TCPSocket> socket_;

        /// \brief RPC server used to receive commands.
        synchrony::RPCServerT<TStream> rpc_server_;

        /// \brief RPC client used to transmit commands.
        synchrony::RPCClientT<TStream> rpc_client_;
    };

    /// \brief Default RPC peer.
    using RPCPeer = RPCPeerT<Syntropy::MsgpackStream>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // RPCPeer.

    template <typename TStream>
    inline RPCPeerT<TStream>::RPCPeerT(std::unique_ptr<synchrony::TCPSocket> socket)
        : socket_(std::move(socket))
    {

    }

    template <typename TStream>
    inline void RPCPeerT<TStream>::Start()
    {
        rpc_server_.Start(*socket_);
        rpc_client_.Start(*socket_);
    }

    template <typename TStream>
    inline void RPCPeerT<TStream>::Stop()
    {
        rpc_server_.Stop();
        rpc_client_.Stop();
    }

    template <typename TStream>
    inline void RPCPeerT<TStream>::Join()
    {
        rpc_server_.Join();
        rpc_client_.Join();
    }

    template <typename TStream>
    inline bool RPCPeerT<TStream>::IsRunning() const
    {
        return rpc_server_.IsRunning() && rpc_client_.IsRunning();
    }

    template <typename TStream>
    inline const synchrony::TCPSocket& RPCPeerT<TStream>::GetSocket() const
    {
        return *socket_;
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCPeerT<TStream>& RPCPeerT<TStream>::Bind(const Syntropy::Label& name, TProcedure&& procedure)
    {
        rpc_server_.Bind(name, std::forward<TProcedure>(procedure));

        return *this;
    }

    template <typename TStream>
    template <typename... TArguments>
    inline void RPCPeerT<TStream>::Call(const Syntropy::Label& name, TArguments&&... arguments)
    {
        rpc_client_.Call(name, std::forward<TArguments>(arguments)...);
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCPeerT<TStream>& RPCPeerT<TStream>::BindError(TProcedure&& procedure)
    {
        rpc_server_.BindError(procedure);
        rpc_client_.BindError(procedure);

        return *this;
    }

    template <typename TStream>
    template <typename TProcedure>
    inline RPCPeerT<TStream>& RPCPeerT<TStream>::BindDisconnected(TProcedure&& procedure)
    {
        rpc_server_.BindDisconnected(procedure);
        rpc_client_.BindDisconnected(procedure);

        return *this;
    }

    template <typename TStream>
    inline void RPCPeerT<TStream>::SetReceiveRate(std::chrono::milliseconds receive_rate)
    {
        rpc_server_.SetReceiveRate(receive_rate);
    }

    template <typename TStream>
    inline void RPCPeerT<TStream>::SetReceiveSize(Syntropy::Bytes receive_size)
    {
        rpc_server_.SetReceiveSize(receive_size);
    }

}