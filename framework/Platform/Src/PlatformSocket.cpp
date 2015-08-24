#include <Platform/Include/PlatformSocket.h>

namespace CommunicationPlatform
{
    PlatformSocket::PlatformSocket()
        :PlatformSocketImplementation()
    {

    }

    PlatformSocket::~PlatformSocket()
    {

    }

    void PlatformSocket::Close()
    {
        PlatformSocketImplementation::Close();
    }

    PlatformIPAddress* PlatformSocket::GetPeerIPAddress()
    {
        return &(this->m_PeerIPAddress);
    }

    bool PlatformSocket::InitializeNetworkFunctionality()
    {
        return PlatformSocketImplementation::InitializeNetworkFunctionality();
    }

    bool PlatformSocket::GetAllBroadcastAddressesOfInterfaces(std::list<PlatformIPAddress>& outputAddressList)
    {
        return PlatformSocketImplementation::GetAllBroadcastAddressesOfInterfaces(outputAddressList);
    }

}