#include "synchrony/socket/udp.h"

#include "synchrony/platform/windows_udp.h"

namespace synchrony
{
    /************************************************************************/
    /* UDP                                                                  */
    /************************************************************************/

    std::unique_ptr<UDPSocket> UDP::CreatePeer(const NetworkEndpoint& local)
    {
        return PlatformUDP::CreatePeer(local);
    }

    std::unique_ptr<UDPChannel> UDP::CreatePeer(const NetworkEndpoint& local, const NetworkEndpoint& remote)
    {
        return PlatformUDP::CreatePeer(local, remote);
    }
}
