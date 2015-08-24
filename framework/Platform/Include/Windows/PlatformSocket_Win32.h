#ifndef PLATFORMSOCKET_WIN32_H
#define PLATFORMSOCKET_WIN32_H

#include <list>

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>

#include <Platform/Include/PlatformIPAddress.h>

namespace CommunicationPlatform
{
    class PlatformSocketImplementation
    {
    public:
        PlatformSocketImplementation();
        virtual ~PlatformSocketImplementation();

        void Close();

        static bool InitializeNetworkFunctionality();

        static bool GetAllBroadcastAddressesOfInterfaces(std::list<PlatformIPAddress>& outputAddressList); /**< Get each interface's broadcast address */

    protected:
        SOCKET m_SocketHandle;
    };
}


#endif