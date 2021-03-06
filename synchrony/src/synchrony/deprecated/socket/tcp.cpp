#include "synchrony/socket/tcp.h"

#include "synchrony/platform/windows_tcp.h"

namespace synchrony
{
    /************************************************************************/
    /* TCP                                                                  */
    /************************************************************************/

    std::unique_ptr<TCPSocket> TCP::Connect(const NetworkEndpoint& local, const NetworkEndpoint& remote)
    {
        return PlatformTCP::Connect(local, remote);
    }

    std::unique_ptr<TCPServer> TCP::StartServer(const NetworkEndpoint& local, std::int32_t backlog)
    {
        return PlatformTCP::StartServer(local, backlog);
    }

}
