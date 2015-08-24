#define DECL_PLATFORM_INTERNAL_MESSAGE(x) #x

namespace CommunicationPlatform
{
	const char *PlatformSpecificMessageIdNames[] = 
	{
		#include <Platform/Include/PlatformInternalMessageDeclare.h>
	};

	
}



