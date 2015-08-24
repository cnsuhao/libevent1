#include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#include <Platform/Include/Windows/PlatformEvent_Win32.h>


namespace CommunicationPlatform
{
    EventImplementation::EventImplementation(bool autoReset)
    {
        _event = CreateEvent(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
    }


    EventImplementation::~EventImplementation()
    {
        CloseHandle(_event);
    }


    bool EventImplementation::waitImpl()
    {
        switch (WaitForSingleObject(_event, INFINITE))
        {
        case WAIT_OBJECT_0:
            return true;
        default:
            return false;
        }
    }


    bool EventImplementation::waitImpl(long milliseconds)
    {
        switch (WaitForSingleObject(_event, milliseconds + 1))
        {
        case WAIT_TIMEOUT:
            return false;
        case WAIT_OBJECT_0:
            return true;
        default:
            return false;		
        }
    }
}