#include <Platform/Include/PlatformTimer.h>
#include <Platform/Include/PlatformModule.h>
#include <Platform/Include/MessageRouting.h>
#include <Platform/Include/PlatformCommonEnums.h>
#include <Platform/Include/debug.h>

namespace CommunicationPlatform
{
    WaitableTimingObject::WaitableTimingObject()
        :WaitableTimingObjectImplementation()
    {
    
    }

    WaitableTimingObject::~WaitableTimingObject()
    {
    
    }

    TimingItemInfo::TimingItemInfo(int64_t PeriodToTimeoutInMicroSecond,uint32_t DestinationNodeId,uint32_t DestinationModuleId,uint32_t TimerId,void* m_TimeOutData)
        :m_PeriodToTimeoutInMicroSecond(PeriodToTimeoutInMicroSecond),
        m_DestinationNodeId(DestinationNodeId),
        m_DestinationModuleId(DestinationModuleId),
        m_TimerId(TimerId),
        m_TimeOutData(m_TimeOutData),
		    m_TimerObjectRef(NULL),
		    m_TimerInstanceId(0)
    {

    }

    TimingItemInfo::~TimingItemInfo()
    {
    
    }


    TimerModuleRunner::TimerModuleRunner(std::string moduleName,int64_t minialTimingTickPeriodInMicroSecond)
        :ModuleRunner(moduleName),
        KeepWorking(true),
        m_MinialTimingTickPeriodInMicroSecond(minialTimingTickPeriodInMicroSecond)
    {
        m_TimingItemsQueue.clear();
    }

    TimerModuleRunner::~TimerModuleRunner()
    {
        m_TimingItemsQueue.clear();
    }

    ModuleRunner::RunnerInitResult TimerModuleRunner::InitRunner()
    {
        if (false == m_TimingTick.Initialize())
        {
            return RUNNER_INIT_FAIL;
        } 

        return RUNNER_INIT_SUCCESS;
    }


    bool TimerModuleRunner::InsertTimingItem(TimingItemInfo& timingItemToInsert)
    {
        #ifdef cdebug
        //printf("\r\nInsertTimeingItem!\r\n");
        #endif
        m_TimingItemsQueueProtectLock.Lock();

        //Find the first timing item whose timeout  period is equal or bigger than this item and insert this item before it
        bool itemIsInserted = false;
        std::list<TimingItemInfo>::iterator timingItemItor = this->m_TimingItemsQueue.begin();
        int64_t previousTotalTimeoutPeriod = 0;
        int64_t currentTotalTimeoutPeriod  = 0;
        for (;timingItemItor!=this->m_TimingItemsQueue.end();timingItemItor++)
        {
            TimingItemInfo& currentTimingItem = *timingItemItor;

            previousTotalTimeoutPeriod = currentTotalTimeoutPeriod;
            currentTotalTimeoutPeriod += currentTimingItem.m_PeriodToTimeoutInMicroSecond;

            if (currentTotalTimeoutPeriod > timingItemToInsert.m_PeriodToTimeoutInMicroSecond)
            {
                timingItemToInsert.m_PeriodToTimeoutInMicroSecond -= previousTotalTimeoutPeriod;

                TimerRunnerQueue::iterator insertedTimerIterator = this->m_TimingItemsQueue.insert(timingItemItor,timingItemToInsert);

                 if( timingItemToInsert.m_TimerObjectRef != NULL)
				 {
					 timingItemToInsert.m_TimerObjectRef->SetTimerQueueIterator(insertedTimerIterator);
				 }

                currentTimingItem.m_PeriodToTimeoutInMicroSecond -= timingItemToInsert.m_PeriodToTimeoutInMicroSecond;

                itemIsInserted = true;

                break;
            }
        }

        //In case currentTotalTimeoutPeriod is less than timingItemToInsert, insert timingItemToInsert to the end of the timing item queue
        if (false == itemIsInserted)
        {
            timingItemToInsert.m_PeriodToTimeoutInMicroSecond -= currentTotalTimeoutPeriod;

            this->m_TimingItemsQueue.push_back(timingItemToInsert);

            TimerRunnerQueue::iterator insertedTimerIterator = this->m_TimingItemsQueue.end();

            insertedTimerIterator--;
			
			 if( timingItemToInsert.m_TimerObjectRef != NULL)
			 {
				 timingItemToInsert.m_TimerObjectRef->SetTimerQueueIterator(insertedTimerIterator);
			 }

            itemIsInserted = true;
        }

         if(timingItemToInsert.m_TimerObjectRef != NULL)
		 {
			 timingItemToInsert.m_TimerObjectRef->m_IsRunning = true;
		 }

        m_TimingItemsQueueProtectLock.Unlock();

        return true;
    }

