#ifndef PLATFORMMODULE_WIN32_H
#define PLATFORMMODULE_WIN32_H

#include <string>

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/PlatformCommonClass.h>

namespace CommunicationPlatform
{
    class BasicTaskModuleImplementation
    {
    public:
        BasicTaskModuleImplementation(ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, ModuleRunner* moduleRunner);
        virtual ~BasicTaskModuleImplementation();
        ModuleCreateResult Start();
        bool Join();
    protected:
        static void Sleep(uint32_t millisecondsToSleep);
        static unsigned __stdcall ModuleEntry(LPVOID pThread);

        virtual uint32_t RunEntry();
        void Cleanup();

        ModulePriority m_Priority;
        uint32_t       m_StackSize;
        std::string    m_ModuleName;
        ModuleRunner*  m_ModuleRunner;
        BasicTaskModuleImplementation(){};

    private:
        HANDLE m_ThreadHandle;
        uint32_t m_ThreadId;

    };

    class MessageTriggeredTaskModuleImplementation: public BasicTaskModuleImplementation
    {
    public:
        MessageTriggeredTaskModuleImplementation(ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, MessageTriggeredModuleRunner* messageTriggeredModuleRunner);
        virtual ~MessageTriggeredTaskModuleImplementation();
        virtual uint32_t RunEntry();
        virtual void StopProcessMessage();
    protected:
        MessageTriggeredModuleRunner* m_MessageTriggeredModuleRunner;
        bool           m_KeepRunning;
    };
}



#endif