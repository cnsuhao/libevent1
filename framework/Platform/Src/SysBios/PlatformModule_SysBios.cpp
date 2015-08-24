#include <Platform/Include/SysBios/SysBiosSpecificHeaders.h>
#include <Platform/Include/SysBios/PlatformModule_SysBios.h>
#include <Platform/Include/InterTaskMessage.h>
#include <Platform/Include/PlatformCommonEnums.h>

namespace CommunicationPlatform
{
    uint32_t BasicTaskModuleImplementation::ModuleEntryReturnValue;

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

    int BasicTaskModuleImplementation::CalculatePthreadPriorityValue(ModulePriority abstractPriority)
    {
        /* The maximum priority level is Task_numPriorities-1 (default=15; maximum=31). The minimum priority is 
           1. If the priority is less than 0, the task is barred from further execution until its priority is raised at a later 
           time by another task. If the priority equals Task_numPriorities-1, the task cannot be preempted by another task. */
        int minPriorityInSystem = 1;
        int maxPriorityInSystem = Task_numPriorities - 1;

        int priorityScope = maxPriorityInSystem - minPriorityInSystem;

        int returnedPriority = 0;

        switch (abstractPriority)
        {
        case TASK_PRIORITY_HIGHEST:
            {
                returnedPriority = maxPriorityInSystem;
            }
            break;
        case TASK_PRIORITY_HIGH:
            {
                returnedPriority =  (minPriorityInSystem + 7*(priorityScope/8));
            }
            break;
        case TASK_PRIORITY_MIDDLE_HIGH:
            {
                returnedPriority =  (minPriorityInSystem + 3*(priorityScope/4));
            }
            break;
        case TASK_PRIORITY_MIDDLE_LOW:
            {
                returnedPriority =  (minPriorityInSystem + (priorityScope/4));
            }
            break;
        case TASK_PRIORITY_LOW:
            {
                returnedPriority =  (minPriorityInSystem + (priorityScope/8));
            }
            break;
        case TASK_PRIORITY_LOWEST:
            {
                returnedPriority =  minPriorityInSystem;
            }
            break;
        case TASK_PRIORITY_MIDDLE:
        default:
            {
                returnedPriority =  (minPriorityInSystem + (priorityScope/2));
            }
            break;
        }

        return returnedPriority;
    }

    ModuleCreateResult BasicTaskModuleImplementation::Start()
    {
        Task_Params thisTaskParams;

        Task_Params_init(&thisTaskParams);

        /* Set stack size */
        thisTaskParams.stackSize = this->m_StackSize;

        /* Set thread scheduling priority */
        thisTaskParams.priority = CalculatePthreadPriorityValue(this->m_Priority);

        /* Set argument 0 to pointer to this */
        thisTaskParams.arg0 = (UArg)this;

        this->m_ThreadHandle = Task_create(&BasicTaskModuleImplementation::ModuleEntry,&thisTaskParams,NULL);

        if (NULL == this->m_ThreadHandle)
        {
            return TASK_CREATE_RESULT_FAIL_BEGIN_THREAD_ERROR;
        }

        return TASK_CREATE_RESULT_SUCCESS;
    }

    Void BasicTaskModuleImplementation::ModuleEntry(UArg arg0, UArg arg1)
    {
        BasicTaskModuleImplementation* pCurrentThread = (BasicTaskModuleImplementation*)arg0;

        ModuleEntryReturnValue = pCurrentThread->RunEntry();

        return ;
    }

    bool BasicTaskModuleImplementation::Join()
    {
       //Unsupported Interface Join;

        return true;
    }

    void BasicTaskModuleImplementation::Sleep(uint32_t millisecondsToSleep)
    {
        Task_sleep(ConvertMicroSecondsToSystemTick(millisecondsToSleep*1000));
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
            InterTaskMessage* pReceivedMsg = this->m_MessageTriggeredModuleRunner->m_MessageQueue.waitDequeueNotification();

            if (NULL != pReceivedMsg)
            {
                this->m_MessageTriggeredModuleRunner->OnReceivedMessage(pReceivedMsg);
            }
        }

        return 0;
    }

    void MessageTriggeredTaskModuleImplementation::StopProcessMessage()
    {
        this->m_KeepRunning = false;

        MessageTriggeredModuleRunner* theModuleRunner = static_cast<MessageTriggeredModuleRunner*>(this->m_ModuleRunner);

        InterTaskMessage* stopProcessReq = new InterTaskMessage(PLATFORM_MESSAGE_STOP_PROCESSING_MESSAGE_REQ,
                                                                sizeof(InterTaskMessage),
                                                                PLATFORM_INTERNAL_NODE_THIS_NODE,
                                                                PLATFORM_INTERNAL_MODULE_MESSAGE_ROUTER,
                                                                InterTaskMessage::MESSAGE_DATA_MEMORY_TYPE_NONE
                                                                );
        theModuleRunner->m_MessageQueue.enqueueNotification(stopProcessReq);
    }

}
