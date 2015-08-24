#ifndef PLATFORMENVIRONMENTWATCHER_LINUX_H
#define PLATFORMENVIRONMENTWATCHER_LINUX_H


#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

namespace CommunicationPlatform
{
	class PlatformEnvironmentWatcherImplementation
	{
	public:
		static uint64_t GetTick(); /**< Get the tick value */

	};
}



#endif