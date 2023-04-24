#include "synchrony/network/network_address.h"

#include "synchrony/platform/windows_network.h"

namespace synchrony
{
    /************************************************************************/
    /* NETWORK ADDRESS                                                      */
    /************************************************************************/

    std::optional<NetworkAddress> NetworkAddress::FromString(const Syntropy::String& address)
    {
        return PlatformNetwork::MakeNetworkAddress(address);
    }

    Syntropy::String NetworkAddress::ToString() const
    {
        return PlatformNetwork::NetworkAddressToString(*this);
    }

}