    bool TimerModuleRunner::RemoveTimingItem(TimerRunnerQueue::iterator& timingItemItor)
    {
        m_TimingItemsQueueProtectLock.Lock();

        this->m_TimingItemsQueue.erase(timingItemItor);

        m_TimingItemsQueueProtectLock.Unlock();

        return true;
    }


    uint32_t TimerModuleRunner::RunEntry()
    {
    	#ifdef cdebug
    	//printf("\r\nrunentry!\r\n");
    	#endif 
        while(true == KeepWorking)
        {
            this->m_TimingTick.Wait(m_MinialTimingTickPeriodInMicroSecond);

            this->m_TimingItemsQueueProtectLock.Lock();

            //There is no timing item needs to be checked for timeout
            if (this->m_TimingItemsQueue.size() == 0)
            {
                this->m_TimingItemsQueueProtectLock.Unlock();
                continue;
            }

            std::list<TimingItemInfo>::iterator timingItemItor = this->m_TimingItemsQueue.begin();

            TimingItemInfo& currentTimingItem = *timingItemItor;

            if (currentTimingItem.m_PeriodToTimeoutInMicroSecond < m_MinialTimingTickPeriodInMicroSecond)
            {
                currentTimingItem.m_PeriodToTimeoutInMicroSecond = 0;
            } 
            else
            {
                currentTimingItem.m_PeriodToTimeoutInMicroSecond -= m_MinialTimingTickPeriodInMicroSecond;
            }

            //Current timing item is timeout
            if (0 == currentTimingItem.m_PeriodToTimeoutInMicroSecond)
            {
                OnItemTimeout(currentTimingItem);
                timingItemItor = this->m_TimingItemsQueue.erase(timingItemItor);

                //Check whether there is any timing items whose timeout period is equal to zero, i.e. timeout period is the same as the "currentTimingItem"
                while (timingItemItor != this->m_TimingItemsQueue.end())
                {
                    TimingItemInfo& checkedZeroTimingItems = *timingItemItor;

                    if (0 != checkedZeroTimingItems.m_PeriodToTimeoutInMicroSecond)
                    {
                        break;
                    }
                    else
                    {
                        OnItemTimeout(checkedZeroTimingItems);
                        timingItemItor = this->m_TimingItemsQueue.erase(timingItemItor);
                    }
                }
            }

            this->m_TimingItemsQueueProtectLock.Unlock();
        }

        return 0;
    }

    void TimerModuleRunner::OnItemTimeout(TimingItemInfo theTimeoutTimerInfo)
    {
    	#ifdef cdebug
    	//printf("\r\nenter OnItemTimeout!\r\n");
    	#endif
        if (NULL != theTimeoutTimerInfo.m_TimerObjectRef)
        {
            theTimeoutTimerInfo.m_TimerObjectRef->m_IsRunning = false;
        }

        NodeMessageRouter* pMessageRouter = NodeMessageRouter::GetMessageRouter();

        if (NULL == pMessageRouter)
        {
        	#ifdef cdebug
        	//printf("\r\n1:error\r\n");
        	#endif
            return;
        }

        ModuleDestinationInfo* pDestination = pMessageRouter->GetDestination(theTimeoutTimerInfo.m_DestinationNodeId,theTimeoutTimerInfo.m_DestinationModuleId);

        if (NULL == pDestination)
        {
            #ifdef cdebug
        	//printf("\r\n2:error\r\n");
        	#endif
            return;
        }

        PlatformTimerTimeoutIndMsg* pTimeOutMsg = new PlatformTimerTimeoutIndMsg();

        if (NULL == pTimeOutMsg)
        {
            #ifdef cdebug
        	//printf("\r\n3:error\r\n");
        	#endif
            return;
        }

        pTimeOutMsg->pTimerTimoutData  = theTimeoutTimerInfo.m_TimeOutData;
        pTimeOutMsg->m_TimerId         = theTimeoutTimerInfo.m_TimerId;
		    pTimeOutMsg->m_TimerInstanceId = theTimeoutTimerInfo.m_TimerInstanceId;
		   
		//add by caokun0310 
		#ifdef cdebug
		//printf("\r\nsend message!\r\n");
		#endif
        pDestination->SendMessageNew(this->m_NodeId,PLATFORM_INTERNAL_MODULE_TIMER,
                                     PLATFORM_MESSAGE_TIMER_TIMEOUT_IND,pTimeOutMsg,sizeof(PlatformTimerTimeoutIndMsg));

    }

    TimerModuleRunner* TimerModuleRunner::m_MilliSecondTimerQueue = NULL;  /**< Minimal Period is half of a millisecond */
    TimerModuleRunner* TimerModuleRunner::m_SecondTimerQueue      = NULL;  /**< Minimal Period is half of a second */
    TimerModuleRunner* TimerModuleRunner::m_MinuteTimerQueue      = NULL;  /**< Minimal Period is half of a minute */

