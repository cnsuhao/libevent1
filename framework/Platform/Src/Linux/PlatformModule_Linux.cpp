#include <Platform/Include/Linux/LinuxSpecificHeaders.h>
#include <Platform/Include/Linux/PlatformModule_Linux.h>
#include <Platform/Include/InterTaskMessage.h>

namespace CommunicationPlatform
{
    BasicTaskModuleImplementation::BasicTaskModuleImplementation(ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, ModuleRunner* moduleRunner)
        :m_Priority(modulePriority),
        m_StackSize(moduleStackSize),
        m_ModuleName(moduleName),
        m_ModuleRunner(moduleRunner)
    {
    }

    BasicTaskModuleImplementation::~BasicTaskModuleImplementation()
    {

    }

    ModuleCreateResult BasicTaskModuleImplementation::Start()
    {
        pthread_attr_t threadAttr;

        pthread_attr_init(&threadAttr);

        /* Set stack size */
        /* On success, these functions return 0; on error, they return a nonzero error number. */
        /* pthread_attr_setstacksize() can fail with the following error:
        EINVAL: The stack size is less than PTHREAD_STACK_MIN (16384) bytes.
        On some systems, pthread_attr_setstacksize() can fail with the error EINVAL if stacksize is not a multiple of the system page size. */
        if (0 != pthread_attr_setstacksize(&threadAttr,this->m_StackSize))
        {
            pthread_attr_destroy(&threadAttr);
            return TASK_CREATE_RESULT_FAIL_OTHER_REASON;
        }

        /* Set scheduling policy */
        /* On success, these functions return 0; on error, they return a nonzero error number. */
        /* */
        if (0 != pthread_attr_setschedpolicy(&threadAttr,SCHED_RR))
        {
            pthread_attr_destroy(&threadAttr);
            return TASK_CREATE_RESULT_FAIL_OTHER_REASON;
        }

        /* Set thread scheduling priority */
        sched_param scheculeParam;
        scheculeParam.sched_priority = CalculatePthreadPriorityValue(this->m_Priority);

        /* On success, these functions return 0; on error, they return a nonzero error number. */
        if (0 != pthread_attr_setschedparam(&threadAttr,&scheculeParam))
        {
            pthread_attr_destroy(&threadAttr);
            return TASK_CREATE_RESULT_FAIL_SET_PRIORITY_ERROR;
        }

        /* The pthread_attr_setinheritsched() function sets the inherit scheduler attribute of 
        the thread attributes object referred to by attr to the value specified in inheritsched.
        The inherit scheduler attribute determines whether a thread created using the thread attributes 
        object attr will inherit its scheduling attributes from the calling thread or whether it will take them from attr. */
        /*PTHREAD_INHERIT_SCHED
        Threads that are created using attr inherit scheduling attributes from the creating thread; the scheduling attributes in attr are ignored. 
        PTHREAD_EXPLICIT_SCHED
        Threads that are created using attr take their scheduling attributes from the values specified by the attributes object. 
        The default setting of the inherit scheduler attribute in a newly initialized thread attributes object is PTHREAD_INHERIT_SCHED. */
        if (0 != pthread_attr_setinheritsched(&threadAttr,PTHREAD_EXPLICIT_SCHED))
        {
            pthread_attr_destroy(&threadAttr);
            return TASK_CREATE_RESULT_FAIL_OTHER_REASON;
        }

		if (ModuleRunner::RUNNER_INIT_SUCCESS != this->m_ModuleRunner->InitRunner())
		{
			return TASK_CREATE_RESULT_FAIL_RUNNER_INIT_FAIL;
		}

        int errorResult = pthread_create(&m_ThreadId,&threadAttr,&ModuleEntry,this);
        /* On success, pthread_create() returns 0; on error, it returns an error number, and the contents of *thread are undefined. */
        if (errorResult )
        {
            switch(errorResult)
            {
            case EAGAIN:
                {
                    //TODO://printf("pthread_create return result = EAGAIN\r\n");
                }
                break;
            case EINVAL:
                {
                    //TODO://printf("pthread_create return result = EINVAL\r\n");
                }
                break;
            case EPERM:
                {
                    //TODO://printf("pthread_create return result = EPERM\r\n");
                }
                break;
            default:
                {
                    //TODO://printf("pthread_create return result = %d\r\n",errorResult);
                }
                break;
            }

            pthread_attr_destroy(&threadAttr);

            return TASK_CREATE_RESULT_FAIL_BEGIN_THREAD_ERROR;
        }

        return TASK_CREATE_RESULT_SUCCESS;
    }

