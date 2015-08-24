#ifndef PLATFORMAPI_H
#define PLATFORMAPI_H

#ifdef _MSC_VER
    #include <Platform/Include/Windows/WindowsSpecificHeaders.h>
    #include <Platform/Include/Windows/PlatformWindowsConsoleChannel.h>

#else
    #ifdef linux
        #include <Platform/Include/Linux/LinuxSpecificHeaders.h>
        #include <Platform/Include/Linux/PlatformSyslogChannel.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
        #else
            #error Not supported Platform!
        #endif
    #endif    
#endif

#include <Platform/Include/PlatformCommonMacros.h>
#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/PlatformMemoryManagment.h>
#include <Platform/Include/InterTaskMessage.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/PlatformModule.h>
#include <Platform/Include/MessageRouting.h>
#include <Platform/Include/PlatformTimer.h>
#include <Platform/Include/PlatformLogging.h>
#include <Platform/Include/PlatformAsyncLogChannel.h>
#include <Platform/Include/PlatformFileChannel.h>
#include <Platform/Include/PlatformIPAddress.h>
#include <Platform/Include/PlatformSocket.h>
#include <Platform/Include/PlatformUDPSocket.h>
#include <Platform/Include/PlatformSocketLogChannel.h>

#include <sched.h>

#endif
