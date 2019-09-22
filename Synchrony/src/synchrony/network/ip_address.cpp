#include "synchrony/network/ip_address.h"

#include "synchrony/platform/windows_network.h"

namespace synchrony
{
    /************************************************************************/
    /* IPv4 ADDRESS                                                         */
    /************************************************************************/

    std::optional<IPv4Address> IPv4Address::FromString(const std::string& address)
    {
        return PlatformNetwork::MakeIPv4Address(address);
    }

    std::string IPv4Address::ToString() const
    {
        return PlatformNetwork::AddressToString(*this);
    }

    /************************************************************************/
    /* IPv6 ADDRESS                                                         */
    /************************************************************************/

    std::optional<IPv6Address> IPv6Address::FromString(const std::string& address)
    {
        return PlatformNetwork::MakeIPv6Address(address);
    }

    std::string IPv6Address::ToString() const
    {
        return PlatformNetwork::AddressToString(*this);
    }

}
