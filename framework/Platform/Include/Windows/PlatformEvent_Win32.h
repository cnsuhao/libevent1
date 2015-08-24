#ifndef PLATFORMEVENT_WIN32_H
#define PLATFORMEVENT_WIN32_H

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>


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
        HANDLE _event;
    };


    //
    // inlines
    //
    inline void EventImplementation::setImpl()
    {
        SetEvent(_event);
    }


    inline void EventImplementation::resetImpl()
    {
        ResetEvent(_event);
    }

}


#endif