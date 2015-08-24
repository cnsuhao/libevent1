#include <Platform/Include/PlatformCommonMacros.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/InterTaskMessage.h>

namespace CommunicationPlatform
{
    void MessageTriggeredModuleRunner::PostReceivedMessage(InterTaskMessage* pReceivedMessage)
    {
        USE_UNFERENCED_PARAMETER(pReceivedMessage);
    }

    void ModuleRunner::SetNodeId(uint32_t nodeId)
    {
        this->m_NodeId = nodeId;
    }

    void MessageTriggeredModuleRunner::SetModuleId(uint32_t moduleId)
    {
        this->m_ModuleId = moduleId;
    }

}
