#ifndef PLATFORMEVENT_LINUX_H
#define PLATFORMEVENT_LINUX_H

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

namespace CommunicationPlatform
{
    class EventImplementation
    {
    protected:
        EventImplementation(bool autoReset);		
        ~EventImplementation();
        void setImpl();
        bool waitImpl();
        bool waitImpl(long milliseconds);
        void resetImpl();
    private:
        volatile bool   m_State;
        pthread_mutex_t m_Mutex;
        pthread_cond_t  m_Condition;
    };


    //
    // inlines
    //
    inline void EventImplementation::setImpl()
    {
        if (0 != pthread_mutex_lock(&m_Mutex))
        {
            return;
        }

        this->m_State = true;

        if (0 != pthread_cond_broadcast(&m_Condition))
        {
            pthread_mutex_unlock(&m_Mutex);

            return;
        }

        pthread_mutex_unlock(&m_Mutex);
    }


    inline void EventImplementation::resetImpl()
    {
        if (0 != pthread_mutex_lock(&m_Mutex))
        {
            return;
        }

        this->m_State = false;

        pthread_mutex_unlock(&m_Mutex);
    }
}

#endif