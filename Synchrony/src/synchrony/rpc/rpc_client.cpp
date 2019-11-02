#include "synchrony/rpc/rpc_client.h"

#include <array>
#include <string>

namespace synchrony
{
    /************************************************************************/
    /* RPC CLIENT                                                           */
    /************************************************************************/

    void RPCClient::Run(TCPSocket& socket)
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
