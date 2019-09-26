#include "synchrony/socket/tcp.h"

#include "synchrony/platform/windows_tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* TCP                                                                  */
    /************************************************************************/

    std::unique_ptr<TCPSocket> TCP::Connect(const NetworkEndpoint& server)
    {
        return PlatformTCP::Connect(server);
    }

    std::unique_ptr<TCPServer> TCP::StartServer(const NetworkEndpoint& listen_interface, std::int32_t backlog)
    {
        return PlatformTCP::StartServer(listen_interface, backlog);
    }

}
