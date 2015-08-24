#include <Platform/Include/PlatformUDPSocket.h>

namespace CommunicationPlatform
{
    PlatformUDPSocket::PlatformUDPSocket()
        :PlatformUDPSocketImplementation()
        ,m_SendDataMutex()
    {

    }

    PlatformUDPSocket::~PlatformUDPSocket()
    {

    }

    bool PlatformUDPSocket::InitClientSide(PlatformIPAddress& serverSizeIP, uint16_t destinationPort)
    {
        return PlatformUDPSocketImplementation::InitClientSide(serverSizeIP,destinationPort);
    }

    bool PlatformUDPSocket::InitServerSide(PlatformIPAddress& listeningIP, uint16_t listeningPort,bool canReceiveBroadcast)
    {
        return PlatformUDPSocketImplementation::InitServerSide(listeningIP,listeningPort,canReceiveBroadcast);
    }

    bool PlatformUDPSocket::SendBytes(void* pDataBuffer, uint32_t dataLengthInByte)
    {
        Mutex::ScopedMutex lock(m_SendDataMutex);

        return PlatformUDPSocketImplementation::SendBytes(pDataBuffer,dataLengthInByte);
    }

    bool PlatformUDPSocket::ReceiveBytes(void* pReceiveBuffer, uint32_t receiveBufferSizeInByte, uint32_t& numberOfBytesReceived,PlatformIPAddress& receivedDataSourceIP)
    {
        return PlatformUDPSocketImplementation::ReceiveBytes(pReceiveBuffer,receiveBufferSizeInByte,numberOfBytesReceived,receivedDataSourceIP);
    }
}