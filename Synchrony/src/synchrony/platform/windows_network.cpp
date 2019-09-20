#include "synchrony/platform/windows_network.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    bool WindowsNetwork::Startup()
    {
        static const auto WSAVersion = MAKEWORD(2, 2);

        WSADATA wsa_data;

        return WSAStartup(WSAVersion, &wsa_data) == 0;
    }

    bool WindowsNetwork::Shutdown()
    {
        return WSACleanup() == 0;
    }

}

#endif
