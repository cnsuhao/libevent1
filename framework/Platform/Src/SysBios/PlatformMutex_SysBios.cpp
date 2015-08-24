#include <Platform/Include/SysBios/PlatformMutex_SysBios.h>

namespace CommunicationPlatform
{
    MutexImplementation::MutexImplementation()
    {
        this->m_Mutex = Semaphore_create(1,NULL,NULL);
    }

    MutexImplementation::~MutexImplementation()
    {
        Semaphore_delete(&this->m_Mutex);
    }

    void MutexImplementation::Lock()
    {
        Semaphore_pend(this->m_Mutex,BIOS_WAIT_FOREVER);
    }

    bool MutexImplementation::TryLock()
    {
        if (TRUE == Semaphore_pend(this->m_Mutex,BIOS_NO_WAIT))
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
        Semaphore_post(this->m_Mutex);
    }
}

