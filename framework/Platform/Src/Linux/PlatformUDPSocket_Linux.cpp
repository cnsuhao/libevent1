#include <Platform/Include/Linux/PlatformUDPSocket_Linux.h>

namespace CommunicationPlatform
{
    PlatformUDPSocketImplementation::PlatformUDPSocketImplementation()
        :m_WorkMode(UDPSOCKET_WORKMODE_UNKNOWN)
    {
        this->m_SocketIPAddress.sin_family      = AF_INET;
        this->m_SocketIPAddress.sin_port        = 0;
        this->m_SocketIPAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    PlatformUDPSocketImplementation::~PlatformUDPSocketImplementation()
    {
    }


    bool PlatformUDPSocketImplementation::InitClientSide(PlatformIPAddress& serverSideIP, uint16_t destinationPort)
    {
        if (this->m_WorkMode != UDPSOCKET_WORKMODE_UNKNOWN)
        {
            return false;
        }

        this->m_SocketHandle = socket(AF_INET,SOCK_DGRAM,0);

		/* On  success, a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.*/
        if (-1 == this->m_SocketHandle)
        {
            return false;
        }

        int bOptVal = 1;
        int bOptLen = sizeof(bOptVal);

		/* On success, zero is returned.  On error, -1 is returned, and errno is set appropriately. */
        int setOptionResult = setsockopt(this->m_SocketHandle,SOL_SOCKET,SO_BROADCAST,&bOptVal,bOptLen);

        if (-1 == setOptionResult)
        {
            Close();
            return false;
        }

        this->m_TheIPAddress = serverSideIP;

        this->m_SocketIPAddress.sin_port = htons(destinationPort);

        memcpy(&(this->m_SocketIPAddress.sin_addr),serverSideIP.AddressDataPointer(),serverSideIP.GetAddressLength());

        this->m_WorkMode = UDPSOCKET_WORKMODE_CLIENT;

        return true;
    }

    bool PlatformUDPSocketImplementation::InitServerSide(PlatformIPAddress& listeningIP, uint16_t listeningPort,bool canReceiveBroadcast)
    {
        if (this->m_WorkMode != UDPSOCKET_WORKMODE_UNKNOWN)
        {
            return false;
        }

        this->m_SocketHandle = socket(AF_INET,SOCK_DGRAM,0);

		/* On  success, a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.*/
		if (-1 == this->m_SocketHandle)
		{
			return false;
		}

        if (true == canReceiveBroadcast)
        {
			int bOptVal = 1;
			int bOptLen = sizeof(bOptVal);

			/* On success, zero is returned.  On error, -1 is returned, and errno is set appropriately. */
			int setOptionResult = setsockopt(this->m_SocketHandle,SOL_SOCKET,SO_BROADCAST,&bOptVal,bOptLen);

			if (-1 == setOptionResult)
			{
				Close();
				return false;
			}
        }

        this->m_TheIPAddress = listeningIP;

        this->m_SocketIPAddress.sin_port = htons(listeningPort);

        memcpy(&(this->m_SocketIPAddress.sin_addr),listeningIP.AddressDataPointer(),listeningIP.GetAddressLength());

        /* On success, zero is returned.  On error, -1 is returned, and errno is set appropriately. */
        int bindResult = bind(this->m_SocketHandle, (sockaddr *) &(this->m_SocketIPAddress), sizeof(sockaddr_in));

        if (-1 == bindResult)
        {
            Close();
            return false;
        }

        this->m_WorkMode = UDPSOCKET_WORKMODE_SERVER;

        return true;
    }

    bool PlatformUDPSocketImplementation::SendBytes(void* pDataBuffer, uint32_t dataLengthInByte)
    {
        if (this->m_WorkMode == UDPSOCKET_WORKMODE_UNKNOWN)
        {
            return false;
        }

		/* On  success,  these calls return the number of characters sent.  On error, -1 is returned, and errno is set appropriately */
        int sendResult = sendto(this->m_SocketHandle,(char*)pDataBuffer,dataLengthInByte,0,(sockaddr*)&(this->m_SocketIPAddress),sizeof(sockaddr_in));

        if (-1 == sendResult)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool PlatformUDPSocketImplementation::ReceiveBytes(void* pReceiveBuffer, uint32_t receiveBufferSizeInByte, uint32_t& numberOfBytesReceived,PlatformIPAddress& receivedDataSourceIP)
    {
        if (this->m_WorkMode == UDPSOCKET_WORKMODE_UNKNOWN)
        {
            return false;
        }

        sockaddr_in SenderAddr;

        int SenderAddrSize = sizeof(sockaddr_in);

		/* These calls return the number of bytes received, or -1 if an error occurred.   The  return value will be 0 when the peer has performed an orderly shutdown.*/
        int receiveResult = recvfrom(this->m_SocketHandle,pReceiveBuffer,receiveBufferSizeInByte,0,(sockaddr*)&SenderAddr,(socklen_t*)&SenderAddrSize);

        if (-1 == receiveResult)
        {
            return false;
        }

        if (0 == receiveResult)
        {
            Close();
            this->m_WorkMode = UDPSOCKET_WORKMODE_UNKNOWN;
            return false;
        }

        receivedDataSourceIP.SetAddressImple(SenderAddr.sin_addr);

        numberOfBytesReceived = receiveResult;

        return true;
    }

    UDPSocketWorkMode PlatformUDPSocketImplementation::GetWorkMode()
    {
        return this->m_WorkMode;
    }
}