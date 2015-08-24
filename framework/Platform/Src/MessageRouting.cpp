#include <string.h>
#include <Platform/Include/MessageRouting.h>
#include <Platform/Include/PlatformCommonClass.h>
#include <Platform/Include/InterTaskMessage.h>
#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/PlatformLogging.h>
#include <Platform/Include/PlatformCommonMacros.h>
#include <Platform/Include/PlatformUDPSocket.h>

SET_THIS_FILE_NAME()

namespace CommunicationPlatform
{
    IntraNodeModuleDestinationInfo::IntraNodeModuleDestinationInfo(uint32_t nodeId, uint32_t moduleId,MessageTriggeredModuleRunner* destination)
        :ModuleDestinationInfo(nodeId,moduleId),
        m_Destination(destination)
    {
    }

    IntraNodeModuleDestinationInfo::~IntraNodeModuleDestinationInfo()
    {
    
    }

    /** 
    *    For intra-node, inter-module communication only, 
    *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult IntraNodeModuleDestinationInfo::SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity, InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte)
    {
        InterTaskMessage* pMessageToSend = NULL;

        if (pMessage != NULL)
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NEW);
        } 
        else
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE);
        }

        if (NULL == pMessageToSend)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pMessageToSend->m_MessageData.pMessageDataNew = pMessage;

        m_Destination->m_MessageQueue.enqueueNotification(pMessageToSend);

        return MESSAGE_SEND_SUCCESS;
    }

    /** 
    *    Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult IntraNodeModuleDestinationInfo::CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity, void* pMessage, uint32_t MessageSizeInByte)
    {
        InterTaskMessage* pMessageToSend = NULL;

        if (   (NULL != pMessage)
             &&(0    != MessageSizeInByte )
           )
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_MALLOC);
        } 
        else
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE);
        }

        if (NULL == pMessageToSend)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        if (   (NULL != pMessage)
             &&(0    != MessageSizeInByte )
           )
        {
            pMessageToSend->m_MessageData.pMessageDataMalloc = malloc(MessageSizeInByte);
            if (NULL == pMessageToSend->m_MessageData.pMessageDataMalloc)
            {
                delete pMessageToSend;
                return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
            }

            memcpy(pMessageToSend->m_MessageData.pMessageDataMalloc,pMessage,MessageSizeInByte);
        }

        m_Destination->m_MessageQueue.enqueueNotification(pMessageToSend);

        return MESSAGE_SEND_SUCCESS;
    }

    /** 
    *    Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult IntraNodeModuleDestinationInfo::SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity, void* pMessage, uint32_t MessageSizeInByte)
    {
        InterTaskMessage* pMessageToSend = NULL;

        if (pMessage != NULL)
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_MALLOC);
        } 
        else
        {
            pMessageToSend = new InterTaskMessage(messageIdentity,MessageSizeInByte,sourceNodeId,sourceModuleId,InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE);
        }

        if (NULL == pMessageToSend)
        {
            return MESSAGE_SEND_FAIL_INSUFFICIENT_MEMORY;
        }

        pMessageToSend->m_MessageData.pMessageDataMalloc = pMessage;

        m_Destination->m_MessageQueue.enqueueNotification(pMessageToSend);

        return MESSAGE_SEND_SUCCESS;
    }

    InterNodeModuleUDPDestinationInfo::InterNodeModuleUDPDestinationInfo(uint32_t nodeId, uint32_t moduleId, PlatformUDPSocket* targetModuleSocket)
        :ModuleDestinationInfo(nodeId,moduleId),
        m_Destination(targetModuleSocket)
    {

    }

    InterNodeModuleUDPDestinationInfo::~InterNodeModuleUDPDestinationInfo()
    {

    }
    
    /** 
    *    @brief  For intra-node, inter-module communication only, 
    *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult InterNodeModuleUDPDestinationInfo::SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte)
    {
        USE_UNFERENCED_PARAMETER(sourceNodeId);
        USE_UNFERENCED_PARAMETER(sourceModuleId);
        USE_UNFERENCED_PARAMETER(messageIdentity);
        USE_UNFERENCED_PARAMETER(pMessage);
        USE_UNFERENCED_PARAMETER(MessageSizeInByte);

        return MESSAGE_SEND_FAIL_UNSUPPORTED_SEND_METHOD;
    }

    /** 
    *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult InterNodeModuleUDPDestinationInfo::CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte)
    {
        InterNodeTaskMessage* pMessageToSend = (InterNodeTaskMessage*)m_PacketAssembleBuffer;

        pMessageToSend->m_MessageIdentity[0] = (uint8_t)(messageIdentity>>24);
        pMessageToSend->m_MessageIdentity[1] = (uint8_t)(messageIdentity>>16);
        pMessageToSend->m_MessageIdentity[2] = (uint8_t)(messageIdentity>>8);
        pMessageToSend->m_MessageIdentity[3] = (uint8_t)(messageIdentity);


        pMessageToSend->m_MessageSizeInByte[0] = (uint8_t)(MessageSizeInByte>>24);
        pMessageToSend->m_MessageSizeInByte[1] = (uint8_t)(MessageSizeInByte>>16);
        pMessageToSend->m_MessageSizeInByte[2] = (uint8_t)(MessageSizeInByte>>8);
        pMessageToSend->m_MessageSizeInByte[3] = (uint8_t)MessageSizeInByte;

        pMessageToSend->m_MessageSourceNodeId[0] = (uint8_t)(sourceNodeId>>24);
        pMessageToSend->m_MessageSourceNodeId[1] = (uint8_t)(sourceNodeId>>16);
        pMessageToSend->m_MessageSourceNodeId[2] = (uint8_t)(sourceNodeId>>8);
        pMessageToSend->m_MessageSourceNodeId[3] = (uint8_t)sourceNodeId;

        pMessageToSend->m_MessageSourceModuleId[0] = (uint8_t)(sourceModuleId>>24);
        pMessageToSend->m_MessageSourceModuleId[1] = (uint8_t)(sourceModuleId>>16);
        pMessageToSend->m_MessageSourceModuleId[2] = (uint8_t)(sourceModuleId>>8);
        pMessageToSend->m_MessageSourceModuleId[3] = (uint8_t)sourceModuleId;

        pMessageToSend->m_MessageDestNodeId[0] = (uint8_t)(this->m_NodeId>>24);
        pMessageToSend->m_MessageDestNodeId[1] = (uint8_t)(this->m_NodeId>>16);
        pMessageToSend->m_MessageDestNodeId[2] = (uint8_t)(this->m_NodeId>>8);
        pMessageToSend->m_MessageDestNodeId[3] = (uint8_t)this->m_NodeId;

        pMessageToSend->m_MessageDestModuleId[0] = (uint8_t)(this->m_ModuleId>>24);
        pMessageToSend->m_MessageDestModuleId[1] = (uint8_t)(this->m_ModuleId>>16);
        pMessageToSend->m_MessageDestModuleId[2] = (uint8_t)(this->m_ModuleId>>8);
        pMessageToSend->m_MessageDestModuleId[3] = (uint8_t)this->m_ModuleId;

        if (   (NULL != pMessage)
            &&(0    != MessageSizeInByte )
            )
        {
            memcpy(pMessageToSend->messageBody,pMessage,MessageSizeInByte);
        }

        if(false == this->m_Destination->SendBytes(pMessageToSend,sizeof(InterNodeTaskMessage)+MessageSizeInByte))
        {
            return MESSAGE_SEND_FAIL_OTHER_REASON;
        }

        return MESSAGE_SEND_SUCCESS;
    }

    /** 
    *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
    *    @param [in] pMessage           The message pointer 
    *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
    */
    MessageSendResult InterNodeModuleUDPDestinationInfo::SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte)
    {
        InterNodeTaskMessage* pMessageToSend = (InterNodeTaskMessage*)m_PacketAssembleBuffer;

        pMessageToSend->m_MessageIdentity[0] = (uint8_t)(messageIdentity>>24);
        pMessageToSend->m_MessageIdentity[1] = (uint8_t)(messageIdentity>>16);
        pMessageToSend->m_MessageIdentity[2] = (uint8_t)(messageIdentity>>8);
        pMessageToSend->m_MessageIdentity[3] = (uint8_t)messageIdentity;


        pMessageToSend->m_MessageSizeInByte[0] = (uint8_t)(MessageSizeInByte>>24);
        pMessageToSend->m_MessageSizeInByte[1] = (uint8_t)(MessageSizeInByte>>16);
        pMessageToSend->m_MessageSizeInByte[2] = (uint8_t)(MessageSizeInByte>>8);
        pMessageToSend->m_MessageSizeInByte[3] = (uint8_t)MessageSizeInByte;

        pMessageToSend->m_MessageSourceNodeId[0] = (uint8_t)(sourceNodeId>>24);
        pMessageToSend->m_MessageSourceNodeId[1] = (uint8_t)(sourceNodeId>>16);
        pMessageToSend->m_MessageSourceNodeId[2] = (uint8_t)(sourceNodeId>>8);
        pMessageToSend->m_MessageSourceNodeId[3] = (uint8_t)sourceNodeId;

        pMessageToSend->m_MessageSourceModuleId[0] = (uint8_t)(sourceModuleId>>24);
        pMessageToSend->m_MessageSourceModuleId[1] = (uint8_t)(sourceModuleId>>16);
        pMessageToSend->m_MessageSourceModuleId[2] = (uint8_t)(sourceModuleId>>8);
        pMessageToSend->m_MessageSourceModuleId[3] = (uint8_t)sourceModuleId;

        pMessageToSend->m_MessageDestNodeId[0] = (uint8_t)(this->m_NodeId>>24);
        pMessageToSend->m_MessageDestNodeId[1] = (uint8_t)(this->m_NodeId>>16);
        pMessageToSend->m_MessageDestNodeId[2] = (uint8_t)(this->m_NodeId>>8);
        pMessageToSend->m_MessageDestNodeId[3] = (uint8_t)this->m_NodeId;

        pMessageToSend->m_MessageDestModuleId[0] = (uint8_t)(this->m_ModuleId>>24);
        pMessageToSend->m_MessageDestModuleId[1] = (uint8_t)(this->m_ModuleId>>16);
        pMessageToSend->m_MessageDestModuleId[2] = (uint8_t)(this->m_ModuleId>>8);
        pMessageToSend->m_MessageDestModuleId[3] = (uint8_t)this->m_ModuleId;

        if (   (NULL != pMessage)
            &&(0    != MessageSizeInByte )
            )
        {
            memcpy(pMessageToSend->messageBody,pMessage,MessageSizeInByte);
        }

        if(false == this->m_Destination->SendBytes(pMessageToSend,sizeof(InterNodeTaskMessage)+MessageSizeInByte))
        {
            free(pMessage);
            return MESSAGE_SEND_FAIL_OTHER_REASON;
        }

        free(pMessage);
        return MESSAGE_SEND_SUCCESS;
    }

    InterNodeModuleUDPDestinationInfoEndPoint::InterNodeModuleUDPDestinationInfoEndPoint(std::string moduleName)
        :ModuleRunner(moduleName),
        m_DataReceiveSocket(),
        m_ListeningPort(0)
    {

    }

    InterNodeModuleUDPDestinationInfoEndPoint::~InterNodeModuleUDPDestinationInfoEndPoint()
    {
        this->m_DataReceiveSocket.Close();
    }

    ModuleRunner::RunnerInitResult InterNodeModuleUDPDestinationInfoEndPoint::InitRunner()
    {
        PlatformIPAddress listingAddress;
        listingAddress.SetAddress("0.0.0.0");

        if (false == this->m_DataReceiveSocket.InitServerSide(listingAddress,this->m_ListeningPort,true))
        {
            Log_printf(Logger::Get(),LOGGING_LEVEL_ERROR,"InterNodeModuleUDPDestinationInfoEndPoint Init fail because m_DataReceiveSocket.InitServerSide fails");
            return RUNNER_INIT_FAIL;
        }

        return RUNNER_INIT_SUCCESS;
    }

    uint32_t InterNodeModuleUDPDestinationInfoEndPoint::RunEntry()
    {
        uint32_t receivedDataLength = 0;

        PlatformIPAddress clientIPAddress;

        for (;;)
        {
            bool receiveResult = this->m_DataReceiveSocket.ReceiveBytes(this->m_PacketReceivingBuffer,sizeof(m_PacketReceivingBuffer),receivedDataLength,clientIPAddress);

            if (false == receiveResult)
            {
                continue;
            }

            InterNodeTaskMessage* pReceivedMessage = (InterNodeTaskMessage*)this->m_PacketReceivingBuffer;


            Message_CopyAndSend(pReceivedMessage->GetSourceNodeId(),pReceivedMessage->GetSourceModuleId(),
                                PLATFORM_INTERNAL_NODE_THIS_NODE,pReceivedMessage->GetDestModuleId(),
                                pReceivedMessage->GetMessageIdentity(),pReceivedMessage->messageBody,pReceivedMessage->GetMessageSizeInByte());

        }
    }

    NodeMessageRouter* NodeMessageRouter::m_pInstance = NULL;

    NodeMessageRouter::NodeMessageRouter()
    {
        this->m_RounterLogger = Logger::Get("MESSAGE ROUTER");
    }

    NodeMessageRouter* NodeMessageRouter::GetMessageRouter()
    {
        if (NULL == m_pInstance)
        {
            m_pInstance = new NodeMessageRouter();
        }
        
        return m_pInstance;
    }

    NodeMessageRouter::InterNodeRoutingConfig& NodeMessageRouter::GetRoutingConfig()
    {
        return this->m_RountingConfigs;
    }
    
	/**
	 * Initialize the routing configuration using pRoutingInitializer 
	 */
	bool NodeMessageRouter::InitializeRoutingConfig(IRoutingConfiger* pRoutingInitializer)
	{
		if (NULL != pRoutingInitializer)
		{
			return pRoutingInitializer->InitRoutingConfig(this->m_RountingConfigs);
		}
		else
		{
			return false;
		}
	}
	
	bool NodeMessageRouter::ModuleRegister(uint32_t nodeId, uint32_t moduleId, ModuleDestinationInfo* pModuleDestInfo)
	{
		NodeMessageRouter::InterNodeRoutingConfig::iterator nodeFindResult = m_RountingConfigs.find(nodeId);
		if (nodeFindResult == m_RountingConfigs.end())
		{
		    NodeMessageRouter::IntraNodeRoutingConfig intraNodeRoutingConfig;
		    intraNodeRoutingConfig.insert(NodeMessageRouter::IntraNodeRoutingConfig::value_type(moduleId,pModuleDestInfo));
			m_RountingConfigs.insert(NodeMessageRouter::InterNodeRoutingConfig::value_type(nodeId,intraNodeRoutingConfig));
			return true;
		    // return NULL;
		}else {
			NodeMessageRouter::IntraNodeRoutingConfig::iterator moduleFindResult = nodeFindResult->second.find(moduleId);
			if ( moduleFindResult == nodeFindResult->second.end())
		    {
		        
		        nodeFindResult->second.insert(NodeMessageRouter::IntraNodeRoutingConfig::value_type(moduleId,pModuleDestInfo));
		        return true;
		        //return NULL;
		    }else {
		    	return false;
		    }
		}
	}
	bool NodeMessageRouter::ModuleUnregister(uint32_t nodeId, uint32_t moduleId)
	{
		NodeMessageRouter::InterNodeRoutingConfig::iterator nodeFindResult = m_RountingConfigs.find(nodeId);
		if (nodeFindResult == m_RountingConfigs.end())
		{
		    return false;
		}

		NodeMessageRouter::IntraNodeRoutingConfig::iterator moduleFindResult = nodeFindResult->second.find(moduleId);

		if ( moduleFindResult == nodeFindResult->second.end())
		{
		    return false;
		}
		nodeFindResult->second.erase( moduleFindResult);
		if(nodeFindResult->second.size()==0) {
			m_RountingConfigs.erase(nodeFindResult);
		}
		return true;
	}

    /**< If the specific Destination can be found, return the destination info's pointer, otherwise return NULL */
    ModuleDestinationInfo* NodeMessageRouter::GetDestination(uint32_t nodeId, uint32_t moduleId)
    {
        InterNodeRoutingConfig::iterator nodeFindResult = m_RountingConfigs.find(nodeId);

        if (nodeFindResult == m_RountingConfigs.end())
        {
            return NULL;
        }

        IntraNodeRoutingConfig::iterator moduleFindResult = nodeFindResult->second.find(moduleId);

        if ( moduleFindResult == nodeFindResult->second.end())
        {
            return NULL;
        }

        return moduleFindResult->second;
    }

    MessageSendResult NodeMessageRouter::SendMessageNew(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId ,const char* sourceModuleIdName,uint32_t destNodeId, const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte)
    {
        USE_UNFERENCED_PARAMETER(sourceNodeIdName);

        NodeMessageRouter* pMessageRouter = NodeMessageRouter::GetMessageRouter();

        if (NULL == pMessageRouter)
        {
            return MESSAGE_SEND_FAIL_ROUTER_NOT_AVAILABLE;
        }

        ModuleDestinationInfo* pDestInfo = pMessageRouter->GetDestination(destNodeId,destModuleId);

        if (NULL == pDestInfo)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_CRITICAL,"Message Send Fail(SendMessageNew),reason: MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND, destNodeIdName=%s, destModuleIdName=%s",destNodeIdName,destModuleIdName);
            return MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND;
        }

        MessageSendResult sendResult = pDestInfo->SendMessageNew(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);
        
        if (MESSAGE_SEND_SUCCESS == sendResult)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_DEBUG,"Message Got Send(SendMessageNew): [%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }
        else
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_ERROR,"Message Send Fail(SendMessageNew),[%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }

        return sendResult;
    }

    MessageSendResult NodeMessageRouter::CopyAndSendMessage(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId, const char* sourceModuleIdName,uint32_t destNodeId,const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, void* pMessage, uint32_t MessageSizeInByte)
    {
        USE_UNFERENCED_PARAMETER(sourceNodeIdName);

        NodeMessageRouter* pMessageRouter = NodeMessageRouter::GetMessageRouter();

        if (NULL == pMessageRouter)
        {
            return MESSAGE_SEND_FAIL_ROUTER_NOT_AVAILABLE;
        }

        ModuleDestinationInfo* pDestInfo = pMessageRouter->GetDestination(destNodeId,destModuleId);

        if (NULL == pDestInfo)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_CRITICAL,"Message Send Fail(CopyAndSendMessage),reason: MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND, destNodeIdName=%s, destModuleIdName=%s",destNodeIdName,destModuleIdName);
            return MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND;
        }

        MessageSendResult sendResult = pDestInfo->CopyAndSendMessage(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);

        if (MESSAGE_SEND_SUCCESS == sendResult)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_DEBUG,"Message Got Send(CopyAndSendMessage): [%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }
        else
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_ERROR,"Message Send Fail(CopyAndSendMessage),[%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }

        return sendResult;
    }

    MessageSendResult NodeMessageRouter::SendMessageMalloc(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId, const char* sourceModuleIdName,uint32_t destNodeId,const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, void* pMessage, uint32_t MessageSizeInByte)
    {
        USE_UNFERENCED_PARAMETER(sourceNodeIdName);

        NodeMessageRouter* pMessageRouter = NodeMessageRouter::GetMessageRouter();

        if (NULL == pMessageRouter)
        {
            return MESSAGE_SEND_FAIL_ROUTER_NOT_AVAILABLE;
        }

        ModuleDestinationInfo* pDestInfo = pMessageRouter->GetDestination(destNodeId,destModuleId);

        if (NULL == pDestInfo)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_CRITICAL,"Message Send Fail(SendMessageMalloc),reason: MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND, destNodeIdName=%s, destModuleIdName=%s",destNodeIdName,destModuleIdName);
            return MESSAGE_SEND_FAIL_DESTINATION_NOT_FOUND;
        }

        MessageSendResult sendResult = pDestInfo->SendMessageMalloc(sourceNodeId,sourceModuleId,messageIdentity,pMessage,MessageSizeInByte);

        if (MESSAGE_SEND_SUCCESS == sendResult)
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_DEBUG,"Message Got Send(SendMessageMalloc): [%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }
        else
        {
            Log_printf(pMessageRouter->m_RounterLogger,LOGGING_LEVEL_ERROR,"Message Send Fail(SendMessageMalloc),[%s]->[%s]: <%s>(Size=%d)",sourceModuleIdName,destModuleIdName,messageIdentityName,MessageSizeInByte);
        }

        return sendResult;
    }
}
