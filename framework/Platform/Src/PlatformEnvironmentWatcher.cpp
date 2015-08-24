#include <Platform/Include/PlatformEnvironmentWatcher.h>

#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformEnvironmentWatcher_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformEnvironmentWatcher_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformEnvironmentWatcher_SYSBIOS.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

namespace CommunicationPlatform
{
    /**< Get the tick value */
    uint64_t PlatformEnvironmentWatcher::GetTick()
    {
        return PlatformEnvironmentWatcherImplementation::GetTick();
    }
}