#ifndef PLATFORMMODULE_LINUX_H
#define PLATFORMMODULE_LINUX_H

#include <string>

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>
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
        static void* ModuleEntry(void* pThread);
        static uint32_t ModuleEntryReturnValue;
    protected:
        static void Sleep(uint32_t millisecondsToSleep);

        virtual uint32_t RunEntry();

        ModulePriority m_Priority;
        uint32_t       m_StackSize;
        std::string    m_ModuleName;
        ModuleRunner*  m_ModuleRunner;
        BasicTaskModuleImplementation(){};

    private:
        pthread_t m_ThreadId;

        int CalculatePthreadPriorityValue(ModulePriority abstractPriority);
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
