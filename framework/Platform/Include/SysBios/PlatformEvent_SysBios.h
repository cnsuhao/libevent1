#ifndef PLATFORMEVENT_SYSBIOS_H
#define PLATFORMEVENT_SYSBIOS_H

#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>

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
        Event_Handle m_EventHandle;
    };

    inline void EventImplementation::setImpl()
    {
        Event_post(this->m_EventHandle,Event_Id_00);
    }

    inline void EventImplementation::resetImpl()
    {
        //Not supported on SYSBIOS
    }
}

#endif
