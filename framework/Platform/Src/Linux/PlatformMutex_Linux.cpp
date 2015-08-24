#include <Platform/Include/Linux/PlatformMutex_Linux.h>

namespace CommunicationPlatform
{
    MutexImplementation::MutexImplementation()
    {
        /* If successful, the pthread_mutex_destroy() and pthread_mutex_init() functions shall return zero; otherwise, an error number shall be returned to indicate the error */
        if ( pthread_mutex_init(&this->m_Mutex,NULL) )
        {
            //Initialize Mutex error
        }
    }

    MutexImplementation::~MutexImplementation()
    {
        /* If successful, the pthread_mutex_destroy() and pthread_mutex_init() functions shall return zero; otherwise, an error number shall be returned to indicate the error */
        if ( pthread_mutex_destroy(&this->m_Mutex) )
        {
            //Destroy Mutex error
        }
    }

    void MutexImplementation::Lock()
    {
        /* If successful, the pthread_mutex_lock() and pthread_mutex_unlock() functions shall return zero; otherwise, an error number shall be returned to indicate the error. */
        if ( pthread_mutex_lock(&this->m_Mutex) )
        {
            //Lock Error;
        }
    }

    bool MutexImplementation::TryLock()
    {
        /* The pthread_mutex_trylock() function shall return zero if a lock on the mutex object referenced by mutex is acquired. Otherwise, an error number is returned to indicate the error. */
        if ( 0 == pthread_mutex_trylock(&this->m_Mutex) )
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
        /* If successful, the pthread_mutex_lock() and pthread_mutex_unlock() functions shall return zero; otherwise, an error number shall be returned to indicate the error. */
        if ( pthread_mutex_unlock(&this->m_Mutex) )
        {
            //UnLock Error;
        }
    }
}

