#include "synchrony/socket/udp_socket.h"

#include "synchrony/platform/windows_socket.h"

namespace synchrony
{
    /************************************************************************/
    /* UDP                                                                  */
    /************************************************************************/

    std::unique_ptr<UDPSocket> UDP::CreatePeer(const NetworkEndpoint& local_address)
    {
        return PlatformUDP::CreatePeer(local_address);
    }

    std::unique_ptr<UDPChannel> UDP::CreatePeer(const NetworkEndpoint& local_address, const NetworkEndpoint& remote_address)
    {
        return PlatformUDP::CreatePeer(local_address, remote_address);
    }

}
