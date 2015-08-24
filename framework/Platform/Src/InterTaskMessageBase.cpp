#include <Platform/Include/InterTaskMessageBase.h>

namespace CommunicationPlatform
{
    InterNodeTaskMessage::InterNodeTaskMessage()
    {

    }

    InterNodeTaskMessage::InterNodeTaskMessage(uint32_t messageIdentity, uint32_t messageSizeInByte, uint32_t messageSourceNodeId, uint32_t messageSourceModuleId)
    {
        this->m_MessageIdentity[0] = (uint8_t)(messageIdentity>>24);
        this->m_MessageIdentity[1] = (uint8_t)(messageIdentity>>16);
        this->m_MessageIdentity[2] = (uint8_t)(messageIdentity>>8);
        this->m_MessageIdentity[3] = (uint8_t)messageIdentity;


        this->m_MessageSizeInByte[0] = (uint8_t)(messageSizeInByte>>24);
        this->m_MessageSizeInByte[1] = (uint8_t)(messageSizeInByte>>16);
        this->m_MessageSizeInByte[2] = (uint8_t)(messageSizeInByte>>8);
        this->m_MessageSizeInByte[3] = (uint8_t)messageSizeInByte;

        this->m_MessageSourceNodeId[0] = (uint8_t)(messageSourceNodeId>>24);
        this->m_MessageSourceNodeId[1] = (uint8_t)(messageSourceNodeId>>16);
        this->m_MessageSourceNodeId[2] = (uint8_t)(messageSourceNodeId>>8);
        this->m_MessageSourceNodeId[3] = (uint8_t)messageSourceNodeId;

        this->m_MessageSourceModuleId[0] = (uint8_t)(messageSourceModuleId>>24);
        this->m_MessageSourceModuleId[1] = (uint8_t)(messageSourceModuleId>>16);
        this->m_MessageSourceModuleId[2] = (uint8_t)(messageSourceModuleId>>8);
        this->m_MessageSourceModuleId[3] = (uint8_t)messageSourceModuleId;
    }

    InterNodeTaskMessage::~InterNodeTaskMessage()
    {

    }

    uint32_t InterNodeTaskMessage::GetMessageIdentity()
    {
        uint32_t messageIdentity = this->m_MessageIdentity[0]<<24 |
                                   this->m_MessageIdentity[1]<<16 |
                                   this->m_MessageIdentity[2]<<8  |
                                   this->m_MessageIdentity[3];

        return messageIdentity;
    }

    uint32_t InterNodeTaskMessage::GetMessageSizeInByte()
    {
        uint32_t messageSize = this->m_MessageSizeInByte[0]<<24 |
                               this->m_MessageSizeInByte[1]<<16 |
                               this->m_MessageSizeInByte[2]<<8  |
                               this->m_MessageSizeInByte[3];

        return messageSize;
    }

    uint32_t InterNodeTaskMessage::GetSourceNodeId()
    {
        uint32_t sourceNodeId = this->m_MessageSourceNodeId[0]<<24 |
                                this->m_MessageSourceNodeId[1]<<16 |
                                this->m_MessageSourceNodeId[2]<<8  |
                                this->m_MessageSourceNodeId[3];

        return sourceNodeId;
    }

    uint32_t InterNodeTaskMessage::GetSourceModuleId()
    {
        uint32_t sourceModuleId = this->m_MessageSourceModuleId[0]<<24 |
                                  this->m_MessageSourceModuleId[1]<<16 |
                                  this->m_MessageSourceModuleId[2]<<8  |
                                  this->m_MessageSourceModuleId[3];

        return sourceModuleId;
    }

    uint32_t InterNodeTaskMessage::GetDestNodeId()
    {
        uint32_t destinationNodeId = this->m_MessageDestNodeId[0]<<24 |
                                       this->m_MessageDestNodeId[1]<<16 |
                                       this->m_MessageDestNodeId[2]<<8  |
                                       this->m_MessageDestNodeId[3];

        return destinationNodeId;
    }

    uint32_t InterNodeTaskMessage::GetDestModuleId()
    {
        uint32_t destinationModuleId = this->m_MessageDestModuleId[0]<<24 |
                                       this->m_MessageDestModuleId[1]<<16 |
                                       this->m_MessageDestModuleId[2]<<8  |
                                       this->m_MessageDestModuleId[3];

        return destinationModuleId;
    }

}