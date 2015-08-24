#ifndef PLATFORMMODULE_SYSBIOS_H
#define PLATFORMMODULE_SYSBIOS_H

#include <string>

#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
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
        static Void ModuleEntry(UArg arg0, UArg arg1);
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
        Task_Handle m_ThreadHandle;

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
