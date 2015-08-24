#include <Platform/Include/PlatformAPI.h>
#include <Platform/Include/InterTaskMessage.h>

namespace CommunicationPlatform
{
    InterTaskMessage::InterTaskMessage()
        :m_MessageIdentity(0),
        m_MessageSizeInByte(0),
        m_MessageSourceNodeId(0),
        m_MessageSourceModuleId(0),
        m_MessageMemoryType(MESSAGE_DATA_MEMORY_TYPE_UNKNOWN)
    {
        m_MessageData.pMessageDataMalloc = NULL;
        m_MessageData.pMessageDataNew    = NULL;
    }

    InterTaskMessage::InterTaskMessage(uint32_t messageIdentity, uint32_t messageSizeInByte, uint32_t messageSourceNodeId, uint32_t messageSourceModuleId,MessageDataMemoryType messageMemoryType)
        :m_MessageIdentity(messageIdentity),
        m_MessageSizeInByte(messageSizeInByte),
        m_MessageSourceNodeId(messageSourceNodeId),
        m_MessageSourceModuleId(messageSourceModuleId),
        m_MessageMemoryType(messageMemoryType)
    {
        m_MessageData.pMessageDataMalloc = NULL;
        m_MessageData.pMessageDataNew    = NULL;
    }


    InterTaskMessage::~InterTaskMessage()
    {
        switch (this->m_MessageMemoryType)
        {
        case MESSAGE_DATA_MEMORY_TYPE_MALLOC:
            {
                free(this->m_MessageData.pMessageDataMalloc);
                this->m_MessageData.pMessageDataMalloc = NULL;
            }
            break;
        case MESSAGE_DATA_MEMORY_TYPE_NEW:
            {
                delete this->m_MessageData.pMessageDataNew;
                this->m_MessageData.pMessageDataNew = NULL;
            }
            break;
        case MESSAGE_DATA_MEMORY_TYPE_NONE:
            {
                //Do nothing
            }
            break;
        default:
            break;
        }
    }

    InterTaskMessageQueue::InterTaskMessageQueue():InterTaskMessageQueueImplementation()
    {
    }


    InterTaskMessageQueue::~InterTaskMessageQueue()
    {
        InterTaskMessageQueueImplementation::clear();
    }


    void InterTaskMessageQueue::enqueueNotification(InterTaskMessage* pNotification)
    {
        InterTaskMessageQueueImplementation::enqueueNotification(pNotification);
    }


    void InterTaskMessageQueue::enqueueUrgentNotification(InterTaskMessage* pNotification)
    {
        InterTaskMessageQueueImplementation::enqueueUrgentNotification(pNotification);
    }


    InterTaskMessage* InterTaskMessageQueue::waitDequeueNotification()
    {
        return InterTaskMessageQueueImplementation::waitDequeueNotification();
    }


    InterTaskMessage* InterTaskMessageQueue::waitDequeueNotification(long milliseconds)
    {
        return InterTaskMessageQueueImplementation::waitDequeueNotification(milliseconds);
    }

    void InterTaskMessageQueue::wakeUpAll()
    {
        InterTaskMessageQueueImplementation::wakeUpAll();
    }


    bool InterTaskMessageQueue::empty() const
    {
        return InterTaskMessageQueueImplementation::empty();
    }


    uint32_t InterTaskMessageQueue::size() const
    {
        return InterTaskMessageQueueImplementation::size();
    }


    void InterTaskMessageQueue::clear()
    {
        InterTaskMessageQueueImplementation::clear();	
    }


    bool InterTaskMessageQueue::hasIdleThreads() const
    {
        return InterTaskMessageQueueImplementation::hasIdleThreads();
    }

}
