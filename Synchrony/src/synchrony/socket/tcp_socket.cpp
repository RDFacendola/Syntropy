#include "synchrony/socket/tcp_socket.h"

#include "synchrony/platform/windows_socket.h"

namespace synchrony
{
    /************************************************************************/
    /* TCP                                                                  */
    /************************************************************************/

    std::unique_ptr<TCPSocket> TCP::Connect(const NetworkEndpoint& server)
    {
        return PlatformTCP::Connect(server);
    }

    std::unique_ptr<TCPServer> TCP::StartServer(const NetworkEndpoint& server, std::int32_t backlog)
    {
        return PlatformTCP::StartServer(server, backlog);
    }

}
