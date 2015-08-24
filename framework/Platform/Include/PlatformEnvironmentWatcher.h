#ifndef PLATFORMENVIRONMENTWATCHER_H
#define PLATFORMENVIRONMENTWATCHER_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/LinuxSpecificHeaders.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

namespace CommunicationPlatform
{
    class PlatformEnvironmentWatcher
    {
    public:
        static uint64_t GetTick(); /**< Get the tick value */
    };
}


#endif