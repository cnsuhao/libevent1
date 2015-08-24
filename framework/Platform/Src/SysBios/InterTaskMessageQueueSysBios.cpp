
#include <Platform/Include/SysBios/InterTaskMessageQueueSysBios.h>

namespace CommunicationPlatform
{
    InterTaskMessageQueueImplementation::InterTaskMessageQueueImplementation()
    {
        this->m_MessageQueue = Mailbox_create(sizeof(InterTaskMessage),4096,NULL,NULL);
    }

    InterTaskMessageQueueImplementation::~InterTaskMessageQueueImplementation()
    {
        Mailbox_delete(&this->m_MessageQueue); 
    }

    void InterTaskMessageQueueImplementation::enqueueNotification(InterTaskMessage* theNotification)
    {
        if (TRUE == Mailbox_post(this->m_MessageQueue,theNotification,BIOS_NO_WAIT))
        {
            //Send success
        } 
        else
        {
            //Send fail
        }
        
        delete theNotification;

    }

    void InterTaskMessageQueueImplementation::enqueueUrgentNotification(InterTaskMessage* pNotification)
    {
        if (TRUE == Mailbox_post(this->m_MessageQueue,pNotification,BIOS_NO_WAIT))
        {
            //Send success
        } 
        else
        {
            //Send fail
        }

        delete pNotification;
    }

    InterTaskMessage* InterTaskMessageQueueImplementation::waitDequeueNotification()
    {
        if (TRUE == Mailbox_pend(this->m_MessageQueue,&m_ReceiveMessageInstance,BIOS_WAIT_FOREVER))
        {
            return &m_ReceiveMessageInstance;
        } 
        else
        {
            return NULL;
        }
    }


    InterTaskMessage* InterTaskMessageQueueImplementation::waitDequeueNotification(long milliseconds)
    {
        if (TRUE == Mailbox_pend(this->m_MessageQueue,&m_ReceiveMessageInstance,ConvertMicroSecondsToSystemTick(milliseconds*1000)))
        {
            return &m_ReceiveMessageInstance;
        } 
        else
        {
            return NULL;
        }
    }

    void InterTaskMessageQueueImplementation::wakeUpAll()
    {
        //Unsupported Interface
    }


    bool InterTaskMessageQueueImplementation::empty() const
    {
        //Unsupported Interface
        return true;
    }


    uint32_t InterTaskMessageQueueImplementation::size() const
    {
        return (uint32_t)Mailbox_getNumPendingMsgs(this->m_MessageQueue);
    }


    void InterTaskMessageQueueImplementation::clear()
    {
        //Unsupported Interface
    }


    bool InterTaskMessageQueueImplementation::hasIdleThreads() const
    {
        //Unsupported Interface
        return true;
    }
}
