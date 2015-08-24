#include <Platform/Include/PlatformModule.h>

namespace CommunicationPlatform
{
    BasicTaskModule::BasicTaskModule(uint32_t nodeId, ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, ModuleRunner* moduleRunner)
        :BasicTaskModuleImplementation(modulePriority,moduleStackSize,moduleName,moduleRunner),
        m_NodeId(nodeId)
    {
        moduleRunner->SetNodeId(this->m_NodeId);
    }

    BasicTaskModule::~BasicTaskModule()
    {

    }

    ModuleCreateResult BasicTaskModule::Start()
    {
        ModuleCreateResult startResult = BasicTaskModuleImplementation::Start();

        return startResult;
    }

    bool BasicTaskModule::Join()
    {
        return BasicTaskModuleImplementation::Join();
    }

    void BasicTaskModule::Sleep(uint32_t millisecondsToSleep)
    {
        BasicTaskModuleImplementation::Sleep(millisecondsToSleep);
    }

    MessageTriggeredTaskModule::MessageTriggeredTaskModule(uint32_t nodeId, uint32_t moduleId, ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, MessageTriggeredModuleRunner* messageTriggeredModuleRunner)
        :MessageTriggeredTaskModuleImplementation(modulePriority,moduleStackSize,moduleName,messageTriggeredModuleRunner),
        m_NodeId(nodeId),
        m_ModuleId(moduleId)
    {
        MessageTriggeredModuleRunner* pMessageModuleRunner = (MessageTriggeredModuleRunner*)this->m_ModuleRunner;

        pMessageModuleRunner->SetModuleId(this->m_ModuleId);
    }

    MessageTriggeredTaskModule::~MessageTriggeredTaskModule()
    {

    }

    void MessageTriggeredTaskModule::StopProcessMessage()
    {
        MessageTriggeredTaskModuleImplementation::StopProcessMessage();
    }

    void MessageTriggeredTaskModule::Sleep(uint32_t millisecondsToSleep)
    {
        MessageTriggeredTaskModuleImplementation::Sleep(millisecondsToSleep);
    }

}
