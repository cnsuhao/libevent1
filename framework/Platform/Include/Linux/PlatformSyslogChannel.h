#ifndef PLATFORMSYSLOGCHANNEL_H
#define PLATFORMSYSLOGCHANNEL_H

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>
#include <Platform/Include/PlatformLogChannel.h>

namespace CommunicationPlatform
{
	class PlatformSyslogChannel: public PlatformLogChannel
	{
	public:
		PlatformSyslogChannel();

		virtual ~PlatformSyslogChannel();

		bool Open();
	protected:
		virtual void logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte);
		/// Logs the given string to the channel. Must be
		/// overridden by subclasses.

		virtual void logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte);
		/// Logs the given string and dump the bin buffer to the channel. Must be
		/// overridden by subclasses.
	private:
		PlatformSyslogChannel(const PlatformSyslogChannel&);
		PlatformSyslogChannel& operator = (const PlatformSyslogChannel&);

	};
}


#endif