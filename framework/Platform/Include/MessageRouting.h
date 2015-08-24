#ifndef MESSAGEROUTING_H
#define MESSAGEROUTING_H

#include <map>
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

#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/PlatformUDPSocket.h>
#include <Platform/Include/PlatformCommonClass.h>

namespace CommunicationPlatform
{
    class MessageTriggeredModuleRunner;
    class InterModuleMessageBase;
    class Logger;

    class ModuleDestinationInfo
    {
    public:
        ModuleDestinationInfo(uint32_t nodeId, uint32_t moduleId):m_NodeId(nodeId),m_ModuleId(moduleId){};
        ~ModuleDestinationInfo(){};

        /** 
        *    @brief  For intra-node, inter-module communication only, 
        *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte) = 0;            /**< For intra-node, inter-module communication only, pMessage should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination,  */

        /** 
        *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte) = 0;        /**< pMessage is allocted using C++ new operator, Message content pMessage is copied and sent to the destination */

        /** 
        *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte) = 0;         /**< pMessage should be allocated using C malloc operator, Message content pMessage is not copied and be directly sent to the destination,  */
    protected:
    //public:
        uint32_t m_NodeId;
        uint32_t m_ModuleId;
    private:
        ModuleDestinationInfo();
    };

    class IntraNodeModuleDestinationInfo: public ModuleDestinationInfo
    {
    public:
        IntraNodeModuleDestinationInfo(uint32_t nodeId, uint32_t moduleId,MessageTriggeredModuleRunner* destination);
        ~IntraNodeModuleDestinationInfo();
        
    protected:
        /** 
        *    @brief  For intra-node, inter-module communication only, 
        *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte);

        /** 
        *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);

        /** 
        *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);         /**< pMessage should be allocated using C malloc operator, Message content pMessage is not copied and be directly sent to the destination,  */
    private:
        MessageTriggeredModuleRunner* m_Destination;
        IntraNodeModuleDestinationInfo();
    };


    class InterNodeModuleUDPDestinationInfo: public ModuleDestinationInfo
    {
    public:
        InterNodeModuleUDPDestinationInfo(uint32_t nodeId, uint32_t moduleId, PlatformUDPSocket* targetModuleSocket);
        ~InterNodeModuleUDPDestinationInfo();
        
    protected:
        /** 
        *    @brief  For intra-node, inter-module communication only, 
        *    @param [in] pMessage           The message pointer, should be allocated using C++ new operator,  Message content pMessage is not copied and be directly sent to the destination, 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageNew(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte);

        /** 
        *    @brief  Copy and send message to any module on any node,  Message content pMessage is copied and sent to the destination 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult CopyAndSendMessage(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);

        /** 
        *    @brief  Send message to any module on any node,  pMessage should be allocated using C malloc operator,message content is not copied for intra-node and copied for inter-node communication 
        *    @param [in] pMessage           The message pointer 
        *    @param [in] MessageSizeInByte  How much data the pMessage is pointed to.
        */
        virtual MessageSendResult SendMessageMalloc(uint32_t sourceNodeId, uint32_t sourceModuleId, uint32_t messageIdentity,void* pMessage, uint32_t MessageSizeInByte);         /**< pMessage should be allocated using C malloc operator, Message content pMessage is not copied and be directly sent to the destination,  */
    private:
        PlatformUDPSocket* m_Destination;
        InterNodeModuleUDPDestinationInfo();

#define PLATFORM_UDP_SOCKET_CHANNEL_PACKET_ASSEMBLE_BUFFER_SIZE 65535
        uint8_t m_PacketAssembleBuffer[65535];
    };

    class InterNodeModuleUDPDestinationInfoEndPoint: public ModuleRunner
    {
    public:
        InterNodeModuleUDPDestinationInfoEndPoint(std::string moduleName);
        ~InterNodeModuleUDPDestinationInfoEndPoint();
        RunnerInitResult InitRunner();
        uint32_t RunEntry();

        uint16_t m_ListeningPort;
    protected:
        PlatformUDPSocket m_DataReceiveSocket;
#define PLATFORM_UDP_SOCKET_CHANNEL_PACKET_ASSEMBLE_BUFFER_SIZE 65535
        uint8_t m_PacketReceivingBuffer[65535];
    };

	class IRoutingConfiger;
	

    /* Message router for each node, each node only have one instance of this class */
    class NodeMessageRouter
    {
    public:
        static NodeMessageRouter* GetMessageRouter();
        bool InitializeRoutingConfig(IRoutingConfiger* pRoutingInitializer);       /**< Initialize the routing configuration using pRoutingInitializer */
        //add by caokun
        bool ModuleRegister(uint32_t nodeId, uint32_t moduleId, ModuleDestinationInfo* pModuleDestInfo);
        bool ModuleUnregister(uint32_t nodeId, uint32_t moduleId);
        //
        ModuleDestinationInfo* GetDestination(uint32_t nodeId, uint32_t moduleId); /**< If the specific Destination can be found, return the destination info's pointer, otherwise return NULL */


