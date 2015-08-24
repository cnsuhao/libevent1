#include <Platform/Include/Windows/PlatformEnvironmentWatcher_Win32.h>

namespace CommunicationPlatform
{
    /**< Get the tick value */
    uint64_t PlatformEnvironmentWatcherImplementation::GetTick()
    {
        return GetTickCount64();
    }
}