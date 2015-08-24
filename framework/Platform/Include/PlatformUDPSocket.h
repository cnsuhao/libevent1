#ifndef PLATFORMUDPSOCKET_H
#define PLATFORMUDPSOCKET_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformUDPSocket_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformUDPSocket_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformUDPSocket_SYSBIOS_NDK.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif


#include <Platform/Include/PlatformIPAddress.h>
#include <Platform/Include/PlatformMutex.h>

namespace CommunicationPlatform
{
    class PlatformUDPSocket: public PlatformUDPSocketImplementation
    {
    public:
        PlatformUDPSocket();
        virtual ~PlatformUDPSocket();

        bool InitClientSide(PlatformIPAddress& serverSizeIP, uint16_t destinationPort);

        bool InitServerSide(PlatformIPAddress& listeningIP, uint16_t listeningPort, bool canReceiveBroadcast = false);

        bool SendBytes(void* pDataBuffer, uint32_t dataLengthInByte);

        bool ReceiveBytes(void* pReceiveBuffer, uint32_t receiveBufferSizeInByte, uint32_t& numberOfBytesReceived,PlatformIPAddress& receivedDataSourceIP);
    protected:
        Mutex m_SendDataMutex;
    };
}

#endif