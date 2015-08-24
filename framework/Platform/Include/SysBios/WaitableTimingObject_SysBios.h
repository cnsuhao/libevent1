#ifndef WAITABLETIMINGOBJECT_SYSBIOS_H
#define WAITABLETIMINGOBJECT_SYSBIOS_H

#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>

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
