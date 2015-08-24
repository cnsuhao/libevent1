#ifndef INTERTASKMESSAGEBASE_H
#define INTERTASKMESSAGEBASE_H

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
    /*
    *   Base class for all the intra-node, inter-module messages
    */
    class InterModuleMessageBase
    {
    public:
        InterModuleMessageBase(){};
        ~InterModuleMessageBase(){};
        virtual InterModuleMessageBase* Clone() = 0;
    };

    /**< Used for inter-node message exchange */
    class InterNodeTaskMessage
    {
    public:

        InterNodeTaskMessage();
        InterNodeTaskMessage(uint32_t messageIdentity, uint32_t messageSizeInByte, uint32_t messageSourceNodeId, uint32_t messageSourceModuleId);
        ~InterNodeTaskMessage();

        uint32_t GetMessageIdentity();
        uint32_t GetMessageSizeInByte();
        uint32_t GetSourceNodeId();
        uint32_t GetSourceModuleId();
        uint32_t GetDestNodeId();
        uint32_t GetDestModuleId();

#define INTER_NODE_MESSAGE_MESSAGE_ID_LEN (4)
        uint8_t m_MessageIdentity[INTER_NODE_MESSAGE_MESSAGE_ID_LEN];
#define INTER_NODE_MESSAGE_MESSAGE_SIZE_LEN (4)
        uint8_t m_MessageSizeInByte[INTER_NODE_MESSAGE_MESSAGE_SIZE_LEN];
#define INTER_NODE_MESSAGE_MESSAGE_SOURCE_NODE_ID_LEN (4)
        uint8_t m_MessageSourceNodeId[INTER_NODE_MESSAGE_MESSAGE_SOURCE_NODE_ID_LEN];
#define INTER_NODE_MESSAGE_MESSAGE_SOURCE_MODULE_ID_LEN (4)
        uint8_t m_MessageSourceModuleId[INTER_NODE_MESSAGE_MESSAGE_SOURCE_MODULE_ID_LEN];
#define INTER_NODE_MESSAGE_MESSAGE_DEST_NODE_ID_LEN (4)
        uint8_t m_MessageDestNodeId[INTER_NODE_MESSAGE_MESSAGE_DEST_NODE_ID_LEN];
#define INTER_NODE_MESSAGE_MESSAGE_DEST_MODULE_ID_LEN (4)
        uint8_t m_MessageDestModuleId[INTER_NODE_MESSAGE_MESSAGE_DEST_MODULE_ID_LEN];

        uint8_t messageBody[1];
    };
}


#endif
