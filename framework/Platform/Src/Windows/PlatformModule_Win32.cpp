#include <Platform/Include/Windows/WindowsSpecificHeaders.h>
#include <Platform/Include/Windows/PlatformModule_Win32.h>
#include <Platform/Include/InterTaskMessage.h>

#include <process.h>


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

    void BasicTaskModuleImplementation::Cleanup()
    {
        if (!this->m_ThreadHandle)
        {
            return;
        }

        if (CloseHandle(this->m_ThreadHandle))
        {
            this->m_ThreadHandle = 0;
        }
    }

    ModuleCreateResult BasicTaskModuleImplementation::Start()
    {
        this->m_ThreadHandle = (HANDLE)_beginthreadex(NULL,this->m_StackSize,&CommunicationPlatform::BasicTaskModuleImplementation::ModuleEntry,this,CREATE_SUSPENDED ,&(this->m_ThreadId));

        /*  _beginthreadex returns 0 on an error, in which case errno and _doserrno are set. */
        if (0 == this->m_ThreadHandle)
        {
            return TASK_CREATE_RESULT_FAIL_BEGIN_THREAD_ERROR;
        }

        BOOL setPriorityResult = FALSE;

        switch (this->m_Priority)
        {
        case TASK_PRIORITY_HIGHEST:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_TIME_CRITICAL);
            }
        	break;
        case TASK_PRIORITY_HIGH:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_HIGHEST);
            }
            break;
        case TASK_PRIORITY_MIDDLE_HIGH:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_ABOVE_NORMAL);
            }
            break;
        case TASK_PRIORITY_MIDDLE:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_NORMAL);
            }
            break;
        case TASK_PRIORITY_MIDDLE_LOW:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_BELOW_NORMAL);
            }
            break;
        case TASK_PRIORITY_LOW:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_LOWEST);
            }
            break;
        case TASK_PRIORITY_LOWEST:
            {
                setPriorityResult = SetThreadPriority(this->m_ThreadHandle,THREAD_PRIORITY_IDLE);
            }
            break;
        default:
            break;
        }

        if (FALSE == setPriorityResult)
        {
            CloseHandle(this->m_ThreadHandle);
            return TASK_CREATE_RESULT_FAIL_SET_PRIORITY_ERROR;
        }

        if (ModuleRunner::RUNNER_INIT_SUCCESS != this->m_ModuleRunner->InitRunner())
        {
            CloseHandle(this->m_ThreadHandle);
            return TASK_CREATE_RESULT_FAIL_RUNNER_INIT_FAIL;
        }
        
        DWORD resumeThreadResult = ResumeThread(this->m_ThreadHandle);
        /* If the function fails, the return value is (DWORD) -1. To get extended error information, call GetLastError. */
        if (-1 == resumeThreadResult)
        {
            CloseHandle(this->m_ThreadHandle);
            return TASK_CREATE_RESULT_FAIL_RESUME_THREAD_ERROR;
        }

        return TASK_CREATE_RESULT_SUCCESS;
    }

    bool BasicTaskModuleImplementation::Join()
    {
        if (!m_ThreadHandle)
        {
            return true;
        }

        switch (WaitForSingleObject(this->m_ThreadHandle, INFINITE))
        {
        case WAIT_OBJECT_0:
            Cleanup();
            return true;
        default:
            return false;
        }
    }

    void BasicTaskModuleImplementation::Sleep(uint32_t millisecondsToSleep)
    {
        ::Sleep(millisecondsToSleep);
    }

    unsigned __stdcall BasicTaskModuleImplementation::ModuleEntry(LPVOID pThread)
    {
        BasicTaskModuleImplementation* pCurrentThread = (BasicTaskModuleImplementation*)pThread;

        uint32_t returnValue = pCurrentThread->RunEntry();

        return returnValue;
    }

    uint32_t BasicTaskModuleImplementation::RunEntry()
    {
        uint32_t returnValue = this->m_ModuleRunner->RunEntry();

        return returnValue;
    }

    MessageTriggeredTaskModuleImplementation::MessageTriggeredTaskModuleImplementation(ModulePriority modulePriority, uint32_t moduleStackSize, std::string moduleName, MessageTriggeredModuleRunner* messageTriggeredModuleRunner)
        :m_MessageTriggeredModuleRunner(messageTriggeredModuleRunner),m_KeepRunning(false),
        BasicTaskModuleImplementation(modulePriority,moduleStackSize,moduleName,messageTriggeredModuleRunner)
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