    BasicTaskModule*   TimerModuleRunner::m_MilliSecondTimerTask  = NULL;
    BasicTaskModule*   TimerModuleRunner::m_SecondTimerTask       = NULL;
    BasicTaskModule*   TimerModuleRunner::m_MinuteTimerTask       = NULL;

    /**
    *   Initialize the timer queues and start the corresponding tasks
    * */
    bool TimerModuleRunner::InitTimerMechanisms()
    {
        m_MilliSecondTimerQueue = new TimerModuleRunner("MilliSecondTimerQueue",20);
        m_MilliSecondTimerTask  = new BasicTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,TASK_PRIORITY_HIGHEST,32*1024,"MilliSecondTimerQueue Task",m_MilliSecondTimerQueue);

        if (    (NULL == m_MilliSecondTimerQueue)
             || (NULL == m_MilliSecondTimerTask)
           )
        {
            if (NULL != m_MilliSecondTimerQueue)
            {
                delete m_MilliSecondTimerQueue;
                m_MilliSecondTimerQueue = NULL;
            }

            if (NULL != m_MilliSecondTimerTask)
            {
                delete m_MilliSecondTimerTask;
                m_MilliSecondTimerTask = NULL;
            }

            return false;
        }

        ModuleCreateResult startResultForMilliSecondTimerTask = m_MilliSecondTimerTask->Start();

        if (startResultForMilliSecondTimerTask != TASK_CREATE_RESULT_SUCCESS)
        {
            delete m_MilliSecondTimerQueue;
            m_MilliSecondTimerQueue = NULL;

            delete m_MilliSecondTimerTask;
            m_MilliSecondTimerTask = NULL;

            return false;
        }