    int BasicTaskModuleImplementation::CalculatePthreadPriorityValue(ModulePriority abstractPriority)
    {
        int minPriorityInSystem = sched_get_priority_min(SCHED_RR);
        int maxPriorityInSystem = sched_get_priority_max(SCHED_RR);

        int priorityScope = maxPriorityInSystem - minPriorityInSystem;

        switch (abstractPriority)
        {
        case TASK_PRIORITY_HIGHEST:
            {
                return maxPriorityInSystem;
            }
            break;
        case TASK_PRIORITY_HIGH:
            {
                return (minPriorityInSystem + 7*(priorityScope/8));
            }
            break;
        case TASK_PRIORITY_MIDDLE_HIGH:
            {
                return (minPriorityInSystem + 3*(priorityScope/4));
            }
            break;
        case TASK_PRIORITY_MIDDLE_LOW:
            {
                return (minPriorityInSystem + (priorityScope/4));
            }
            break;
        case TASK_PRIORITY_LOW:
            {
                return (minPriorityInSystem + (priorityScope/8));
            }
            break;
        case TASK_PRIORITY_LOWEST:
            {
                return minPriorityInSystem;
            }
            break;
        case TASK_PRIORITY_MIDDLE:
        default:
            {
                return (minPriorityInSystem + (priorityScope/2));
            }
            break;
        }
    }

    uint32_t BasicTaskModuleImplementation::ModuleEntryReturnValue = 0;

    void* BasicTaskModuleImplementation::ModuleEntry(void* pThread)
    {
        BasicTaskModuleImplementation* pCurrentThread = (BasicTaskModuleImplementation*)pThread;

        ModuleEntryReturnValue = pCurrentThread->RunEntry();

        return (&ModuleEntryReturnValue);
    }

    bool BasicTaskModuleImplementation::Join()
    {
        void* resultData;
        pthread_join(this->m_ThreadId,&resultData);

        return true;
    }

    void BasicTaskModuleImplementation::Sleep(uint32_t millisecondsToSleep)
    {
        timespec requestTimeOutPeriod;
        requestTimeOutPeriod.tv_sec  = millisecondsToSleep/1000;
        requestTimeOutPeriod.tv_nsec = (millisecondsToSleep%1000)*1000000;

        timespec remainingTimeOutPeriod;

        int nanoSleepResult = 0;
        for(;;)
        {
            /* On successfully sleeping for the requested interval, nanosleep() returns 0. If the call is interrupted by a signal handler or encounters an error, then it returns -1, with errno set to indicate the error. */
            nanoSleepResult = nanosleep(&requestTimeOutPeriod,&remainingTimeOutPeriod);

            if (0 == nanoSleepResult)
                /* nanosleep success */
            {
                break;
            }
            /* nanosleep fail */
            else
            {
                /*The pause has been interrupted by a signal that was delivered to the thread. 
                The remaining sleep time has been written into *rem so that the thread can 
                easily call nanosleep() again and continue with the pause.  */
                if (nanoSleepResult == EINTR)
                {
                    requestTimeOutPeriod = remainingTimeOutPeriod;
                    continue;
                }
                /* Other error, sleep fail */
                else
                {
                    break;
                }
            }
        }
    }

    uint32_t BasicTaskModuleImplementation::RunEntry()
    {
        uint32_t returnValue = this->m_ModuleRunner->RunEntry();

        return returnValue;
    }

    MessageTriggeredTaskModuleImplementation::MessageTriggeredTaskModuleImplementation(ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, MessageTriggeredModuleRunner* messageTriggeredModuleRunner)
        :BasicTaskModuleImplementation(modulePriority,moduleStackSize,moduleName,messageTriggeredModuleRunner),
        m_MessageTriggeredModuleRunner(messageTriggeredModuleRunner),m_KeepRunning(false)
    {

    }

    MessageTriggeredTaskModuleImplementation::~MessageTriggeredTaskModuleImplementation()
    {

    }

    uint32_t MessageTriggeredTaskModuleImplementation::RunEntry()
    {
        m_KeepRunning = true;

        while (m_KeepRunning)
        {
            InterTaskMessage::AutoInterTaskMsgPtr pReceivedMsg = this->m_MessageTriggeredModuleRunner->m_MessageQueue.waitDequeueNotification();

            if (false == pReceivedMsg.isNull())
            {
                this->m_MessageTriggeredModuleRunner->OnReceivedMessage(pReceivedMsg.get());
            }
        }

        return 0;
    }

    void MessageTriggeredTaskModuleImplementation::StopProcessMessage()
    {
        this->m_KeepRunning = false;

        MessageTriggeredModuleRunner* theModuleRunner = static_cast<MessageTriggeredModuleRunner*>(this->m_ModuleRunner);

        theModuleRunner->m_MessageQueue.wakeUpAll();
    }

}
