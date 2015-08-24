#include <Platform/Include/PlatformCommonMacros.h>
#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
#include <Platform/Include/SysBios/PlatformEvent_SysBios.h>


namespace CommunicationPlatform
{
    EventImplementation::EventImplementation(bool autoReset)
    {
        USE_UNFERENCED_PARAMETER(autoReset);

        this->m_EventHandle = Event_create(NULL,NULL);
    }

    EventImplementation::~EventImplementation()
    {
        Event_delete(&this->m_EventHandle);
    }

    bool EventImplementation::waitImpl()
    {
        /* Return value: All consumed events or zero if timeout. */
        if (0 != Event_pend(this->m_EventHandle,Event_Id_NONE,Event_Id_00,BIOS_WAIT_FOREVER))
        {
            return true;
        }
        else
        {
            /* Event timeout */
            return false;
        }
    }


    bool EventImplementation::waitImpl(long milliseconds)
    {
        /* Return value: All consumed events or zero if timeout. */
        if (0 != Event_pend(this->m_EventHandle,Event_Id_NONE,Event_Id_00,ConvertMicroSecondsToSystemTick(milliseconds*1000)))
        {
            return true;
        }
        else
        {
            /* Event timeout */
            return false;
        }
    }
}
