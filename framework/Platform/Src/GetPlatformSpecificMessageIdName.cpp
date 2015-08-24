#include <Platform/Include/PlatformInternalMessageEnum.h>


namespace CommunicationPlatform
{
	const char* GetPlatformSpecificMessageIdName(uint32_t platformSepcificMessageId)
	{
		if ( platformSepcificMessageId > PLATFORM_MESSAGE_INVALID_MESSAGE_ID )
		{
			return NULL;
		}

		return PlatformSpecificMessageIdNames[platformSepcificMessageId];
	}
}