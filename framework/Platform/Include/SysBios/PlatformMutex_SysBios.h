#ifndef PLATFORMMUTEX_SYSBIOS_H
#define PLATFORMMUTEX_SYSBIOS_H

#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>

namespace CommunicationPlatform
{
    class MutexImplementation
    {
    public:
        MutexImplementation();
        virtual ~MutexImplementation();

        void Lock();

        bool TryLock();

        void Unlock();
    protected:
        Semaphore_Handle m_Mutex;
    };
}


#endif
