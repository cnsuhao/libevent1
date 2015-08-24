#ifndef PLATFORMSOCKET_LINUX_H
#define PLATFORMSOCKET_LINUX_H

#include <list>

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

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
		int m_SocketHandle;
	};
}


#endif