        static MessageSendResult SendMessageNew(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId ,const char* sourceModuleIdName,uint32_t destNodeId,const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, InterModuleMessageBase* pMessage, uint32_t MessageSizeInByte);

        static MessageSendResult CopyAndSendMessage(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId, const char* sourceModuleIdName,uint32_t destNodeId,const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, void* pMessage, uint32_t MessageSizeInByte);

        static MessageSendResult SendMessageMalloc(uint32_t sourceNodeId,const char* sourceNodeIdName, uint32_t sourceModuleId, const char* sourceModuleIdName,uint32_t destNodeId,const char* destNodeIdName,uint32_t destModuleId, const char* destModuleIdName,uint32_t messageIdentity,const char* messageIdentityName, void* pMessage, uint32_t MessageSizeInByte);         /**< pMessage should be allocated using C malloc operator, Message content pMessage is not copied and be directly sent to the destination,  */
        typedef std::map<uint32_t,ModuleDestinationInfo*> IntraNodeRoutingConfig;

        typedef std::map<uint32_t,IntraNodeRoutingConfig> InterNodeRoutingConfig;

        InterNodeRoutingConfig& GetRoutingConfig();

		Logger* m_RounterLogger;

    private:
        NodeMessageRouter();
        NodeMessageRouter(NodeMessageRouter const&){};             
        NodeMessageRouter& operator=(NodeMessageRouter const&){return *this;}
        static NodeMessageRouter*           m_pInstance;

        InterNodeRoutingConfig m_RountingConfigs; /**< Message routing configuration for this node */

    };

	/**
	 * @brief	Routing configuration initializer .
	 */
	class IRoutingConfiger
	{
	public:
		IRoutingConfiger(){};
		virtual ~IRoutingConfiger(){};

		virtual bool InitRoutingConfig(NodeMessageRouter::InterNodeRoutingConfig& interNodeRoutingConfig) = 0;
	};

/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is dynamically allocated using new operator
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using new operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                        destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                        messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)

/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is copied and then send
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is copied and then send
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define Message_CopyAndSend(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::CopyAndSendMessage(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
                                     
/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is dynamically allocated using malloc operator
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using malloc operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define Message_SendMalloc(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::SendMessageMalloc(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)



	/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is dynamically allocated using new operator
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using new operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
//#define Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                        destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                        messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)

/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is copied and then send
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is copied and then send
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
//#define Message_CopyAndSend(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::CopyAndSendMessage(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message directly through platform API, the message is dynamically allocated using malloc operator
 *
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using malloc operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
//#define Message_SendMalloc(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    NodeMessageRouter::SendMessageMalloc(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)

/**
 * @def	PlatformAgent_MessageSendNew(pPlatformAgent,sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message through platform agent object, the message is dynamically allocated using new operator
 *
 * @param   pPlatformAgent      Pointer to the platform agent object
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using new operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define PlatformAgent_MessageSendNew(pPlatformAgent,sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    if(pPlatformAgent) pPlatformAgent->SendMessageNew(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                        destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                        messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)

/**
 * @def	PlatformAgent_MessageCopyAndSend(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message through platform agent object, the message is copied and then send
 *
 * @param   pPlatformAgent      Pointer to the platform agent object
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is copied and then send
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define PlatformAgent_MessageCopyAndSend(pPlatformAgent,sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    if(pPlatformAgent) pPlatformAgent->CopyAndSendMessage(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
/**
 * @def	Message_SendNew(sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,
 * 		pMessage,MessageSizeInByte) NodeMessageRouter::SendMessageNew(sourceNodeId,#sourceNodeId,
 * 		sourceModuleId,#sourceModuleId, destNodeId,#destNodeId,destModuleId,#destModuleId,
 * 		messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
 *
 * @brief	Send Message through platform agent object, the message is dynamically allocated using malloc operator
 *
 * @param   pPlatformAgent      Pointer to the platform agent object
 * @param	sourceNodeId	 	Identifier for the source node.
 * @param	sourceModuleId   	Identifier for the source module.
 * @param	destNodeId		 	Identifier for the destination node.
 * @param	destModuleId	 	Identifier for the destination module.
 * @param	messageIdentity  	The message identity.
 * @param	pMessage		 	The message to send, which is dynamically allocated using malloc operator
 * @param	MessageSizeInByte	The message size in byte pointed by pMessage.
 */
#define PlatformAgent_MessageSendMalloc(pPlatformAgent,sourceNodeId,sourceModuleId,destNodeId,destModuleId,messageIdentity,pMessage,MessageSizeInByte) \
    if(pPlatformAgent) pPlatformAgent->SendMessageMalloc(sourceNodeId,#sourceNodeId,sourceModuleId,#sourceModuleId,\
                                            destNodeId,#destNodeId,destModuleId,#destModuleId, \
                                            messageIdentity,#messageIdentity,pMessage,MessageSizeInByte)
}


#endif
