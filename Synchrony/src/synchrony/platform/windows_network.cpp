#include "synchrony/platform/windows_network.h"

#ifdef _WIN64

#include <winsock2.h>
#include <ws2tcpip.h>

#include "synchrony/network/ip_address.h"
#include "synchrony/network/ip_endpoint.h"

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

    std::optional<IPv4Address> WindowsNetwork::MakeIPv4Address(const std::string& address)
    {
        auto network_address = IN_ADDR{};           // Network byte-order: big-endian.

        if (inet_pton(AF_INET, address.c_str(), &network_address) == 1)
        {
            auto host_address = IPv4Address{};

            host_address.a_ = network_address.S_un.S_un_b.s_b1;
            host_address.b_ = network_address.S_un.S_un_b.s_b2;
            host_address.c_ = network_address.S_un.S_un_b.s_b3;
            host_address.d_ = network_address.S_un.S_un_b.s_b4;

            return host_address;
        }

        return {};
    }

    std::optional<IPv6Address> WindowsNetwork::MakeIPv6Address(const std::string& address)
    {
        auto network_address = IN6_ADDR{};          // Network byte-order: big-endian.

        if (inet_pton(AF_INET6, address.c_str(), &network_address) == 1)
        {
            auto host_address = IPv6Address{};

            host_address.a_ = ntohs(network_address.u.Word[0]);
            host_address.b_ = ntohs(network_address.u.Word[1]);
            host_address.c_ = ntohs(network_address.u.Word[2]);
            host_address.d_ = ntohs(network_address.u.Word[3]);
            host_address.e_ = ntohs(network_address.u.Word[4]);
            host_address.f_ = ntohs(network_address.u.Word[5]);
            host_address.g_ = ntohs(network_address.u.Word[6]);
            host_address.h_ = ntohs(network_address.u.Word[7]);

            return host_address;
        }

        return {};
    }

    std::string WindowsNetwork::AddressToString(const IPv4Address& address)
    {
        auto network_address = IN_ADDR{};           // Network byte-order: big-endian.

        network_address.S_un.S_un_b.s_b1 = address.a_;
        network_address.S_un.S_un_b.s_b2 = address.b_;
        network_address.S_un.S_un_b.s_b3 = address.c_;
        network_address.S_un.S_un_b.s_b4 = address.d_;

        char ipv4_address[16];

        inet_ntop(AF_INET, &network_address, ipv4_address, sizeof(ipv4_address));

        return ipv4_address;
    }

    std::string WindowsNetwork::AddressToString(const IPv6Address& address)
    {
        auto network_address = IN6_ADDR{};          // Network byte-order: big-endian.

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
