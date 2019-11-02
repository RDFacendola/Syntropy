#include "synchrony/rpc/rpc_server.h"

#include <array>
#include <vector>

#include "syntropy/memory/memory_range.h"

namespace synchrony
{
    /************************************************************************/
    /* RPC SERVER                                                           */
    /************************************************************************/

    void RPCServer::Run(TCPSocket& socket)
    {
        static const auto kBufferSize = 1024;

        auto receive_buffer = std::array<char, kBufferSize>{};
        auto receive_stream = std::string{};

        while (IsRunning())
        {
            auto receive_range = syntropy::MakeMemoryRange(receive_buffer);

            if (socket.Receive(receive_range))                  // Receive a chunk of data.
            {
                receive_stream.reserve(receive_stream.capacity() + std::size_t(receive_range.GetSize()));

                std::copy(receive_range.Begin().As<std::int8_t>(), receive_range.End().As<std::int8_t>(), std::back_inserter(receive_stream));

                DeserializeStream(receive_stream);              // Attempt to deserialize a remote procedure.
            }
            else if (!socket.IsConnected())
            {
                Stop();                                         // Disconnected: stop the server and return.
            }
        }
    }

    void RPCServer::DeserializeStream(std::string& stream)
    {
        auto reader = std::stringstream{ stream };

        auto reader_position = reader.tellg();

        for (; DeserializeProcedure(reader); reader_position = reader.tellg());

        stream.erase(begin(stream), reader_position >= 0 ? begin(stream) + reader_position : end(stream));
    }

    bool RPCServer::DeserializeProcedure(std::stringstream& stream)
    {
        auto procedure_name = std::string{};

        stream >> procedure_name;

        auto procedure_it = procedures_.find(procedure_name);

        if (procedure_it != end(procedures_))
        {
            procedure_it->second(stream);
        }

        auto more = stream.eof() && (stream.fail() || (procedure_it == end(procedures_)));

        stream.clear();

        return !more;
    }

}
