#include <Platform/Include/Linux/WaitableTimingObject_Linux.h>

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
        timespec requestedTimeout;
        requestedTimeout.tv_sec  = waitPeriodInMicroSeconds/1000000;        //in seconds
        requestedTimeout.tv_nsec = (waitPeriodInMicroSeconds%1000000)*1000; //in nanoseconds

        timespec remainedTimeout;
        int nanosleepResult = 0;
        for (;;)
        {
            nanosleepResult = nanosleep(&requestedTimeout,&remainedTimeout);

            /* On successfully sleeping for the requested interval, nanosleep() returns 0. If the call is interrupted by a signal handler or encounters an error, then it returns -1, with errno set to indicate the error. */
            if (0 == nanosleepResult)
            {
                break;
            } 
            /* nanosleep fail */
            else
            {
                if (EINTR == errno)
                {
                    requestedTimeout = remainedTimeout;
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }

        return true;
    }
}