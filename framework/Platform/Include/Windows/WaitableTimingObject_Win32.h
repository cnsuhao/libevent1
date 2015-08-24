#ifndef WAITABLETIMINGOBJECT_WIN32_H
#define WAITABLETIMINGOBJECT_WIN32_H

#include <Platform/Include/Windows/WindowsSpecificHeaders.h>

namespace CommunicationPlatform
{
    class WaitableTimingObjectImplementation
    {
    public:
        WaitableTimingObjectImplementation();
        virtual ~WaitableTimingObjectImplementation();
        bool Initialize();
        bool Wait(int64_t waitPeriodInMicroSeconds);
    private:
        HANDLE waitableObject; 
    };
}

#endif