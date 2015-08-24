#include <syslog.h>

#include <Platform/Include/Linux/PlatformSyslogChannel.h>
#include <Platform/Include/PlatformLogFormatter.h>

namespace CommunicationPlatform
{
	PlatformSyslogChannel::PlatformSyslogChannel()
		:PlatformLogChannel()
	{
	}

	PlatformSyslogChannel::~PlatformSyslogChannel()
	{

	}

	bool PlatformSyslogChannel::Open()
	{
		openlog(NULL,LOG_NDELAY,LOG_USER);

		return true;
	}


	void PlatformSyslogChannel::logString(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte)
	{
		switch (logLevel)
		{
		case LOGGING_LEVEL_FATAL:
			{
				syslog(LOG_EMERG,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_CRITICAL:
			{
				syslog(LOG_ALERT,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_ERROR:
			{
				syslog(LOG_CRIT,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_WARNING:
			{
				syslog(LOG_ERR,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_NOTICE:
			{
				syslog(LOG_WARNING,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_INFORMATION:
			{
				syslog(LOG_NOTICE,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_DEBUG:
			{
				syslog(LOG_INFO,"%s",stringToBeLogged);
			}
			break;
		case LOGGING_LEVEL_TRACE:
			{
				syslog(LOG_DEBUG,"%s",stringToBeLogged);
			}
			break;
		default:
			break;
		}
	}

	void PlatformSyslogChannel::logDumpBin(LoggingLevel logLevel, const char* stringToBeLogged, uint32_t lenOfStringToBeLoggedInByte, const void* pData, uint32_t lenOfDataInByte)
	{
		std::ostringstream binDataText;

		PlatformLogFormatter::FormatBinaryBufferToString(binDataText,pData,lenOfDataInByte);

		std::string formattedString = binDataText.str();

		switch (logLevel)
		{
		case LOGGING_LEVEL_FATAL:
			{
				syslog(LOG_EMERG,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_CRITICAL:
			{
				syslog(LOG_ALERT,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_ERROR:
			{
				syslog(LOG_CRIT,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_WARNING:
			{
				syslog(LOG_ERR,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_NOTICE:
			{
				syslog(LOG_WARNING,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_INFORMATION:
			{
				syslog(LOG_NOTICE,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_DEBUG:
			{
				syslog(LOG_INFO,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		case LOGGING_LEVEL_TRACE:
			{
				syslog(LOG_DEBUG,"%s %s",stringToBeLogged,formattedString.c_str());
			}
			break;
		default:
			break;
		}
	}
}