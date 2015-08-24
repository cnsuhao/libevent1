#ifndef PLATFORMSOCKET_H
#define PLATFORMSOCKET_H

#include <list>

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformSocket_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/LinuxSpecificHeaders.h>
		#include <Platform/Include/Linux/PlatformSocket_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

#include <Platform/Include/PlatformIPAddress.h>

namespace CommunicationPlatform
{
    enum UDPSocketWorkMode
    {
        UDPSOCKET_WORKMODE_CLIENT  =0,
        UDPSOCKET_WORKMODE_SERVER  =1,
        UDPSOCKET_WORKMODE_UNKNOWN =2
    };

    class PlatformSocket: public PlatformSocketImplementation
    {
    public:
        PlatformSocket();
        virtual ~PlatformSocket();

        void Close();

        PlatformIPAddress* GetPeerIPAddress();

        static bool InitializeNetworkFunctionality();

        static bool GetAllBroadcastAddressesOfInterfaces(std::list<PlatformIPAddress>& outputAddressList); /**< Get each interface's broadcast address */
    protected:
        PlatformIPAddress m_PeerIPAddress;
    };
}


#endif