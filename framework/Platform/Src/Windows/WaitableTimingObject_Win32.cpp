#include <Platform/Include/Windows/WaitableTimingObject_Win32.h>

namespace CommunicationPlatform
{
    WaitableTimingObjectImplementation::WaitableTimingObjectImplementation()
        :waitableObject(NULL)
    {
        
    }

    WaitableTimingObjectImplementation::~WaitableTimingObjectImplementation()
    {
        CloseHandle(this->waitableObject);
    }

    bool WaitableTimingObjectImplementation::Initialize()
    {
        this->waitableObject = CreateWaitableTimer(NULL, FALSE, NULL);

        if(NULL == this->waitableObject)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool WaitableTimingObjectImplementation::Wait(int64_t waitPeriodInMicroSeconds)
    {
        LARGE_INTEGER liDueTime;

        liDueTime.QuadPart= -1 * (waitPeriodInMicroSeconds * 10);

        if (FALSE == SetWaitableTimer(this->waitableObject,&liDueTime,0,NULL,NULL,FALSE))
        {
            return false;
        }

        if (WaitForSingleObject(this->waitableObject, INFINITE) != WAIT_OBJECT_0)
        {
           return false;
        }
        else
        {
            return true;
        }
    }
}