        m_SecondTimerQueue = new TimerModuleRunner("SecondTimerQueue",50*1000);
        m_SecondTimerTask  = new BasicTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,TASK_PRIORITY_HIGHEST,32*1024,"SecondTimerQueue Task",m_SecondTimerQueue);

        if (   (NULL == m_SecondTimerQueue) 
             ||(NULL == m_SecondTimerTask)
           )
        {
            if (NULL != m_SecondTimerQueue)
            {
                delete m_SecondTimerQueue;
                m_SecondTimerQueue = NULL;
            }

            if (NULL != m_SecondTimerTask)
            {
                delete m_SecondTimerTask;
                m_SecondTimerTask = NULL;
            }
            return false;
        }
        
        ModuleCreateResult startResultForSecondTimerTask = m_SecondTimerTask->Start();

        if (startResultForSecondTimerTask != TASK_CREATE_RESULT_SUCCESS)
        {
            /* Clean MilliSecond Timer related resources */
            m_MilliSecondTimerQueue->KeepWorking = false;
            m_MilliSecondTimerTask->Join();

            delete m_MilliSecondTimerQueue;
            m_MilliSecondTimerQueue = NULL;

            delete m_MilliSecondTimerTask;
            m_MilliSecondTimerTask = NULL;

            /* Clean Second Timer related resources */
            delete m_SecondTimerQueue;
            m_SecondTimerQueue = NULL;

            return false;
        }

        m_MinuteTimerQueue = new TimerModuleRunner("MinutesTimerQueue",1*1000*1000);
        m_MinuteTimerTask  = new BasicTaskModule(PLATFORM_INTERNAL_NODE_THIS_NODE,TASK_PRIORITY_HIGHEST,32*1024,"MinutesTimerQueue Task",m_MinuteTimerQueue);
        if (  (NULL == m_MinuteTimerQueue)
            ||(NULL == m_MinuteTimerTask)
           )
        {
            if (NULL != m_MinuteTimerQueue)
            {
                delete m_MinuteTimerQueue;
                m_MinuteTimerQueue = NULL;
            }

            if (NULL != m_MinuteTimerTask)
            {
                delete m_MinuteTimerTask;
                m_MinuteTimerTask = NULL;
            }

            return false;
        }
        
        ModuleCreateResult startResultForMinutesTimerTask = m_MinuteTimerTask->Start();

        if (startResultForMinutesTimerTask != TASK_CREATE_RESULT_SUCCESS)
        {
            /* Clean MilliSecond Timer related resources */
            m_MilliSecondTimerQueue->KeepWorking = false;
            m_MilliSecondTimerTask->Join();

            delete m_MilliSecondTimerQueue;
            m_MilliSecondTimerQueue = NULL;

            delete m_MilliSecondTimerTask;
            m_MilliSecondTimerTask = NULL;

            /* Clean Second Timer related resources */
            m_SecondTimerQueue->KeepWorking = false;
            m_SecondTimerTask->Join();

            delete m_SecondTimerQueue;
            m_SecondTimerQueue = NULL;

            delete m_SecondTimerTask;
            m_SecondTimerTask = NULL;

            /* Clean Minute Timer related resources */
            delete m_MinuteTimerQueue;
            m_MinuteTimerQueue = NULL;

            delete m_MinuteTimerTask;
            m_MinuteTimerTask = NULL;

            return false;
        }

        return true;
    }


    bool TimerModuleRunner::StartTimer(MessageNotifiedTimer& timerToStart,void* timerSpecificData)
    {
        if (true == timerToStart.m_IsRunning)
        {
            TimerModuleRunner::StopTimer(timerToStart);
        }
        
        timerToStart.PrepareStartTimer();
        TimingItemInfo& timingItemToInsert   = timerToStart.GetTimingItemInfos();
        timingItemToInsert.m_TimeOutData     = timerSpecificData;
        timingItemToInsert.m_TimerObjectRef  = &timerToStart;
	    	timingItemToInsert.m_TimerInstanceId = timerToStart.IncreaseTimerInstanceId();

        switch (timerToStart.GetTimerPeriodUnit())
        {
        case TIMER_PERIOD_UNIT_MINUTES:
            {
                timerToStart.SetTimerRunner(m_MinuteTimerQueue);

                m_MinuteTimerQueue->InsertTimingItem(timingItemToInsert);
            }
        	break;
        case TIMER_PERIOD_UNIT_SECOND:
            {
                timerToStart.SetTimerRunner(m_SecondTimerQueue);

                m_SecondTimerQueue->InsertTimingItem(timingItemToInsert);
            }
            break;
        case TIMER_PERIOD_UNIT_MILLISECOND:
        case TIMER_PERIOD_UNIT_MICROSECOND:
            {
                timerToStart.SetTimerRunner(m_MilliSecondTimerQueue);

                m_MilliSecondTimerQueue->InsertTimingItem(timingItemToInsert);
            }
            break;
        default:
            {
                timerToStart.SetTimerRunner(m_MilliSecondTimerQueue);

                m_MilliSecondTimerQueue->InsertTimingItem(timingItemToInsert);
            }
            break;
        }

        return true;
    }

    bool TimerModuleRunner::StopTimer(MessageNotifiedTimer& timerToStop)
    {
        if (false == timerToStop.m_IsRunning)
        {
            return true;
        }

        if (true == timerToStop.GetTimerRunner()->RemoveTimingItem(timerToStop.GetTimerQueueIterator()))
        {
			      timerToStop.IncreaseTimerInstanceId();

            timerToStop.SetTimerRunner(NULL);

            timerToStop.m_IsRunning = false;

            return true;
        }
        else
        {
            return false;
        }
    }


    MessageNotifiedTimer::MessageNotifiedTimer(TimerPeriodUnit timerUnit, int64_t periodToTimeout,uint32_t DestinationNodeId,uint32_t DestinationModuleId,uint32_t TimerId)
        :m_IsRunning(false),
         m_TimingItemInfo(0,DestinationNodeId,DestinationModuleId,TimerId),
		     m_TimerInstanceId(0)
    {
        ResetTimerPeriod(timerUnit,periodToTimeout);
    }

    void MessageNotifiedTimer::PrepareStartTimer()
    {
        this->m_TimingItemInfo.m_PeriodToTimeoutInMicroSecond = this->m_PeriodToTimeoutInMicroSecond;
    }

    void MessageNotifiedTimer::ResetTimerPeriod(TimerPeriodUnit timerUnit, int64_t periodToTimeout)
    {
        this->m_TimerPeriodUnit = timerUnit;

        switch (timerUnit)
        {
        case TIMER_PERIOD_UNIT_MINUTES:
            {
                this->m_PeriodToTimeoutInMicroSecond = periodToTimeout * 60 * 1000 * 1000;
            }
            break;
        case TIMER_PERIOD_UNIT_SECOND:
            {
                this->m_PeriodToTimeoutInMicroSecond = periodToTimeout * 1000 * 1000;
            }
            break;
        case TIMER_PERIOD_UNIT_MILLISECOND:
            {
                this->m_PeriodToTimeoutInMicroSecond = periodToTimeout * 1000;
            }
            break;
        case TIMER_PERIOD_UNIT_MICROSECOND:
            {
                this->m_PeriodToTimeoutInMicroSecond = periodToTimeout;
            }
            break;
        default:
            break;
        }

        this->m_TimingItemInfo.m_PeriodToTimeoutInMicroSecond = this->m_PeriodToTimeoutInMicroSecond;
    }
}
