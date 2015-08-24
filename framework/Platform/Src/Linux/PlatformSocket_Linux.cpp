#include <Platform/Include/Linux/PlatformSocket_Linux.h>

namespace CommunicationPlatform
{
	PlatformSocketImplementation::PlatformSocketImplementation()
		:m_SocketHandle(-1)
	{

	}

	PlatformSocketImplementation::~PlatformSocketImplementation()
	{
		Close();
	}

	void PlatformSocketImplementation::Close()
	{
		close(m_SocketHandle);

		m_SocketHandle = -1;
	}

	bool PlatformSocketImplementation::InitializeNetworkFunctionality()
	{
		return true;
	}

	bool PlatformSocketImplementation::GetAllBroadcastAddressesOfInterfaces(std::list<PlatformIPAddress>& outputAddressList)
	{
		/* On  success,  a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.*/
		int tmpSocketHandle = socket(AF_INET,SOCK_DGRAM,0);

		if (-1 == tmpSocketHandle)
		{
			return false;
		}

		ifconf tmpIfconfig;

		ifreq ifr[8];
		tmpIfconfig.ifc_buf = (char *) ifr;
		tmpIfconfig.ifc_len = sizeof(ifr);

		/* Return a list of interface (transport layer) addresses.  This currently means  only
		addresses of the AF_INET (IPv4) family for compatibility.  The user passes a ifconf
		structure as argument to the ioctl.  It contains a pointer to  an  array  of  ifreq
		structures  in  ifc_req  and  its length in bytes in ifc_len.  The kernel fills the
		ifreqs with all current L3 interface addresses that are running: ifr_name  contains
		the  interface  name  (eth0:1 etc.), ifr_addr the address.  The kernel returns with
		the actual length in ifc_len.  If ifc_len is equal to the original length the  buf©\
		fer  probably  has  overflowed and you should retry with a bigger buffer to get all
		addresses.  When no error occurs the ioctl returns 0; otherwise  -1.   Overflow  is not an error.*/
		if(-1 == ioctl(tmpSocketHandle,SIOCGIFCONF,&tmpIfconfig))
		{
			close(tmpSocketHandle);
			return false;
		}

		int addrNumber = tmpIfconfig.ifc_len / sizeof(ifreq);

		for(int interfaceIndex=0;interfaceIndex<addrNumber;interfaceIndex++)
		{
			if (ioctl(tmpSocketHandle,SIOCGIFBRDADDR,&ifr[interfaceIndex]) >= 0)
			{
				char tmpIPAddressName[INET_ADDRSTRLEN];
				struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[interfaceIndex].ifr_broadaddr;

				/* The loop back ip address 127.0.0.1's broadcast address is 0.0.0.0, we simply ignore it */
				if (s_in->sin_addr.s_addr != htonl(INADDR_ANY))
				{
					PlatformIPAddress currentIPAddress;
					currentIPAddress.SetAddressImple(s_in->sin_addr);

					outputAddressList.push_back(currentIPAddress);
				}
			}
		}

		close(tmpSocketHandle);
		return true;
	}
}