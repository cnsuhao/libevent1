#ifndef PLATFORMIPADDRESS_LINUX_H
#define PLATFORMIPADDRESS_LINUX_H

#include <string>
#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

namespace CommunicationPlatform
{
	class PlatformIPAddressImplementation
	{
	public:
		PlatformIPAddressImplementation();
		virtual ~PlatformIPAddressImplementation();


		bool SetAddress(const std::string& ipAddress);

		bool SetAddressImple(const in_addr& ipAddress);

		std::string ToString() const;

		bool IsBroadcast() const;

		const void* AddressDataPointer() const;

		uint32_t GetAddressLength();

	protected:
	private:
		in_addr m_IPAddress;
	};
}

#endif