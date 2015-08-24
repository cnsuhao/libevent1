#include <Platform/Include/Windows/PlatformMutex_Win32.h>

namespace CommunicationPlatform
{
    MutexImplementation::MutexImplementation()
    {
        InitializeCriticalSection(&(this->m_CSLock));
    }

    MutexImplementation::~MutexImplementation()
    {
        DeleteCriticalSection(&(this->m_CSLock));
    }

    void MutexImplementation::Lock()
    {
        EnterCriticalSection(&(this->m_CSLock));
    }

	bool MutexImplementation::TryLock()
	{
		 if(TryEnterCriticalSection(&(this->m_CSLock)))
         {
             return true;
         }
         else
         {
             return false;
         }
	}

    void MutexImplementation::Unlock()
    {
        LeaveCriticalSection(&(this->m_CSLock));
    }
}

