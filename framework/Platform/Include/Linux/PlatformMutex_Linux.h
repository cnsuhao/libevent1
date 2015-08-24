#ifndef PLATFORMMUTEX_LINUX_H
#define PLATFORMMUTEX_LINUX_H

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

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
        pthread_mutex_t m_Mutex;
    };
}

#endif