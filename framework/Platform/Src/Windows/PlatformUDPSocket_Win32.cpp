#include <Platform/Include/Windows/PlatformUDPSocket_Win32.h>



namespace CommunicationPlatform
{
    PlatformUDPSocketImplementation::PlatformUDPSocketImplementation()
        :m_WorkMode(UDPSOCKET_WORKMODE_UNKNOWN)
    {
        this->m_SocketIPAddress.sin_family      = AF_INET;
        this->m_SocketIPAddress.sin_port        = 0;
        this->m_SocketIPAddress.sin_addr.s_addr = INADDR_ANY;
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

        /* If no error occurs, socket returns a descriptor referencing the new socket. 
           Otherwise, a value of INVALID_SOCKET is returned, and a specific error code can be retrieved by calling WSAGetLastError.*/
        if (INVALID_SOCKET == this->m_SocketHandle)
        {
            return false;
        }

        BOOL bOptVal = TRUE;
        int bOptLen = sizeof(BOOL);

        /* If no error occurs, setsockopt returns zero. Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.*/
        int setOptionResult = setsockopt(this->m_SocketHandle,SOL_SOCKET,SO_BROADCAST,(char*)&bOptVal,bOptLen);

        if (SOCKET_ERROR == setOptionResult)
        {
            Close();
            return false;
        }

        this->m_TheIPAddress = serverSideIP;

        this->m_SocketIPAddress.sin_port = htons(destinationPort);

        memcpy(&(this->m_SocketIPAddress.sin_addr.S_un.S_un_b.s_b1),serverSideIP.AddressDataPointer(),serverSideIP.GetAddressLength());

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

        /* If no error occurs, socket returns a descriptor referencing the new socket. 
           Otherwise, a value of INVALID_SOCKET is returned, and a specific error code can be retrieved by calling WSAGetLastError.*/
        if (INVALID_SOCKET == this->m_SocketHandle)
        {
            return false;
        }

        if (true == canReceiveBroadcast)
        {
            BOOL bOptVal = TRUE;
            int bOptLen = sizeof(BOOL);

            /* If no error occurs, setsockopt returns zero. Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.*/
            int setOptionResult = setsockopt(this->m_SocketHandle,SOL_SOCKET,SO_BROADCAST,(char*)&bOptVal,bOptLen);

            if (SOCKET_ERROR == setOptionResult)
            {
                Close();
                return false;
            }
        }

        this->m_TheIPAddress = listeningIP;

        this->m_SocketIPAddress.sin_port = htons(listeningPort);

        memcpy(&(this->m_SocketIPAddress.sin_addr.S_un.S_un_b.s_b1),listeningIP.AddressDataPointer(),listeningIP.GetAddressLength());

        /* If no error occurs, bind returns zero. Otherwise, it returns SOCKET_ERROR, and a specific error code can be retrieved by calling WSAGetLastError.*/
        int bindResult = bind(this->m_SocketHandle, (SOCKADDR *) &(this->m_SocketIPAddress), sizeof(sockaddr_in));

        if (SOCKET_ERROR == bindResult)
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

        int sendResult = sendto(this->m_SocketHandle,(char*)pDataBuffer,dataLengthInByte,0,(SOCKADDR*)&(this->m_SocketIPAddress),sizeof(sockaddr_in));

        if (SOCKET_ERROR == sendResult)
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

        /* If no error occurs, recvfrom returns the number of bytes received. If the connection has been gracefully closed, the return value is zero. Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError. */
        int receiveResult = recvfrom(this->m_SocketHandle,(char*)pReceiveBuffer,receiveBufferSizeInByte,0,(SOCKADDR*)&SenderAddr,&SenderAddrSize);

        if (SOCKET_ERROR == receiveResult)
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