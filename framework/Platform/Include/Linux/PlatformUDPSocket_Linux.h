#ifndef PLATFORMUDPSOCKET_LINUX_H
#define PLATFORMUDPSOCKET_LINUX_H

#include <Platform/Include/PlatformSocket.h>

namespace CommunicationPlatform
{
	class PlatformUDPSocketImplementation: public PlatformSocket
	{
	public:
		PlatformUDPSocketImplementation();
		virtual ~PlatformUDPSocketImplementation();

		bool InitClientSide(PlatformIPAddress& serverSideIP, uint16_t destinationPort);

		bool InitServerSide(PlatformIPAddress& listeningIP, uint16_t listeningPort,bool canReceiveBroadcast = false);

		bool SendBytes(void* pDataBuffer, uint32_t dataLengthInByte);

		bool ReceiveBytes(void* pReceiveBuffer, uint32_t receiveBufferSizeInByte, uint32_t& numberOfBytesReceived,PlatformIPAddress& receivedDataSourceIP);

		UDPSocketWorkMode GetWorkMode();
	protected:
		PlatformIPAddress m_TheIPAddress;

		sockaddr_in m_SocketIPAddress;

		UDPSocketWorkMode m_WorkMode;
	};
}


#endif