#include <Platform/Include/InterTaskMessageQueueGeneric.h>

namespace CommunicationPlatform
{
    InterTaskMessageQueueImplementation::InterTaskMessageQueueImplementation()
    {
    
    }

    InterTaskMessageQueueImplementation::~InterTaskMessageQueueImplementation()
    {

    }

    void InterTaskMessageQueueImplementation::enqueueNotification(InterTaskMessage::AutoInterTaskMsgPtr pNotification)
    {
        Mutex::ScopedMutex lock(_mutex);
        if (_waitQueue.empty())
        {
            _nfQueue.push_back(pNotification);
        }
        else
        {
            WaitInfo* pWI = _waitQueue.front();
            _waitQueue.pop_front();
            pWI->pNf = pNotification;
            pWI->nfAvailable.set();
        }	
    }


    void InterTaskMessageQueueImplementation::enqueueUrgentNotification(InterTaskMessage::AutoInterTaskMsgPtr pNotification)
    {
        Mutex::ScopedMutex lock(_mutex);
        if (_waitQueue.empty())
        {
            _nfQueue.push_front(pNotification);
        }
        else
        {
            WaitInfo* pWI = _waitQueue.front();
            _waitQueue.pop_front();
            pWI->pNf = pNotification;
            pWI->nfAvailable.set();
        }	
    }

    InterTaskMessage* InterTaskMessageQueueImplementation::waitDequeueNotification()
    {
        InterTaskMessage::AutoInterTaskMsgPtr pNf;
        WaitInfo* pWI = 0;
        {
            Mutex::ScopedMutex lock(_mutex);
            pNf = dequeueOne();
            if (pNf) return pNf.duplicate();
            pWI = new WaitInfo;
            _waitQueue.push_back(pWI);
        }
        pWI->nfAvailable.wait();
        pNf = pWI->pNf;
        delete pWI;
        return pNf.duplicate();
    }


    InterTaskMessage* InterTaskMessageQueueImplementation::waitDequeueNotification(long milliseconds)
    {
        InterTaskMessage::AutoInterTaskMsgPtr pNf;
        WaitInfo* pWI = 0;
        {
            Mutex::ScopedMutex lock(_mutex);
            pNf = dequeueOne();
            if (pNf) return pNf.duplicate();
            pWI = new WaitInfo;
            _waitQueue.push_back(pWI);
        }
        if (pWI->nfAvailable.tryWait(milliseconds))
        {
            pNf = pWI->pNf;
        }
        else
        {
            Mutex::ScopedMutex lock(_mutex);
            pNf = pWI->pNf;
            for (WaitQueue::iterator it = _waitQueue.begin(); it != _waitQueue.end(); ++it)
            {
                if (*it == pWI)
                {
                    _waitQueue.erase(it);
                    break;
                }
            }
        }
        delete pWI;
        return pNf.duplicate();
    }

    void InterTaskMessageQueueImplementation::wakeUpAll()
    {
        Mutex::ScopedMutex lock(_mutex);
        for (WaitQueue::iterator it = _waitQueue.begin(); it != _waitQueue.end(); ++it)
        {
            (*it)->nfAvailable.set();
        }
        _waitQueue.clear();
    }


    bool InterTaskMessageQueueImplementation::empty() const
    {
        Mutex::ScopedMutex lock(_mutex);
        return _nfQueue.empty();
    }


    uint32_t InterTaskMessageQueueImplementation::size() const
    {
        Mutex::ScopedMutex lock(_mutex);
        return static_cast<uint32_t>(_nfQueue.size());
    }


    void InterTaskMessageQueueImplementation::clear()
    {
        Mutex::ScopedMutex lock(_mutex);
        _nfQueue.clear();	
    }


    bool InterTaskMessageQueueImplementation::hasIdleThreads() const
    {
        Mutex::ScopedMutex lock(_mutex);
        return !_waitQueue.empty();
    }


    InterTaskMessage::AutoInterTaskMsgPtr InterTaskMessageQueueImplementation::dequeueOne()
    {
        InterTaskMessage::AutoInterTaskMsgPtr pNf;
        if (!_nfQueue.empty())
        {
            pNf = _nfQueue.front();
            _nfQueue.pop_front();
        }
        return pNf;
    }
}
