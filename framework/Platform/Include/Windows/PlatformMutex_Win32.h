#ifndef PLATFORMMUTEX_WIN32_H
#define PLATFORMMUTEX_WIN32_H

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>

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
        CRITICAL_SECTION m_CSLock;
    };
}

#endif