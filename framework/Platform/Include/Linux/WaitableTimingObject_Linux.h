#ifndef WAITABLETIMINGOBJECT_LINUX_H
#define WAITABLETIMINGOBJECT_LINUX_H

#include <Platform/Include/Linux/LinuxSpecificHeaders.h>

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
    };
}

#endif