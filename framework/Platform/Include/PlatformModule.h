#ifndef PLATFORMMODULE_H
#define PLATFORMMODULE_H

#include <string>

#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/InterTaskMessage.h>


#ifdef _MSC_VER
    #include <Platform/Include/Windows/PlatformModule_Win32.h>
#else
    #ifdef linux
        #include <Platform/Include/Linux/PlatformModule_Linux.h>
    #else
        #ifdef __TI_COMPILER_VERSION__
            #include <Platform/Include/SysBios/PlatformModule_SysBios.h>
        #else
           // #error Unsupported platform!
        #endif
    #endif
#endif



namespace CommunicationPlatform
{
    /* Forward class declaration */
    class InterTaskMessage;

    /*
    *   Basic and plain thread/task module, the module start with a entry point function and user have to decide to what to do and when to exit the thread/task
    */
    class BasicTaskModule: private BasicTaskModuleImplementation
    {
    public:
        BasicTaskModule(uint32_t nodeId, ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, ModuleRunner* moduleRunner);
        ~BasicTaskModule();
        ModuleCreateResult Start();
        bool Join();
        static void Sleep(uint32_t millisecondsToSleep);
    protected:
        uint32_t      m_NodeId;
        BasicTaskModule():BasicTaskModuleImplementation(){};
    };

    /*
    * Message triggered task module, the module provide a entry function which will be called when it received a message   
    */
    class MessageTriggeredTaskModule: public MessageTriggeredTaskModuleImplementation
    {
    public:
        MessageTriggeredTaskModule(uint32_t nodeId, uint32_t moduleId, ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, MessageTriggeredModuleRunner* messageTriggeredModuleRunner);
        ~MessageTriggeredTaskModule();
        void StopProcessMessage();
    protected:
        void Sleep(uint32_t millisecondsToSleep);
        uint32_t              m_NodeId;
        uint32_t              m_ModuleId;
    };

}

#endif
