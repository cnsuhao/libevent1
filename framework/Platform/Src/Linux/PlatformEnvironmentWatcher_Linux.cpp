#include <Platform/Include/Linux/PlatformEnvironmentWatcher_Linux.h>

namespace CommunicationPlatform
{
	uint64_t PlatformEnvironmentWatcherImplementation::GetTick()
	{
		timeval tv;

		/* gettimeofday() and settimeofday() return 0 for success, or -1 for failure (in which case errno is set appropriately). */
		if (0 == gettimeofday(&tv,NULL))
		{
			uint64_t resultMilliSecondTick = tv.tv_sec * 1000 + tv.tv_usec / 1000;
			return resultMilliSecondTick;
		}
		else
		{
			return 0;
		}
	}
}
