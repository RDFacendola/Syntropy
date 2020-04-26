#include "synchrony/network/network_address.h"

#include "synchrony/platform/windows_network.h"

namespace synchrony
{
    /************************************************************************/
    /* NETWORK ADDRESS                                                      */
    /************************************************************************/

    std::optional<NetworkAddress> NetworkAddress::FromString(const syntropy::String& address)
    {
        return PlatformNetwork::MakeNetworkAddress(address);
    }

    syntropy::String NetworkAddress::ToString() const
    {
        return PlatformNetwork::NetworkAddressToString(*this);
    }

}
