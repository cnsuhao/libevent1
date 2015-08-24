#include <Platform/Include/SysBios/WaitableTimingObject_SysBios.h>

namespace CommunicationPlatform
{
    WaitableTimingObjectImplementation::WaitableTimingObjectImplementation()
    {

    }

    WaitableTimingObjectImplementation::~WaitableTimingObjectImplementation()
    {

    }

    bool WaitableTimingObjectImplementation::Initialize()
    {
        return true;
    }

    bool WaitableTimingObjectImplementation::Wait(int64_t waitPeriodInMicroSeconds)
    {
        Task_sleep(ConvertMicroSecondsToSystemTick(waitPeriodInMicroSeconds));

        return true;
    }

}
