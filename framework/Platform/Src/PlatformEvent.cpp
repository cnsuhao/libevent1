#include <Platform/Include/PlatformEvent.h>


namespace CommunicationPlatform
{
    Event::Event(bool autoReset): EventImplementation(autoReset)
    {
    }

    Event::~Event()
    {
    
    }

}
