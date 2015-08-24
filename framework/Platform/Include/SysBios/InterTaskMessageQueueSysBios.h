#ifndef INTERTASKMESSAGEQUEUEGENERIC_SYSBIOS_H
#define INTERTASKMESSAGEQUEUEGENERIC_SYSBIOS_H

#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
#include <Platform/Include/InterTaskMessageBase.h>

namespace CommunicationPlatform
{
    class InterTaskMessage
    {
    public:
        enum MessageDataMemoryType
        {
            MESSAGE_DATA_MEMORY_TYPE_MALLOC  =  0,   // Data is allocated using malloc, and should be deallocated using free
            MESSAGE_DATA_MEMORY_TYPE_NEW     =  1,   // Data is allocated using new, and should be deallocated using delete
            MESSAGE_DATA_MEMORY_TYPE_NONE    =  2,   // No message data is send, just a message Id
            MESSAGE_DATA_MEMORY_TYPE_UNKNOWN =  3
        };

        InterTaskMessage();
        InterTaskMessage(uint32_t messageIdentity, uint32_t messageSizeInByte, uint32_t messageSourceNodeId, uint32_t messageSourceModuleId,MessageDataMemoryType messageMemoryType);
        ~InterTaskMessage();

        uint32_t m_MessageIdentity;
        uint32_t m_MessageSizeInByte;
        uint32_t m_MessageSourceNodeId;
        uint32_t m_MessageSourceModuleId;

        uint32_t m_MessageMemoryType;   /**< Memory type, enum InterTaskMessage::MessageDataMemoryType */ 

        union MessageDataBody
        {
            void*                    pMessageDataMalloc;
            InterModuleMessageBase*  pMessageDataNew;
        };

        MessageDataBody m_MessageData;
    };

    class InterTaskMessageQueueImplementation
        /// A NotificationQueue object provides a way to implement asynchronous
        /// notifications. This is especially useful for sending notifications
        /// from one thread to another, for example from a background thread to 
        /// the main (user interface) thread. 
        /// 
        /// The NotificationQueue can also be used to distribute work from
        /// a controlling thread to one or more worker threads. Each worker thread
        /// repeatedly calls waitDequeueNotification() and processes the
        /// returned notification. Special care must be taken when shutting
        /// down a queue with worker threads waiting for notifications.
        /// The recommended sequence to shut down and destroy the queue is to
        ///   1. set a termination flag for every worker thread
        ///   2. call the wakeUpAll() method
        ///   3. join each worker thread
        ///   4. destroy the notification queue.
    {
    public:
        InterTaskMessageQueueImplementation();
        /// Creates the NotificationQueue.

        ~InterTaskMessageQueueImplementation();
        /// Destroys the NotificationQueue.

        void enqueueNotification(InterTaskMessage* pNotification);
        /// Enqueues the given notification by adding it to
        /// the end of the queue (FIFO).
        /// The queue takes ownership of the notification, thus
        /// a call like
        ///     notificationQueue.enqueueNotification(new MyNotification);
        /// does not result in a memory leak.

        void enqueueUrgentNotification(InterTaskMessage* pNotification);
        /// Enqueues the given notification by adding it to
        /// the front of the queue (LIFO). The event therefore gets processed
        /// before all other events already in the queue.
        /// The queue takes ownership of the notification, thus
        /// a call like
        ///     notificationQueue.enqueueUrgentNotification(new MyNotification);
        /// does not result in a memory leak.

        InterTaskMessage* waitDequeueNotification();
        /// Dequeues the next pending notification.
        /// If no notification is available, waits for a notification
        /// to be enqueued. 
        /// The caller gains ownership of the notification and
        /// is expected to release it when done with it.
        /// This method returns 0 (null) if wakeUpWaitingThreads()
        /// has been called by another thread.
        ///
        /// It is highly recommended that the result is immediately
        /// assigned to a InterTaskMessage::Ptr, to avoid potential
        /// memory management issues.

        InterTaskMessage* waitDequeueNotification(long milliseconds);
        /// Dequeues the next pending notification.
        /// If no notification is available, waits for a notification
        /// to be enqueued up to the specified time.
        /// Returns 0 (null) if no notification is available.
        /// The caller gains ownership of the notification and
        /// is expected to release it when done with it.
        ///
        /// It is highly recommended that the result is immediately
        /// assigned to a InterTaskMessage::Ptr, to avoid potential
        /// memory management issues.

        void wakeUpAll();
        /// Wakes up all threads that wait for a notification.

        bool empty() const;
        /// Returns true iff the queue is empty.

        uint32_t size() const;
        /// Returns the number of notifications in the queue.

        void clear();
        /// Removes all notifications from the queue.

        bool hasIdleThreads() const;	
        /// Returns true if the queue has at least one thread waiting 
        /// for a notification.

    protected:

    private:
        Mailbox_Handle   m_MessageQueue;
        InterTaskMessage m_ReceiveMessageInstance;
    };
}



#endif
