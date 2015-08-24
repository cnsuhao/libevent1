#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

#include <Platform/Include/Linux/PlatformEvent_Linux.h>


namespace CommunicationPlatform
{
    EventImplementation::EventImplementation(bool autoReset)
        :m_State(false)
    {
        if (pthread_mutex_init(&this->m_Mutex,NULL))
        {
            //Init fail
        }
        /* Mutex Init Success */
        else
        {
            if (pthread_cond_init(&m_Condition,NULL))
            {
                //Init fail
            }
        }
    }


    EventImplementation::~EventImplementation()
    {
        pthread_cond_destroy(&m_Condition);

        pthread_mutex_destroy(&m_Mutex);
    }

    bool EventImplementation::waitImpl()
    {
        if (0 != pthread_mutex_lock(&m_Mutex))
        {
            return false;
        }

        while (false == m_State) 
        {
            if (0 != pthread_cond_wait(&m_Condition, &m_Mutex))
            {
                pthread_mutex_unlock(&m_Mutex);

                return false;
            }
        }

        this->m_State = false;

        pthread_mutex_unlock(&m_Mutex);

        return true;
    }


    bool EventImplementation::waitImpl(long milliseconds)
    {
        int rc = 0;
        struct timespec abstime;

        struct timeval tv;
        gettimeofday(&tv, NULL);
        abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
        abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
        if (abstime.tv_nsec >= 1000000000)
        {
            abstime.tv_nsec -= 1000000000;
            abstime.tv_sec++;
        }

        if (pthread_mutex_lock(&m_Mutex) != 0)
        {
            return false;
        }

        while (!m_State) 
        {
            if ((rc = pthread_cond_timedwait(&m_Condition, &m_Mutex, &abstime)))
            {
                if (rc == ETIMEDOUT) break;
                pthread_mutex_unlock(&m_Mutex);
                return false;
            }
        }
        if (rc == 0) m_State = false;

        pthread_mutex_unlock(&m_Mutex);

        if (rc == 0)
        {
            return true;
        } 
        else
        {
            return false;
        }
    }
}