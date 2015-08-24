#include <Platform/Include/Windows/PlatformSocket_Win32.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

namespace CommunicationPlatform
{
    PlatformSocketImplementation::PlatformSocketImplementation()
        :m_SocketHandle(INVALID_SOCKET)
    {

    }

    PlatformSocketImplementation::~PlatformSocketImplementation()
    {

    }

    void PlatformSocketImplementation::Close()
    {
        closesocket(m_SocketHandle);

        m_SocketHandle = INVALID_SOCKET;
    }

    bool PlatformSocketImplementation::InitializeNetworkFunctionality()
    {
        WSADATA wsaData;

        int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );

        if( iResult == NO_ERROR )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PlatformSocketImplementation::GetAllBroadcastAddressesOfInterfaces(std::list<PlatformIPAddress>& outputAddressList)
    {
        PMIB_IPADDRTABLE pIPAddrTable = (MIB_IPADDRTABLE *) malloc(sizeof (MIB_IPADDRTABLE));

        if (NULL == pIPAddrTable)
        {
            return false;
        }

        DWORD dwSize = 0;
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);

            if (pIPAddrTable == NULL) 
            {
                return false;
            }
        }

        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) != NO_ERROR ) 
        { 
            free(pIPAddrTable);
            return false;
        }

        for (DWORD ipIndex = 0;ipIndex< pIPAddrTable->dwNumEntries; ipIndex++)
        {
            in_addr currentIp;
            currentIp.s_addr = pIPAddrTable->table[ipIndex].dwAddr;

            in_addr resultBroadCastAddr;
            resultBroadCastAddr.s_addr = (currentIp.s_addr & pIPAddrTable->table[ipIndex].dwMask) | (~pIPAddrTable->table[ipIndex].dwMask);

            PlatformIPAddress currentIPAddress;
            currentIPAddress.SetAddressImple(resultBroadCastAddr);

            outputAddressList.push_back(currentIPAddress);
        }

        if (pIPAddrTable) 
        {
            free(pIPAddrTable);
            pIPAddrTable = NULL;
        }

        if (0 == outputAddressList.size())
        {
            return false;
        }

        return true;
    }
}