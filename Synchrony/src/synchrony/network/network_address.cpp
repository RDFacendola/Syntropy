#include "synchrony/network/network_address.h"

#include "synchrony/platform/windows_network.h"

namespace synchrony
{
    /************************************************************************/
    /* NETWORK ADDRESS                                                      */
    /************************************************************************/

    std::optional<NetworkAddress> NetworkAddress::FromString(const std::string& address)
    {
        return PlatformNetwork::MakeNetworkAddress(address);
    }

    std::string NetworkAddress::ToString() const
    {
        return PlatformNetwork::NetworkAddressToString(*this);
    }

}
