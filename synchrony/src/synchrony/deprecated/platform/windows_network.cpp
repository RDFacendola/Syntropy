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

    std::optional<NetworkAddress> WindowsNetwork::MakeNetworkAddress(const Syntropy::String& address)
    {
        auto ipv6_address = IN6_ADDR{};                                             // Network byte-order: big-endian.
        auto ipv4_address = IN_ADDR{};

        auto host_address = NetworkAddress{};

        if (inet_pton(AF_INET6, address.c_str(), &ipv6_address) == 1)               // Translate an IPv6 address.
        {
            host_address.a_ = ntohs(ipv6_address.u.Word[0]);
            host_address.b_ = ntohs(ipv6_address.u.Word[1]);
            host_address.c_ = ntohs(ipv6_address.u.Word[2]);
            host_address.d_ = ntohs(ipv6_address.u.Word[3]);
            host_address.e_ = ntohs(ipv6_address.u.Word[4]);
            host_address.f_ = ntohs(ipv6_address.u.Word[5]);
            host_address.g_ = ntohs(ipv6_address.u.Word[6]);
            host_address.h_ = ntohs(ipv6_address.u.Word[7]);

            return host_address;
        }
        else if (inet_pton(AF_INET, address.c_str(), &ipv4_address) == 1)           // Try IPv4-mapped IPv6 address.
        {
            host_address.a_ = 0x0000;
            host_address.b_ = 0x0000;
            host_address.c_ = 0x0000;
            host_address.d_ = 0x0000;
            host_address.e_ = 0x0000;
            host_address.f_ = 0xFFFF;
            host_address.g_ = ntohs(ipv4_address.S_un.S_un_w.s_w1);
            host_address.h_ = ntohs(ipv4_address.S_un.S_un_w.s_w2);

            return host_address;
        }

        return {};
    }

    Syntropy::String WindowsNetwork::NetworkAddressToString(const NetworkAddress& address)
    {
        auto network_address = IN6_ADDR{};                                          // Network byte-order: big-endian.

        network_address.u.Word[0] = htons(address.a_);
        network_address.u.Word[1] = htons(address.b_);
        network_address.u.Word[2] = htons(address.c_);
        network_address.u.Word[3] = htons(address.d_);
        network_address.u.Word[4] = htons(address.e_);
        network_address.u.Word[5] = htons(address.f_);
        network_address.u.Word[6] = htons(address.g_);
        network_address.u.Word[7] = htons(address.h_);

        char ipv6_address[46];

        inet_ntop(AF_INET6, &network_address, ipv6_address, sizeof(ipv6_address));

        return ipv6_address;
    }

}

#endif
