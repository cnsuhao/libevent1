#ifndef PLATFORMENVIRONMENTWATCHER_WIN32_H
#define PLATFORMENVIRONMENTWATCHER_WIN32_H

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>

namespace CommunicationPlatform
{
    class PlatformEnvironmentWatcherImplementation
    {
    public:
        static uint64_t GetTick(); /**< Get the tick value */

    };
}


#endif